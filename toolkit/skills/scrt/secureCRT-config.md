# secureCRT配置setting及使用技巧总结

+ 选项 全局选项 常规 配置路径 直接输入Config
+ 选项 全局选项 终端 外观 高级 选中MonoChrome并编辑,常规/粗体背景色为250,250,230;去除启用闪烁;
+ 选项 全局选项 SSH主机密钥 主机密钥数据库存储位置: Config\ KnownHosts

+ 选项 全局选项 常规 默认会话 编辑默认设置
    * 会话选项 终端 仿真 选择linux并勾选ANSI颜色、使用配色方案,选择备用模拟键盘;尺寸为32*130;
    * 会话选项 终端 外观 选中MonoChrome并设置字体,字体编码utf8,使用Unicode线条绘制字体;光标样式block,闪烁;
    * 会话选项 终端 日志文件 日志文件名设置为 ..\SecureCRT_CN_log\scrt-%S-%Y%M%D.%h%m%s.log,勾选在连接上开始记录日志,追加到文件;
    * 自定义日志数据,连接时 ######### %S %Y%M%D.%h%m%s connected #########
    * 自定义日志数据,断开时 ######### %S %Y%M%D.%h%m%s disconnect #########
+ 确定,变更所有会话

+ 添加快捷操作按钮
    * [查看]-->[按钮栏];
    * [选项]-->[全局选项]-->[全局]-->[终端]-->[外观]-->[在栏位上显示按钮][top];
    * [选项]-->[全局选项]-->[全局]-->[终端]-->[标签]-->[选项]-->[双击动作]-->[关闭标签];

+ 添加按钮
    1. 右击新建按钮,发送字符串,添加需要发送的命令,然后设置标签并确定;
    2. 右击新建按钮,Run Script,选择需要运行的脚本文件,并设置标签如:newLog,确定;具体脚本语法可以参考帮助文档;

> 如python脚本要以以下两行作为开头
```
# $language = "Python"
# $interface = "1.0"
```
下面以重新开启log和打开log文件脚本为例进行演示:

1. 添加 newLog 按钮,需要关闭并开启 Session Log;
脚本内容如下
```
# $language = "Python"
# $interface = "1.0"

#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2019-09-10 20:53:46
####################################################

import os
import subprocess

def newSessionLogFile():
    config = crt.Session.Config
    # crt.Dialog.MessageBox("%s % config)
    try:
        crt.Session.Log(False) # stop
        crt.Session.Log(True, True)  # start
    except Exception as reason:
        crt.Dialog.MessageBox("Error: %s" % reason)

newSessionLogFile()
```

将如上内容保存到文件 scrt_newSessionLogFile.py
然后右击新建按钮,Run Script,选择 scrt_newSessionLogFile.py 脚本文件,并设置标签如:newLog,确定;

2. 添加 openLog 按钮,需要获取 LogFileName;
该脚本是调用 notepad++.exe 打开log,
因此还需要安装 notepad++ 并注意确认脚本中定义的 notepad++.exe 路径是否和实际安装的一致;
> pname = r"C:\Program Files\Notepad++\notepad++.exe";

脚本内容如下
```
# $language = "Python"
# $interface = "1.0"

#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2019-09-10 20:56:56
####################################################

import os
import subprocess

def openSessionLogFile():
    logging = crt.Session.Logging
    if not logging:
        crt.Dialog.MessageBox("Warn: the current session is not logging")
        return
    # crt.Dialog.MessageBox("Info: the current session is logging")

    logfile = crt.Session.LogFileName
    # crt.Dialog.MessageBox(logfile)

    peditorDflt = r"C:\Program Files\Notepad++\notepad++.exe"
    peditorBkup = r"notepad.exe"
    subcmd = "%s %s" % (peditorDflt, logfile)
    # param = logfile
    try:
        p = subprocess.Popen(subcmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
        # result = p.communicate(input=param)
    except Exception as reason:
        crt.Dialog.MessageBox("Error: %s(%s)" % (reason, subcmd))
        # return
        # try again with notepad.exe
        subcmd = "%s %s" % (peditorBkup, logfile)
        try:
            p = subprocess.Popen(subcmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
            # result = p.communicate(input=param)
        except Exception as reason:
            crt.Dialog.MessageBox("Error: %s(%s)" % (reason, subcmd))
    # res = result[0].decode()
    # print(res)

openSessionLogFile()
```

将如上内容保存到文件 scrt_openSessionLogFile.py
然后右击新建按钮,Run Script,选择 scrt_openSessionLogFile.py 脚本文件,并设置标签如:openLog,确定;

3. 添加 copyAll 按钮,拷贝当前对话框所有输出内容;

脚本内容如下
```
# $language = "Python"
# $interface = "1.0"

#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2019-09-11 10:53:16
####################################################

import os
import subprocess

def copyAllContents():
    logging = crt.Session.Logging
    if not logging:
        crt.Dialog.MessageBox("Warn: the current session is not logging")
        return
    # crt.Dialog.MessageBox("Info: the current session is logging")

    logfile = crt.Session.LogFileName
    # crt.Dialog.MessageBox(logfile)

    lines = []
    try:
        fd = open(logfile)
        for line in fd:
            lines.append(line)
        # crt.Dialog.MessageBox("%s" % lines)
        crt.Clipboard.Text = "".join(lines)
        crt.Dialog.MessageBox("copy completed.")
    except Exception as reason:
        crt.Dialog.MessageBox("Error: %s" % reason)

copyAllContents()
```

将如上内容保存到文件 scrt_copyAllContents.py
然后右击新建按钮,Run Script,选择 scrt_copyAllContents.py 脚本文件,并设置标签如:copyAll,确定;

