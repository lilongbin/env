python-selenium与chromedriver安装配置与示例

# 安装selenium
selenium可以直接可以用pip安装。
```
python -m pip install selenium
```

# 安装chromedriver
首先查看Chrome版本，在浏览器中输入```chrome://version/```
下载chromedriver的版本一定要与Chrome的版本一致。
有两个下载地址：
```
1、http://chromedriver.storage.googleapis.com/index.html
2、https://npm.taobao.org/mirrors/chromedriver/
```

# 配置
解压压缩包，找到chromedriver.exe复制到chrome的安装目录（其实也可以随便放一个文件夹）。
1. 复制chromedriver.exe文件的路径并加入到电脑的环境变量中去,右击我的电脑，属性，高级设置，环境变量，
2. 进入环境变量编辑界面，添加到用户变量即可，双击PATH，
将文件位置C:\Program Files (x86)\Google\Chrome\Application\添加到后面，与前面的用分号分隔。

> 不配置环境变量也可以,需要在脚本中指定
```
chrome_driver = r'C:\Program Files (x86)\Google\Chrome\Application\chromedriver.exe'
browser = webdriver.Chrome(executable_path = chrome_driver)
```

# 验证是否安装成功
完成后在cmd下输入chromedriver验证是否安装成功.

测试
```
from selenium import webdriver
import time

def main():
    #chrome_driver = 'C:\Program Files (x86)\Google\Chrome\Application\chromedriver.exe'  #chromedriver的文件位置
    #browser = webdriver.Chrome(executable_path = chrome_driver)
	browser = webdriver.Chrome()
    browser.get('https://www.baidu.com')
    time.sleep(5)
    browser.quit()
'''
open url with chrome and right-click an element then select "Inspect"
go to "Elements" window and right-click the selected source code
then select "Copy", "Copy selector" or "Copy XPath" or "Copy full XPath"
'''
if __name__ == '__main__':
    main()
```

# selenium元素定位技巧
chrome打开页面右击要定位的元素,选择Inspect(检视),会自动打开并定位到Elements(元素)页面源代码,
右击元素源代码可以选择复制，复制css选择器或XPath或full XPath等;

另外，如有页面跳转，请注意增加一定的延时或智能等待;

# selenium元素定位方法分析
以某度为例
输入框
```
<input id="kw" name="wd" class="s_ipt" value="" maxlength="255" autocomplete="off">
```
搜索按钮
```
<span class="bg s_btn_wr"><input type="submit" id="su" value="百度一下" class="bg s_btn btn_h btnhover"></span>
```

## find_element_by_xpath("xpath")
准确定位的首选方式
> 可以直接在chrome中复制XPath,如 //*[@id="kw"]
find_element_by_xpath(r"//*[@id="kw"]")

> 一般不需要使用full XPath如 /html/body/div[1]/div[1]/div[5]/div/div/form/span[1]/input

## find_element_by_css_selector
### 单属性查找
#### 1.用 标签名 定位查找
driver.find_element_by_css_selector("input")
#### 2.用 id 属性定位查找 
driver.find_element_by_css_selector("kw")
#### 3.用 class 属性定位查找
driver.find_element_by_css_selector("s_ipt")
#### 4.其他属性定位
driver.find_element_by_css_selector("[name="wd"]")

### 组合属性查找
#### 1. 标签名及id属性值组合定位
driver.find_element_by_css_selector("input#kw")
#### 2.  标签名及class属性值组合定位
driver.find_element_by_css_selector("input.s_ipt")
#### 3. 标签名及属性（含属性值）组合定位
driver.find_element_by_css_selector("input[name="wd"]")
#### 4. 标签及属性名组合定位
driver.find_element_by_css_selector("input[name]")
#### 5. 多个属性组合定位
driver.find_element_by_css_selector("[class="s_ipt"][name="wd"]")

### 模糊匹配
#### 1. class拥有多个属性值，只匹配其中一个时
driver.find_element_by_css_selector("input[class ~= "bg"]")
#### 2. 匹配以字符串开头的属性值
driver.find_element_by_css_selector("input[class ^= "bg"]")
#### 3. 匹配以字符串结尾的属性值
driver.find_element_by_css_selector("input[class $= "s_btn"]")
#### 4. 匹配被下划线分隔的属性值
driver.find_element_by_css_selector("input[class |= "s"]")

### 层级匹配
#### 1.直接子元素层级关系，input为span的直接子元素(用 > 表示)
driver.find_element_by_css_selector(".bg.s_btn_wr > input") #class为bg和s_btn_wr 的span标签的子元素input

#### 2.只要元素包含在父元素里面，不一定是直接子元素，用空格隔开，如图一所示，form 下面的 span 里面的input
driver.find_element_by_css_selector("#form input")  #id是form的form标签里面的input标签

#### 3.多级关系
driver.find_element_by_css_selector("#form > span > input") #id是form的form标签下面的span标签的下面的input标签

#### 其他
+ p:nth-child(1)             # 选择第一个p标签，还可写为
+ p:first-child
+ p:nth-last-child(1)        # 选择倒数第一个p标签（要保证最后一个标签是p）
+ p:only-child       		   # 唯一的p标签

## find_element_by_id("id_vaule")
直接填入元素的id即可
driver.find_element_by_id("kw")

## find_element_by_name("name_vaule")
填入元素的name值
driver.find_element_by_name("wd")

## find_element_by_link_text("text_vaule")
只对被a标签包围<a></a>的标签有效
```
<a class="c-color-gray2" href="http://ir.baidu.com" target="_blank">About Baidu</a>
```
find_element_by_link_text("About Baidu")

