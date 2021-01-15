Markdown基本语法
Markdown是一种纯文本格式的标记语言。

# 1. 格式与段落
## 标题
语法
```
一个#开头是一级标题，二个#是二级标题，以此类推。支持六级标题。
```
注：在#与文本之间有个空格

标题示例
```
# 这是一级标题
## 这是二级标题
### 这是三级标题
#### 这是四级标题
##### 这是五级标题
###### 这是六级标题
```
# 这是一级标题
## 这是二级标题
### 这是三级标题
#### 这是四级标题
##### 这是五级标题
###### 这是六级标题
# Markdown在线编辑器 [MdEditor](http://www.mdeditor.com/)

## 字体
语法
```
**加粗**       要加粗的文字左右分别用两个*号包起来
*斜体*         要倾斜的文字左右分别用一个*号包起来
***斜体加粗*** 要倾斜和加粗的文字左右分别用三个*号包起来
~~删除线~~     要加删除线的文字左右分别用两个~号包起来
```

字体示例
```
- **这是加粗的文字**
- *这是倾斜的文字*
- ***这是斜体加粗的文字***
- ~~这是加删除线的文字~~
```
- **这是加粗的文字**
- *这是倾斜的文字*
- ***这是斜体加粗的文字***
- ~~这是加删除线的文字~~

## 分割线
语法
```
三个或者三个以上的 - 或者 * 都可以
```

分割线示例
```
---
----
***
```
---
----
***

## 转义字符
Markdown中的转义字符为\ ,转义的有:
```
\ 反斜杠 ` 反引号 * 星号 _ 下划线 {} 大括号 [] 中括号 () 小括号  # 井号 + 加号 - 减号 . 英文句号 ! 感叹号
```
有两个字符需要特殊对待:
```
< 和 &
```
如果你想将它们用作字面量, 必须将它们转义为字符实体

# 换行
    如果只想折行而不想分段,就在行尾加两个空格就可以了,这样两者仍是一段.
    在a b c 的c后面,如果a b c 与d之间不空行,或者只在c后面空一格,在这个时候,按下回车的话,编辑的第二行d,会与a b c在同一行里 要想实现换行,需要至少需要在c的后面空两个空格;
    但是由于markdown编辑器的不同,可能在一行字后面,直接换行回车,也能实现换行,但是在Visual Studio Code上,想要换行必须得在一行字后面空两格才行.

# 空行
    无论第一行与第二行在编辑的时候有多少个空行(只要这一行只有回车或者space没有其他的字符就算空行),在渲染之后,只隔着一行.

# 分段
    在markdown语言中,唯一决定两行文字是否是段落的,就在于这两行文字之间是否有空行.
    如果这两行文字之间,有空行了,就代表,这两行文字为两个段落,如果这两行文字之间,没有空行,仅仅换行,就代表这两行文字是属于同一个段落.
    即使是在一行文字中的末尾,添加了两个空格之后换行,这两个行文字依旧是一个段落.

# 2. 列表
## 无序列表
语法
```
无序列表用 + - * 开头,任何一种都可以
```

列表示例
```
+ 列表内容
- 列表内容
* 列表内容
```
+ 列表内容
- 列表内容
* 列表内容
> 注意+ - * 跟内容之间都要有一个空格

## 有序列表
语法
```
数字加点开头
```

有序列表示例
```
1. 列表内容
2. 列表内容
3. 列表内容
```
1. 列表内容
2. 列表内容
3. 列表内容
> 注意序号跟内容之间要有空格

## 嵌套列表
```
上一级和下一级之间多缩进三个空格即可
```
嵌套列表示例
```
* 列表内容
   * 嵌套列表内容
   * 嵌套列表内容
* 列表内容
```
* 列表内容
   * 嵌套列表内容
   * 嵌套列表内容
* 列表内容

## 可标记列表
语法
```
- 空格 [ ]
- 空格 [x]
```
可标记列表示例
```
- [ ] 任务一 未做任务
- [x] 任务二 已做任务
```
- [ ] 任务一 未做任务
- [x] 任务二 已做任务

# 3. 表格
语法
```
|表头|表头|表头|
|---|:--:|---:|
|内容|内容|内容|
|内容|内容|内容|
```

第二行分割表头和内容。- 有一个就行，为了对齐，可以多加几个
文字默认居左, -两边加:表示文字居中, -右边加:表示文字居右
注:原生的语法两边都要用 | 包起来

表格示例
```
|姓名|班级|特长
|--|:--:|--:|
|刘|1|跑步|
|王|2|游泳|
|张|3|编码|
```
|姓名|班级|特长
|--|:--:|--:|
|刘|1|跑步|
|王|2|游泳|
|张|3|编码|

# 4. 引用与链接
## 引用
语法
```
在引用的文字前加>开头即可
```

引用示例
```
>这是引用的内容
>这是引用的内容
>>这是引用的内容
>>>>>>这是引用的内容
>这是引用的内容
>
```
>这是引用的内容
>这是引用的内容
>>这是引用的内容
>>>>>>这是引用的内容
>这是引用的内容
>
引用完之后，必须再空一行，重新一个新的开始，否则，以后的文字都将在引用的范围内;

## 图片
语法
```
    ![图片alt](图片地址 "图片title")
```
>图片alt就是显示在图片下面的文字，相当于对图片内容的解释。
>图片title是图片的标题，当鼠标移到图片上时显示的内容,title可不加;

图片示例:
```
![test fig](fig0.jpg "test")
```
![test fig](fig0.jpg "test")

## 超链接
语法
```
    [超链接名](超链接地址 "超链接title")
```
> title可不加

超链接示例:
```
[百度链接](http://baidu.com "百度")
```
[百度链接](http://baidu.com "百度")

如果想要在新页面中打开的话可以用html语言的a标签代替
```
<a href="超链接地址" target="_blank">超链接名</a>
```

# 5. 代码块
语法
单行代码:代码之间分别用一个反引号包起来 `代码内容`
代码块:代码之间分别用三个反引号包起来，且两边的反引号单独占一行
```
代码...
    ...
```

代码块示例
单行代码
`void onServiceStatusCheck(status_t status, cause_t cause);`

代码块
```
void onClientSubscribeStatusCheck(int id, int status)
{
    printf("%s id=%d, status=%d\n", __func__, id, status);
}
```

# 6. 流程图
## 定义元素
语法
tag=>type: content:>url

tag就是元素名字，
type是这个元素的类型，有6种类型分别为
 * start # 开始
 * end # 结束
 * operation # 操作
 * subroutine # 子程序
 * condition # 条件
 * inputoutput # 输入或产出

content就是在方框中要写的内容，注意type后的冒号与文本之间一定要有个空格。
url是一个链接，与方框中的文本绑定

## 连接元素
语法
用->来连接两个元素;
注意condition类型，有yes和no两个分支，所以要写成
> condition(yes)->xxx
> condition(no)->yyy

流程图示例
```flow
st_start=>start: Start
op_Operation=>operation: Operation
cond_condition=>condition: Yes/No?
e_end=>end

st_start->op_Operation->cond_condition
cond_condition(yes)->e_end
cond_condition(no)->op_Operation
```

# 7. 时序图
语法
```sequence
title:sequence
```
标题
    title: 我是标题
对象
    participant A

注释
    Note left of A  代表注释在A的左边
    Note right of B 代表注释在B的右边

交互
    -  实线,表示主动发送消息
    -- 虚线,表示返回消息
    >  实心箭头,同步消息
    >> 空心箭头,异步消息

时序图示例
```sequence
title: Alice Bob sequence
Alice->Bob: Authentication call
Bob->Alice: Authentication return

Alice-->Bob: Another authentication Request
Bob-->Alice: another authentication Response
```

# End


# 参考
# Markdown在线编辑器 [MdEditor](http://www.mdeditor.com/)

**Markdown是一种轻量级的「标记语言」**

## MdEditor的功能列表演示

# 标题H1

## 标题H2

### 标题H3

#### 标题H4

##### 标题H5

###### 标题H5

### 字符效果和横线等
----

~~删除线~~ <s>删除线（开启识别HTML标签时）</s>

*斜体字*      _斜体字_

**粗体**  __粗体__

***粗斜体*** ___粗斜体___

上标：X<sub>2</sub>，下标：O<sup>2</sup>

### 引用 Blockquotes

> 引用文本 Blockquotes

引用的行内混合 Blockquotes

### 锚点与链接 Links
[普通链接](https://www.mdeditor.com/)
[普通链接带标题](https://www.mdeditor.com/ "普通链接带标题")
直接链接：<https://www.mdeditor.com>
[锚点链接][anchor-id]
[anchor-id]: https://www.mdeditor.com/
[mailto:test.test@gmail.com](mailto:test.test@gmail.com)

### 多语言代码高亮 Codes

#### 行内代码 Inline code

执行命令：`npm install marked`

### 图片 Images

图片加链接 (Image + Link)：

![图片Alt](图片路径 "图片名")

分割线
---
### 列表 Lists

#### 无序列表（减号）Unordered Lists (-)
- 列表一
- 列表二
- 列表三

#### 无序列表（星号）Unordered Lists (*)
* 列表一
* 列表二
* 列表三

#### 无序列表（加号和嵌套）Unordered Lists (+)
+ 列表一
+ 列表二
    + 列表二-1
    + 列表二-2
    + 列表二-3
+ 列表三
    * 列表一
    * 列表二
    * 列表三

#### 有序列表 Ordered Lists (-)
1. 第一行
2. 第二行
3. 第三行

#### GFM task list
- [x] GFM task list 1
- [x] GFM task list 2
- [ ] GFM task list 3
    - [ ] GFM task list 3-1
    - [ ] GFM task list 3-2
    - [ ] GFM task list 3-3
- [ ] GFM task list 4
    - [ ] GFM task list 4-1
    - [ ] GFM task list 4-2

----

### 绘制表格 Tables

| 项目        | 价格   |  数量  |
| --------   | -----:  | :----:  |
| 计算机      | $1600   |   5     |
| 手机        |   $12   |   12   |
| 管线        |    $1    |  234  |

| Item      | Value |
| --------- | -----:|
| Computer  | $1600 |
| Phone     |   $12 |
| Pipe      |    $1 |

----

#### 特殊符号 HTML Entities Codes

&copy; &  &uml; &trade; &iexcl; &pound;
&amp; &lt; &gt; &yen; &euro; &reg; &plusmn; &para; &sect; &brvbar; &macr; &laquo; &middot;

X&sup2; Y&sup3; &frac34; &frac14;  &times;  &divide;   &raquo;

18&ordm;C  &quot;  &apos;


#### 反斜杠 Escape

\*literal asterisks\*

### 科学公式 TeX(KaTeX)

$$E=mc^2$$

行内的公式$$E=mc^2$$行内的公式，行内的$$E=mc^2$$公式。

$$x > y$$

$$\sin(\alpha)^{\theta}=\sum_{i=0}^{n}(x^i + \cos(f))$$

多行公式：

```math
\displaystyle
\left( \sum\_{k=1}^n a\_k b\_k \right)^2
\leq
\left( \sum\_{k=1}^n a\_k^2 \right)
\left( \sum\_{k=1}^n b\_k^2 \right)
```
### 分页符 Page break

> Print Test: Ctrl + P

[========]

### 绘制序列图 Sequence Diagram

```seq
Andrew->China: Says Hello
Note right of China: China thinks\nabout it
China-->Andrew: How are you?
Andrew->>China: I am good thanks!
```

### End

