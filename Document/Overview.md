# 概览

## 关于本项目

LVM 是下一代的LML语言虚拟机.目前项目维护者是 [@createlxd](https://github.com/creatorlxd) ,另一位合作者(打酱油的蒟蒻)是 [@xsun2001](https://github.com/xsun2001) . 目前的LVM实现了一些基础功能,但是我们会将它完善成一个功能完全的,可扩展的,高性能的语言虚拟机(就像JVM一样).

## 构建运行

1. 前置要求: [Git](https://git-scm.com/downloads), [Visual Studio](https://www.visualstudio.com/vs/) (推荐2017版本)
1. 使用如下命令将代码仓库克隆到本地 `git clone https://github.com/creatorlxd/LVM.git <dir>`. 其中的\<dir\>是你想要存储 LVM 代码的目录.
1. 使用 Visual Studio 打开代码根目录中的 `LVM.sln`(Windows);.
1. 愉快地玩耍吧.

## 总体架构

LVM 属于字节码虚拟机,也就是说它执行的的是所谓的字节码,而不是原始代码文件.关于字节码指令集的更多信息,请参考 [字节码指令集(待填坑)]() , [LML](https://github.com/creatorlxd/LML) 以及 [Commands.hpp](https://github.com/creatorlxd/LVM/blob/master/LVM/Include/Commands.hpp). LVM 身为语言虚拟机,与在其上运行的实际编程语言并没有直接联系,只要给出符合标准的字节码文件,LVM都会将其按照预期的方式执行.

LVM 中的数据类型包含整数(`int`),浮点数(`float`),字符(`char`). 整数(`int`)被规定为32位的有符号整数类型. 浮点数(`float`)是单精度浮点类型. 关于基本的数值计算和位运算都在指令集中定义,更加细节上的处理(比如溢出后的行为)应当遵循c/c++语言默认的行为. 关于类具体在虚拟机中的存在形式将不做限制.