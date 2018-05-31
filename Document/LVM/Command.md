# LHL 指令集
---
* [NullCommand](#NullCommand)
* [TestCommand](#TestCommand)
* [AssignCommand](#AssignCommand)
* [CopyCommand](#CopyCommand)
* [MoveCommand](#MoveCommand)
* [GotoCommand](#GotoCommand)
* [JumpIfCommand](#JumpIfCommand)

## NullCommand
&emsp;&emsp;空指令  
&emsp;&emsp;`command name:null`  
&emsp;&emsp;`index:0`

## TestCommand
&emsp;&emsp;测试指令，输出`"test command\n"`  
&emsp;&emsp;`command name:test`  
&emsp;&emsp;`index:255`

## AssignCommand
&emsp;&emsp;赋值指令  
&emsp;&emsp;`command name:assign`  
&emsp;&emsp;`index:1`

|参数序号|类型|描述|
|:-|-|:-|
|1|`MemoryAddress`|目标内存地址|
|2|无限制|要被赋值给目标内存的内容|

## CopyCommand
&emsp;&emsp;复制指令  
&emsp;&emsp;`command name:copy`  
&emsp;&emsp;`index:2`

|参数序号|类型|描述|
|:-|-|:-|
|1|`MemoryAddress`|复制的目标内存地址|
|2|`MemoryAddress`|被复制内容的内存地址|
|3|`uint64_t`|要被复制的内存的大小(字节)

## MoveCommand
&emsp;&emsp;移动指令  
&emsp;&emsp;`command name:move`  
&emsp;&emsp;`index:3`

|参数序号|类型|描述|
|:-|-|:-|
|1|`MemoryAddress`|移动的目标内存地址|
|2|`MemoryAddress`|被移动内容的内存地址|
|3|`uint64_t`|要被移动的内存的大小(字节)

## GotoCommand
&emsp;&emsp;跳转指令  
&emsp;&emsp;`command name:goto`  
&emsp;&emsp;`index:4`

|参数序号|类型|描述|
|:-|-|:-|
|1|`uint64_t`|目标指令的位置(**从0开始计数**)|

## JumpIfCommand
&emsp;&emsp;条件跳转指令。如果用于判断的`bool`值为真，就跳转到目标指令位置  
&emsp;&emsp;`command name:jump_if`  
&emsp;&emsp;`index:5`

|参数序号|类型|描述|
|:-|-|:-|
|1|`uint64_t`|目标指令的位置(**从0开始计数**)|
|2|`MemoryAddress`|用于判断的`bool`值的内存地址|