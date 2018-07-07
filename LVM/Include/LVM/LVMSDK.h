#include <string>
#include <vector>
#include <map>
#include <functional>
#include <iostream>

namespace LVM
{
	using LVMSDKFunction = std::function<void(std::vector<std::pair<void*, size_t>>)>;

	class Library
	{
	public:
		Library(const std::string& name, const std::string& author_name, const std::string& version, const std::string& lvm_version, const std::map<std::string, LVMSDKFunction>& functions);
		const LVMSDKFunction& GetFunction(const std::string& name);
	public:
		std::string m_Name;
		std::string m_AuthorName;
		std::string m_Version;
		std::string m_LVMVersion;
	private:
		std::map<std::string, LVMSDKFunction> m_Function;
	};
	
	LVM::Library::Library(const std::string & name, const std::string & author_name, const std::string & version, const std::string & lvm_version, const std::map<std::string, LVMSDKFunction>& functions)
		:m_Name(name),m_AuthorName(author_name),m_Version(version),m_LVMVersion(lvm_version),m_Function(functions)
	{

	}

	inline const LVMSDKFunction & Library::GetFunction(const std::string & name)
	{
		auto iter = m_Function.find(name);
		if (iter == m_Function.end())
		{
			std::cout << "can not find this function" << std::endl;
			abort();
		}
		return iter->second;
	}

#define LVM_LIBRARY_DECLARE const LVM::Library& GetLibrary();
#define LVM_LIBRARY_IMPLEMENT(lib_name,author_name,version,lvm_version,...) \
	const LVM::Library& GetLibrary() \
	{ \
		static LVM::Library g_Library(lib_name,author_name,version,lvm_verion,__VA_ARGS__);\
		return g_Library;\
	}
#define LVM_LIBRARY_FUNCTION(func) std::make_pair(#func,LVM::GetLibraryFunction(func))

	/*
	only use for function wrap
	*/
	template<typename T>
	inline T GetLibraryFunctionArgument(size_t cot, std::vector<std::pair<void*, size_t>> args)
	{
		if (sizeof(T) == args[cot].second)
			return (*(T*)(args[cot].first));
		else
		{
			abort();
			return T();
		}
	}

	/*
	only use for function wrap
	*/
	template<typename ReturnType,typename... ArgType>
	inline LVMSDKFunction GetLibraryFunction(ReturnType(*func)(ArgType...))
	{
		return [=](std::vector<std::pair<void*, size_t>> args)
		{
			assert(sizeof...(ArgType) == args.size());
			size_t cot = 0;
			func(GetLibraryFunctionArgument<ArgType>(cot++, args)...);
		};
	}
}