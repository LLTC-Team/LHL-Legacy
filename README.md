# Project LHL [![Build Status](https://travis-ci.org/ChinaCreators/LHL.svg?branch=master)](https://travis-ci.org/ChinaCreators/LHL) [![Build status](https://ci.appveyor.com/api/projects/status/t401c73375kssesx?svg=true)](https://ci.appveyor.com/project/xsun2001/lhl)

<!-- TOC -->

- [Project LHL](#project-lhl)
	- [Overview](#overview)
		- [LHL](#lhl)
		- [LML](#lml)
		- [LVM](#lvm)
	- [Build](#build)
		- [前置要求](#前置要求)
		- [构建方法](#构建方法)
			- [Windows（Visual Studio 2017）](#windowsvisual-studio-2017)
			- [其他环境](#其他环境)
	- [Goals](#goals)
		- [Level 1](#level-1)
		- [Level 2](#level-2)
		- [Level 3](#level-3)
	- [Author](#author)

<!-- /TOC -->

## Overview

### LHL

LHL项目由两个子项目构成。它们分别是LML和LVM。

LHL项目旨在创造一种Lisp方言，但是提供给使用者一种高效优雅的方式扩展语言特性。

### LML

LML是一个编译器开发工具。它提供了类似**“通用编译器”**的工具，以便使用者可以快速地构建自己的编程语言的编译器。

LML提供主要的词法分析和中间代码生成工具，同时也预留了语法分析的接口以便于快速扩展。

LML所接受的语言需要是一种**Lisp**方言，输出是**LVM**的字节码。

### LVM

LVM是一个字节码虚拟机，为LHL提供语言运行时环境，并且提供灵活的标准库扩展。

## Build

LHL使用CMake为构建系统以实现跨平台编译。您可以在使用任何CMake支持的平台上使用您喜爱的IDE\Editor进行开发。

### 前置要求

- CMake（版本3.1以上）
- Git
- 支持C++14标准的C++编译器
  - Windows 推荐使用Visual Studio 2017自带的MSVC编译器
  - GCC 5 以上
  - CLang 3.4 以上
  - 以上编译器都推荐使用最新版本
- 一款IDE或编辑器

### 构建方法

#### Windows（Visual Studio 2017）

1. 使用Visual Studio 2017的“文件->打开->文件夹”打开项目根目录即可，CMake支持将会自动加载

#### 其他环境

包括使用MinGW\Cygwin的Windows，Linux，MacOS等其他环境

1. 在项目根目录新建`build`文件夹
2. 在`build`目录下执行命令`cmake ..`。您可以根据个人要求为CMake指定构建目标
3. 如果您的环境已经配置好，那么`build`目录将会存放CMake生成的构建文件
4. 在`build`目录下执行您系统上对应的`make`命令，或是使用您的IDE自动完成配置

## Goals

_以下目标随时可能变动，请以最新文档为准。_

### Level 1

- [ ] 跨平台
  - [x] 使用CMake为构建系统
  - [ ] 跨平台标准库支持
	- [ ] 代码库函数定义接口
	- [ ] 本地代码库动态加载
  - [x] 跨平台编译
- [ ] 基础目标
  - [ ] 词法分析器
  - [ ] LVM字节码生成器
  - [ ] 语法分析器接口
- [ ] 基础语言设施
  - [ ] 函数定义工具
  - [ ] 分支跳转与循环函数
  - [ ] 字面量解析等
- [ ] 最小通用标准库
  - [ ] 基本算术类型与函数
  - [ ] 字符串相关
  - [ ] 基础数据结构
  - [ ] 控制台IO等

### Level 2

- [ ] 底层语言（C\C++）互通
- [ ] 通用标准库
  - [ ] 日期时间库
  - [ ] 数学库
  - [ ] 文件IO
  - [ ] 网络
- [ ] 宏展开
- [ ] 基础垃圾收集器
  - [ ] 引用计数算法

### Level 3

- [ ] 高级标准库
  - [ ] 并发
	- [ ] 线程操控
	- [ ] 原子操作
	- [ ] 并发实用工具
	- [ ] 协程支持
  - [ ] 跨平台GUI库
	- [ ] Qt封装
  - [ ] 3D图形库
	- [ ] OpenGL封装
	- [ ] Vulkan封装
- [ ] 高级垃圾回收
  - [ ] 并发垃圾回收
  - [ ] CAS垃圾回收
  - [ ] JVM G1垃圾回收实验

## Author

- [creatorlxd](https://github.com/creatorlxd)
- [xsun2001](https://github.com/xsun2001)