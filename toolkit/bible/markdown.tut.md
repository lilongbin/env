Markdown基本语法
Markdown是一种纯文本格式的标记语言。

# 1. 格式与段落
## 标题
语法
一个#开头是一级标题，二个#是二级标题，以此类推。支持六级标题。
注：在#与文本之间有个空格

示例
# 这是一级标题
## 这是二级标题
### 这是三级标题
#### 这是四级标题
##### 这是五级标题
###### 这是六级标题

## 字体
语法
- 加粗    要加粗的文字左右分别用两个*号包起来
- 斜体    要倾斜的文字左右分别用一个*号包起来
- 斜体加粗 要倾斜和加粗的文字左右分别用三个*号包起来
- 删除线   要加删除线的文字左右分别用两个~号包起来

示例
- **这是加粗的文字**
- *这是倾斜的文字*
- ***这是斜体加粗的文字***
- ~~这是加删除线的文字~~

## 分割线
语法
三个或者三个以上的 - 或者 * 都可以
示例
---
----
***

## 转义字符
Markdown中的转义字符为\ ,转义的有:
\ 反斜杠 ` 反引号 * 星号 _ 下划线 {} 大括号 [] 中括号 () 小括号  # 井号 + 加号 - 减号 . 英文句号 ! 感叹号
有两个字符需要特殊对待: < 和 & 如果你想将它们用作字面量, 必须将它们转义为字符实体

# 2. 列表
## 无序列表
语法
无序列表用 + - * 任何一种都可以

+ 列表内容
- 列表内容
* 列表内容
注意+ - * 跟内容之间都要有一个空格

## 有序列表
语法
数字加点

1. 列表内容
2. 列表内容
3. 列表内容

注意：序号跟内容之间要有空格

## 嵌套列表
上一级和下一级之间多缩进三个空格即可
* 列表内容
   * 嵌套列表内容
   * 嵌套列表内容
* 列表内容

## 可标记列表
- [ ] 任务一 未做任务 `- 空格 [ ]`
- [x] 任务二 已做任务 `- 空格 [x]`

# 3. 表格
语法
|表头|表头|表头|
|---|:--:|---:|
|内容|内容|内容|
|内容|内容|内容|

第二行分割表头和内容。- 有一个就行，为了对齐，可以多加几个
文字默认居左, -两边加:表示文字居中, -右边加:表示文字居右
注：原生的语法两边都要用 | 包起来

示例
|姓名|班级|特长
|--|:--:|--:|
|刘|1|跑步|
|王|2|游泳|
|张|3|编码|

# 4. 引用与链接
## 引用
语法
在引用的文字前加>即可

示例
>这是引用的内容
>>这是引用的内容
>>>>>>>>>>这是引用的内容

## 图片
语法
    ![图片alt](图片地址 "图片title")
图片alt就是显示在图片下面的文字，相当于对图片内容的解释。
图片title是图片的标题，当鼠标移到图片上时显示的内容,title可不加;

示例
![test fig](fig0.jpg "test")

## 超链接
语法
    [超链接名](超链接地址 "超链接title")
title可不加

示例
[百度](http://baidu.com)

如果想要在新页面中打开的话可以用html语言的a标签代替
<a href="超链接地址" target="_blank">超链接名</a>

# 5. 代码块
语法
单行代码：代码之间分别用一个反引号包起来
    `代码内容`
代码块：代码之间分别用三个反引号包起来，且两边的反引号单独占一行
```
代码...
    ...
```

示例
单行代码
`void onServiceStatusCheck(status_t status, cause_t cause);`
代码块
```
void onClientSubscribeStatusCheck(evgroup_t evgid, subscribe_status_t status)
{
    printf("%s evgid=%hu, status=%d\n", __func__, evgid, status);
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
condition(yes)->xxx
condition(no)->yyy

示例
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

示例
```sequence
Alice -> Bob: Authentication Request
Bob --> Alice: Authentication Response

Alice -> Bob: Another authentication Request
Alice <-- Bob: another authentication Response
```

