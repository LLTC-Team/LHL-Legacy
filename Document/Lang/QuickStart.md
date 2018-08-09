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
:a       // 关键字 keyword
aFunc    // 符号 函数
map      // 符号 类型
stdlib   // 符号 命名空间
// 注意:上面的符号的名字可能和实际的语言有所差异
// 关键字（Keyword）参考了Clojure语言的设计

// 基础数据类型
`(1 2 3)      // 列表 List (前面的`是必须的)
[1 2 3]       // 向量 Vector
#{1 2 3}      // 集合 Set
{:a 1 :b 2}   // 映射表 Map
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

## 基础语法

### 定义变量

```clojure
(def x 2)
(def aList '(1 2 3))
(def b True)
(def n null)
(def foo (fn [] (print "hello world")))
```

### 算数运算和逻辑比较

```clojure
(+ 1 2)  // -->  3
(- 2 2)  // -->  0
(* 3 2)  // -->  6
(/ 4 2)  // -->  2
(% 5 2)  // -->  1

// 位运算也差不多
(& 2 3)  // --> 2
(| 2 3)  // --> 3
(^ 2 3)  // --> 1

// 甚至可以传入多个数字
(+ 1 2 3 4 5)  // --> 15

// 算术运算当然可以组合
(+ (* 1 2) (- 3 4) (/ 6 3)) // --> 3

// 数值比较
(= 1 2)  // --> False
(> 1 2)  // --> False
(< 1 2)  // --> True
(>= 1 2) // --> False
(<= 1 2) // --> True

// 逻辑运算
(def b1 True)
(def b2 False)
(not b1)    // --> False
(and b1 b2) // --> False
(or b1 b2)  // --> True
```

### 函数

```clojure
(fn [] ())    // fn可以创建一个匿名函数，参数在[]中，后面的多个表达式为函数体。整个函数返回最后一个表达式的返回值。
(def foo1 (fn [] (print "hello LHL"))) // 可以用def将一个函数赋值给一个符号
(defn foo2 [] (print "hello LHL")) // 使用defn可以简化函数定义
(defn foo3 [x] (+ x x)) // (foo3 2) --> 4

// 变长参数的使用
(defn foo4 [& args] (count args))  // (foo4 2 4 6 8 10) --> 5
// 只有最后一个参数前面可以加&变成变长参数。变长参数实际上会被被编译器包装为一个列表。

// 你当然也可以重载函数喽
(defn foo5
    ([] "hello world")
    ([name] (str "hello " name))
)
// (foo5) --> "hello world"
// (foo5 "LHL") --> "hello LHL"

// 递归当然是滋瓷的
(defn fac1 [x] (if (> x 1) (* x (fac1 (- x 1)) 1)))
// 但是上面的写法相当不推荐，因为这很可能导致栈溢出。
// 应该使用下面的loop-recur递归：
(defn fac2 [x]
    (loop [current x                                 // 绑定递归初始变量
           total 1]
        (if (> current 1)                            // 递归边界条件
            (recur (dec current) (* total current))  // 进行递归
            total
        )
    )
)
// 上面的代码将会被编译器进行尾递归优化，从而使函数只消耗参数规模的栈空间。
// defn fn 其实都隐含了 loop 语句的绑定（即为参数列表），所以可以直接进行 recur
(defn fac3
    ([x] (fac3 x 1))
    ([x t]
        (if (> x 1)
            (recur (dec n) (* t n))
            t
        )
    )
)
// 注意：这只是递归调用的一种实现方式，今后的编译器可能支持自动尾递归优化从而避免使用loop-recur，如：
// (defn fac4
//     ([x] (fac4 x 1))
//     ([x t]
//         (if (> x 1)
//             (fac4 (dec n) (* t n))
//             t
//         )
//     )
// )
```

## REFERENCE

- [LISP](https://zh.wikipedia.org/wiki/LISP)
- [cons](https://zh.wikipedia.org/wiki/%E5%88%97%E8%A1%A8%E6%A7%8B%E9%80%A0%E5%87%BD%E6%95%B8)
- [S-expression](https://zh.wikipedia.org/wiki/S-%E8%A1%A8%E8%BE%BE%E5%BC%8F)
- [atom and symbol](https://kuanyui.github.io/2016/02/26/lisp-atom-and-symbol/)
- [clojure](https://clojure.org/index)
- [clojure ref doc](https://clojure.org/reference/documentation)
- [clojure guide](https://clojure.org/guides/guides)
- [ANSI Common LISP doc](http://acl.readthedocs.io/en/latest/zhCN/index.html)