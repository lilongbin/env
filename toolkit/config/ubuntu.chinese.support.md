Ubuntu命令方式安装中文语言包

中文语言包:
+ language-pack-zh-hans 简体中文
+ language-pack-zh-hans-base
+ language-pack-zh-hant 繁体中文
+ language-pack-zh-hant-base

安装中文语言包
```
sudo apt-get install  language-pack-zh-han*
```

安装完整语言支持所缺失包
```
sudo apt install $(check-language-support)
```

