---
__欢迎使用 DMarked :)__

---

## 字体

**我是粗体**

__我是粗体__

*我是斜体*

_我是斜体_

~~我是删除线~~

<u>带下划线文本</u>


## 分隔线

___

---

***


## 排版替换


(c) (C) (r) (R) (tm) (TM) (p) (P) +-

test.. test... test..... test?..... test!....

!!!!!! ???? ,,  -- ---

"Smartypants, double quotes" and 'single quotes'


## 块引用


> 块引用也可以嵌套 ...
>> ... 通过使用连续的 '>' ...
> > > ... '>' 之间可以有空格。


## 列表

无序列表

+ 无序列表使用星号(*)、加号(+)或是减号(-)作为列表标记
- 这些标记后面要添加一个空格，然后再填写内容


有序列表

1. 有序列表使用数字并加上 . 号来表示
2. 列表嵌套只需在子列表中的选项前面多添加2个空格即可:
3. 第一项：
    - 第一项嵌套的第一个元素
    - 第一项嵌套的第二个元素
4. 第二项：
    1. 第二项嵌套的第一个元素
    2. 第二项嵌套的第二个元素


也可以不从 1 开始:

57. foo
1. bar


## 代码

行内代码 `code`

代码区块

代码区块使用 4 个空格或者一个制表符（Tab 键）。

    // Some comments
    line 1 of code
    line 2 of code
    line 3 of code


你也可以用 ``` 包裹一段代码，并指定一种语言（也可以不指定）：

``` js
var foo = function (bar) {
  return bar++;
};

console.log(foo(5));
```

## 表格

Markdown 制作表格使用 | 来分隔不同的单元格，使用 - 来分隔表头和其他行。

|  表头   | 表头  |
|  ----  | ----  |
| 单元格  | 单元格 |
| 单元格  | 单元格 |

对齐方式

我们可以设置表格的对齐方式：
-    -: 设置内容和标题栏居右对齐。
-    :- 设置内容和标题栏居左对齐。
-    :-: 设置内容和标题栏居中对齐。

 左对齐 | 右对齐 | 居中对齐 |
| :-----| ----: | :----: |
| 单元格001 | 单元格 | 单元格003 |
| 单元格 | 单元格002 | 单元格 |

## 链接

这是一个链接 [DMarked](https://github.com/wineee/DMarked)

[点我](http://nodeca.github.io/pica/demo/ "一个图片剪裁工具")


## 图片
Markdown 图片语法格式如下：

```
![alt 属性文本](图片地址)
![alt 属性文本](图片地址 "可选标题")
```

![Minion](https://octodex.github.com/images/minion.png)

你也可以像网址那样对图片网址使用变量:

![Alt text][id]


[id]: https://octodex.github.com/images/dojocat.jpg  "The Dojocat"

## 来自插件的功能
## markdown-it-mark

==标记==

```
==marked== => <mark>inserted</mark>
```

## markdown-it-addr

*[HTML]: Hyper Text Markup Language
*[W3C]:  World Wide Web Consortium
The HTML specification
is maintained by the W3C.


## markdown-it-deflist.min.js
> https://pandoc.org/MANUAL.html#definition-lists

条目 1

:   定义 1
with lazy continuation.

条目 2 *单行描述*

:   定义 2

        { 一些代码, 定义 2 的一部分 }

    定义 2 的第三段。

_紧凑型:_

条目 1
  ~ 定义 1

条目 2
  ~ 定义 2a
  ~ 定义 2b

## markdown-it-sub
H~2~0

```html
H~2~0 => H<sub>2</sub>O
```

## markdown-it-sup

29^th^

```html
29^th^ => 29<sup>th</sup>
```

## markdown-it-ins

++下划线++

```html
++inserted++ => <ins>inserted</ins>
```

## markdown-it-emoji
- Classic markup: :wink: :cry: :laughing: :yum:
- Shortcuts (emoticons): :-) :-( 8-) ;)

用 twemoji [可以进一步美化](https://github.com/markdown-it/markdown-it-emoji#change-output)。

## markdown-it-container

::: warning
*这里是警告条目*
:::

:::success
*这里是成功条目*
:::

## math

设第 $i$ 天需要 $x_i$ 个志愿者，记 $a_{ij}$ 为第 $i$ 天第 $j$ 个志愿者是否能工作。
题目要求总费用最低，即求：$Min \, z = \sum_{i=1}^{n}{C_i * x_i}$。
它们需要满足要求：

$$\begin{cases}
a_{11}x_1+a_{12}x_2+a_{13}x_3...+a_{1m}x_m \geq A_1 \\
a_{21}x_1+a_{22}x_2+a_{23}x_3...+a_{2m}x_m \geq A_2 \\
...\\
a_{n1}x_1+a_{n2}x_2+a_{n3}x_3...+a_{nm}x_m \geq A_n \\
x_1,x_2,x_3  ... x_m \geq 0\\
\end{cases}$$

## mermaid.js


```mermaid
graph TD;
    A-->B;
    A-->C;
    B-->D;
    C-->D;
```


### [Footnotes](https://github.com/markdown-it/markdown-it-footnote)

脚注 1 link[^first].

脚注 2 link[^second].

Inline 脚注^[行内直接加脚注] definition.

Duplicated footnote reference[^second].

[^first]: 脚注 **遵守 markdown 格式**

    并且可以有多行。

[^second]: 脚注 2 的内容.
