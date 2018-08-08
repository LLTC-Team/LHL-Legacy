# LHL QuickStart

LHL是一种由[creatorlxd](https://github.com/creatorlxd)开发的编程语言。LHL是一种[Lisp](https://zh.wikipedia.org/wiki/LISP)方言，语言设计将会参考[Clojure](https://clojure.org/)。LHL是函数式、强类型、编译型的语言，但是其编译后的字节码需要LVM解释运行。

本文档将会简述LHL的相关语法。这个文档还没有完成，并且等待其他成员的审核。

## 基础概念

- `LISP`，全称为`LISt Processor（列表处理器）`。顾名思义，LISP就是一个处理列表的语言，因此理论上LISP中只有一种数据类型和少数的公理（运算符）。LHL身为LISP的一种方言，将会遵守LISP等准则，但是同时提供一些更为方便的函数、宏、语法糖和数据类型。
- 表达式是LHL中的最小代码单元。函数是LHL中最小的可重用的代码集合。一切操作都应该在函数内进行。
- LHL中的数值类型包括了一般编程语言的常见类型，有`int`（整数默认类型）,`long`,`short`,`byte`,`double`（浮点数默认类型）,`float`,`boolean`。具体操作会在下面详细描述。

## 字面量

```clojure
// 整数类型
1    // int
1L   // long
1S   // short
1B   // byte

// 浮点类型
3.14      // double
3.14e11   // double 科学计数法
3.14F     // float

// 其他类型
null     // null
true     // boolean
"hello"  // 字符串 String
\c       // 字符
aFunc    // 符号 函数
map      // 符号 类型
stdlib   // 符号 命名空间
// 注意:上面的符号的名字可能和实际的语言有所差异

// 基础数据类型
`(1 2 3)      // 列表 List (前面的`是必须的)
[1 2 3]       // 向量 Vector
#{1 2 3}      // 集合 Set
{(1 2) (3 4)} // 映射表 Map
```

## 列表与表达式

LHL身为一种Lisp方言，它的表达式就是列表，列表就是表达式。这体现了LISP中重要的一点，“数据即程序、程序即数据”。下面的图片来自Clojure的文档，但也可以用来说明LHL中列表和表达式的关系：

![列表和表达式的关系](https://clojure.org/images/content/guides/learn/syntax/structure-and-semantics.png)

圆括号在LISP中有着特殊的意义。从数据结构上来说，上面的表达式是一个包含一个符号和两个整数的列表；但是从语义上来说，上面表达了一个函数调用，符号代表要调用的函数，而两个整数是其调用参数。

但是如果只想要一个列表，或者想要延迟求值，就需要让它保持身为数据结构的状态。这时就是宏`'`派上用场的时候了。比如下面的代码试图将一个列表赋值给`x`：

```clojure
(def x (1 2 3))
```

但是这么做是错的。因为LHL试图对`(1 2 3)`进行求值，而把`1`理解为一个表示函数的符号。但是显然一个整数不是符号，就会出现类型不符的错误。

下面是正确写法：

```clojure
(def x '(1 2 3))
```

加了`'`以后，`'(1 2 3)`就不会被求值而直接当作一个列表处理。这样就会可以直接赋值给`x`了。

当然，使用函数`list`直接构造一个列表的方法也是可行的，如：

```clojure
(def x (list 1 2 3))
```

（其实`'`是LISP中基础运算符`quote`的语法糖。`quote`是实现LISP中 **宏** 的重要一环）

## 基础运算符

这里将会描述LHL中对LISP的七个基础运算符的表达。

1. `quote`    `(quote x)`的结果即为`x`。可以用`'x`表示。被`quote`的单一元素即为符号。
2. `atom`     `(atom x)`返回`boolean`类型的结果。其返回`false`当且仅当`x`为 **非空列表** 。若`(atom x)`返回`x`，要么`x`属于`atom`类型，要么`x`是`null`。
3. `equal`    `(equal x y)`返回`boolean`类型的结果。其返回`true`当且仅当`x`与`y`所引用的对象相同。
4. `first`    `(first x)`返回列表`x`的第一个元素。
5. `rest`     `(rest x)`返回列表`x`第一个元素之外的剩余元素。
6. `cons`     `(cons x seq)`返回一个列表，其第一个元素为`x`，其余部分为`seq`中的所有元素。（注：LHL中的`cons`并不采用LISP中更加标准的定义，而是采用了Clojure的方式。）
7. `cond`     `(cond (p1 e1) ... (pn en))`（其中`p`都为 **条件表达式** ，即返回`boolean`的表达式）返回第一个返回`true`的条件表达式`pi`所对应的`ei`。如果所有的`p`都返回`false`，则返回`null`。



## REFERENCE

- [LISP](https://zh.wikipedia.org/wiki/LISP)
- [cons](https://zh.wikipedia.org/wiki/%E5%88%97%E8%A1%A8%E6%A7%8B%E9%80%A0%E5%87%BD%E6%95%B8)
- [S-expression](https://zh.wikipedia.org/wiki/S-%E8%A1%A8%E8%BE%BE%E5%BC%8F)
- [atom and symbol](https://kuanyui.github.io/2016/02/26/lisp-atom-and-symbol/)
- [clojure](https://clojure.org/index)
- [clojure ref doc](https://clojure.org/reference/documentation)
- [clojure guide](https://clojure.org/guides/guides)
- [ANSI Common LISP doc](http://acl.readthedocs.io/en/latest/zhCN/index.html)