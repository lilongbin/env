远程jupyter notebook Server服务器搭建

# 1. 安装 jupyter notebook
安装anaconda并配置环境变量
或者
```
python -m pip install notebook
```
然后即可启动一次,以确认是否安装成功
```
python -m notebook
```


# 2. 添加jupyter登录密码
```
python -m notebook password
```
注意有可能失败，如报错:` ~/.jupyter/jupyter_notebook_config.json ` 不存在
请手动创建.jupyter目录
```
mkdir ~/.jupyter
```

然后重试
```
python -m notebook password
```
再查看` ~/.jupyter/jupyter_notebook_config.json `可以看到密码的sha值

# 3. 修改启动参数
```
python -m notebook password --generate-config
vim ~/.jupyter/jupyter_notebook_config.py
```

在 jupyter_notebook_config.py 中找到下面的行，取消注释并修改。
```
c.NotebookApp.ip='*' #外部IP地址客户端可以访问
c.NotebookApp.notebook_dir = '/home/user/jupyter_nb' #本地notebook访问目录
#c.NotebookApp.password = u'sha:ce...刚才复制的那个密文' #可以注释掉让客户端手动输入认证
c.NotebookApp.open_browser = False #启动时不再默认从本地浏览器打开
c.NotebookApp.port = 8888 #可自行指定一个端口, 访问时使用该端口
```

以上设置完以后就可以在服务器上启动 jupyter notebook
```
python -m notebook
```

root 用户使用
```
python -m notebook password --allow-root
```

打开 IP:端口, 输入密码就可以访问了。
如
```
192.168.91.131:8888
```
> 其中192.168.91.131为安装配置的notebook的ip地址，8888为上面配置的端口

需要注意的是不能在隐藏目录(以.开头的目录)下启动 jupyter notebook, 否则无法正常访问文件。


