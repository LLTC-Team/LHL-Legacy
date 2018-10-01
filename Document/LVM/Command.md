# LHL 指令集
---
* [NullCommand](#NullCommand)
* [TestCommand](#TestCommand)
* [AssignCommand](#AssignCommand)
* [CopyCommand](#CopyCommand)
* [MoveCommand](#MoveCommand)
* [GotoCommand](#GotoCommand)
* [JumpIfCommand](#JumpIfCommand)
* [IntAddCommand](#IntAddCommand)
* [IntSubCommand](#IntSubCommand)
* [IntMulCommand](#IntMulCommand)
* [IntDivCommand](#IntDivCommand)
* [IntShiftLogicalLeft](#IntShiftlogicalLeftCommand)
* [IntShiftArithmeticRightCommand](#IntShiftArithmeticRightCommand)
* [IntShiftLogicalRightCommand](#IntShiftLogicalRightCommand)
* [IntAndCommand](#IntAndCommand)

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

## IntAddCommand
&emsp;&emsp;整数相加指令  
&emsp;&emsp;`command name:iadd`  
&emsp;&emsp;`index:6`

|参数序号|类型|描述|
|:-|-|:-|
|1|`MemoryAddress`|存放结果的内存地址|
|2|`MemoryAddress`|参数1的地址|
|3|`MemoryAddress`|参数2的地址|

## IntSubCommand
&emsp;&emsp;整数相减指令  
&emsp;&emsp;`command name:isub`  
&emsp;&emsp;`index:7`

|参数序号|类型|描述|
|:-|-|:-|
|1|`MemoryAddress`|存放结果的内存地址|
|2|`MemoryAddress`|参数1的地址|
|3|`MemoryAddress`|参数2的地址|

## IntMulCommand
&emsp;&emsp;整数相乘指令  
&emsp;&emsp;`command name:imul`  
&emsp;&emsp;`index:8`

|参数序号|类型|描述|
|:-|-|:-|
|1|`MemoryAddress`|存放结果的内存地址|
|2|`MemoryAddress`|参数1的地址|
|3|`MemoryAddress`|参数2的地址|

## IntDivCommand
&emsp;&emsp;整数相除指令  
&emsp;&emsp;`command name:idiv`  
&emsp;&emsp;`index:9`

|参数序号|类型|描述|
|:-|-|:-|
|1|`MemoryAddress`|存放结果的内存地址|
|2|`MemoryAddress`|参数1的地址|
|3|`MemoryAddress`|参数2的地址|

## IntShiftLogicalLeftCommand
&emsp;&emsp;整数左移指令
&emsp;&emsp;`command name:ishl`  
&emsp;&emsp;`index:10`

|参数序号|类型|描述|
|:-|-|:-|
|1|`MemoryAddress`|存放结果的内存地址|
|2|`MemoryAddress`|参数1的地址|
|3|`MemoryAddress`|参数2的地址|

## IntShiftArithmeticRightCommand
&emsp;&emsp;整数算术右移指令  
&emsp;&emsp;`command name:isar`  
&emsp;&emsp;`index:11`

|参数序号|类型|描述|
|:-|-|:-|
|1|`MemoryAddress`|存放结果的内存地址|
|2|`MemoryAddress`|参数1的地址|
|3|`MemoryAddress`|参数2的地址|

## IntShiftLogicalRightCommand
&emsp;&emsp;整数逻辑右移指令  
&emsp;&emsp;`command name:ishr`  
&emsp;&emsp;`index:12`

|参数序号|类型|描述|
|:-|-|:-|
|1|`MemoryAddress`|存放结果的内存地址|
|2|`MemoryAddress`|参数1的地址|
|3|`MemoryAddress`|参数2的地址|

## IntAndCommand
&emsp;&emsp;整数与指令  
&emsp;&emsp;`command name:iand`  
&emsp;&emsp;`index:13`

|参数序号|类型|描述|
|:-|-|:-|
|1|`MemoryAddress`|存放结果的内存地址|
|2|`MemoryAddress`|参数1的地址|
|3|`MemoryAddress`|参数2的地址|

## IntOrCommand
&emsp;&emsp;整数或指令  
&emsp;&emsp;`command name:ior`  
&emsp;&emsp;`index:14`

|参数序号|类型|描述|
|:-|-|:-|
|1|`MemoryAddress`|存放结果的内存地址|
|2|`MemoryAddress`|参数1的地址|
|3|`MemoryAddress`|参数2的地址|

## IntXorCommand
&emsp;&emsp;整数异或指令  
&emsp;&emsp;`command name:ixor`  
&emsp;&emsp;`index:15`

|参数序号|类型|描述|
|:-|-|:-|
|1|`MemoryAddress`|存放结果的内存地址|
|2|`MemoryAddress`|参数1的地址|
|3|`MemoryAddress`|参数2的地址|

## IntNotCommand
&emsp;&emsp;整数取非指令  
&emsp;&emsp;`command name:inot`  
&emsp;&emsp;`index:16`

|参数序号|类型|描述|
|:-|-|:-|
|1|`MemoryAddress`|存放结果的内存地址|
|2|`MemoryAddress`|参数1的地址|