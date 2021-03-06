# Linux系统samba配置示例

+ jack用户的共享需要用户名和密码认证
+ vtmp用户的共享可以所有人访问,权限读写

配置用户名和密码后修改smb配置
```
vim /etc/samba/smb.conf
```

```
[jackshare]
comment = jack share folder #描述
path = /home/jack/jackshare #共享目录
writable = yes              #允许写权限
valid users = jack, @root   #允许访问的用户
write list  = jack, root    #允许写入的用户
browseable = yes            #该指定目录可浏览
create mask = 0644          #创建文件夹的权限mask
directory mask = 0755       #创建文件的权限mask

[vtmp]
comment = vtmp share folder
path = /home/vtmp
guest ok = yes
writable = yes
browseable = yes
create mask = 0666
directory mask = 0777
```

然后还需要将jack用户添加到smb账户库
```
smbpasswd -a jack

service smb restart
service smbd restart #ubuntu系统服务名为smbd
```

# linux系统挂载他人的smb共享目录
假设jack用户名和密码为jack/jkpass
命令
```
mount -t cifs -o username=jack,password=jkpass //10.171.59.99/jackshare /usr1/share
```

或者加入到 /etc/fstab 文件开机自动挂载
```
//10.171.59.99/jackshare /usr1/share  cifs  username=jack,password=jkpass  0 0
``` 

