/*
Copyright 2018 creatorlxd

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include "LVM/stdafx.h"
#include "LVM/Command.h"
#include "LVM/Commands.hpp"

using namespace std;
using namespace LVM;

int main(int argv,char** argc)
{
	std::vector<std::string> args;
	std::vector<std::pair<std::thread,bool>> threads;		//thread if_joined
	for (int i = 1; i < argv; i++)
		args.emplace_back(argc[i]);
	for (int i = 0; i < args.size(); i++)
	{
		threads.emplace_back(std::make_pair([=](){
			VirtualMachine vm;
			vm.RunFromFile(args[i]);
		},false));
	}

	//wait for join
	int thread_join_cot = threads.size();
	do
	{
		for (auto& i : threads)
		{
			if (!i.second&&i.first.joinable())
			{
				i.first.join();
				i.second = true;
				thread_join_cot -= 1;
			}
		}
	} while (thread_join_cot != 0);
	return 0;
}
