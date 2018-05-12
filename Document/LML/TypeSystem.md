# TypeSystem 类型系统

&emsp;&emsp;由于`LML`是一种函数式语言平台，所以在`LML`的类型中，不具有指针和引用两种类型。

## LML的类型标记信息(`TypeId`)

### 1.基础类型

类型 | 标记
---- | ----
int | i
char | c
float | f
void | v

### 2.函数类型:

```c++
int aplusb(int a,int n)
{
    return a+b;
}
```

对应的在LML中的类型标记为:

```
F[i][i][i]
```

### 3.类类型

```c++
class test
{

};
```

对应的在LML中的类型标记为：

```
C[test]
```

&emsp;&emsp;你可以通过

```c++
TypeId::TypeId(const string& name);
TypeId::SetAsFunction(const string& name);
TypeId::SetAsClass(const string& name);
```

来设置它们，也可以通过`TypeId::GetContent()`来得到其用以标示的字符串。

&emsp;&emsp;对于类类型来说，还可以用`TypeId::GetClassName()`来得到类的名字（前提是其类型必须是类类型）。

## LML的类型标记概念(`TypeId::TypeConcept`)

&emsp;&emsp;LML的类型标记概念是为了标记一个类型是基本类型、函数、类类型还是指针类型，避免了开发者直接读取`TypeId`中的`string`字符串去判断类型。

```c++
enum class TypeConcept
{
    VariableConcept,
    FunctionConcept,
    ClassConcept
};

```

  你可以使用`TypeId::GetConcept`来获得一个`TypeId`的`Concept`。

## 类型的具体描述(`TypeInformation`)

### 基本概念

&emsp;&emsp;同原来的`ClassInformation`不同，`TypeInformation`不仅用来描述类类型，同时也可以描述基本类型，但是不能用来描述基本类型。  
&emsp;&emsp;`TypeInformation`主要包括以下内容：

1. `TypeId m_TypeId` 类型标识符
2. `uint32_t m_Size` 类型所占的内存的字节大小
3. `std::map<std::string,std::map<TypeId,uint32_t>> m_MemberVariable` 成员变量 第一个`std::string`是变量名，`TypeId`是该成员变量的类型标识，`uint32_t`是该变量的内存地址偏移值。

### 注意事项

1. 由于`LML`是用来构造函数式语言的，所以`LML`的类是不具有封装的功能的。但是具有部分的多态的功能。因为`LML`支持同一个类的不同对象的成员函数可以不同（函数是对象，函数可以被重新赋值）。
2. 一般的变量，一个名字(`std::string`)所对应的`std::map<TypeId,uint32_t>`的`size()`等于1.但是一个函数名所对应的`map`的`size`可以大于等于1。因为函数可以被重载。
3. 当基本类型用`TypeInformation`表示时，其不带有任何的成员变量而具有大小。其大小为

type | size
----- | ----
void | 0
int  | 4
float| 4
char | 1

## 函数类型的具体描述(`FunctionTypeInformation`)

### 概念

&emsp;&emsp;不同于用来描述基本类型与类类型的`TypeInformation`，`FunctionTypeInformation`被设计用来记录/储存函数类型的信息。其主要包括以下内容：

1. `TypeId`类型标识符
2. `uint32_t m_Size=4`类型大小
3. `std::vector<std::pair<TypeId,uint32_t>> VariableList`返回值+参数表
4. `uint32_t ArgumentSize = (VariableList.size() - 1)*4`参数所占的内存空间大小

#### 注意事项

1. 这里的`VariableList`包含了返回值和参数表。其大小(`size`)至少为1，其中返回值是其第一个元素。
2. 每个`VariableList`的元素代表着的相对内存位移值，利用这个偏移值，就可以确定函数的参数表在函数所使用的内存中的分配方式。注意，这个内存偏移值不是指实际上该参数类型所占的内存空间大小(`size`)，而是取决于函数传递参数时，是传递值还是地址。

## 类型管理器(`TypeManager`)

### 概念

&emsp;&emsp;类型管理器是用来储存/查询类型信息的一个管理器，其主要包括以下内容：

1. `std::map<TypeId,TypeInformation> m_TypeInformation`非函数类型的具体描述
2. `std::map<TypeId,FunctionTypeInformation> m_FunctionTypeInformation`函数类型的具体描述

&emsp;&emsp;其主要包括以下方法：

1. `void InsertType(const TypeInformation& type)`插入普通类型信息

2. `void InsertType(const FunctionTypeInformation& type)`插入函数类型信息

3. `TypeInformation GetTypeInformation<TypeInformation>(const TypeId& type_id)`通过`TypeId`获取一个`TypeInformation`

4. `FunctionTypeInformation GetTypeInformation<FunctionTypeInformation>(const TypeId& type_id)`通过`TypeId`获取一个`FunctionTypeInformation`

#### 注意事项

1. 在插入和查找时，需先判断类型是不是函数类型。如果是，就在`m_FunctionTypeInformaiton`中操作，否则就在`m_TypeInformation`中操作。
2. 全局只有一个`TypeManager`。通过`GetTypeManger()`获取。
