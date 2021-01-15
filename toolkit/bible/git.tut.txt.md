Git version control

* Author: longbin
* Created Date: 2015-08-08
* Last Modified: 2021-01-15

# 常用git命令
+ `git init`           #创建一个数据库;  
+ `git clone`          #复制一个数据库到指定文件夹  
+ `git add`            #把想提交的文件add上
+ `git commit`         #commit这些文件到本地数据库;  
+ `git pull`           #从服务器下载数据库,并跟自己的数据库合并;
+ `git fetch`          #从服务器下载数据库,并放到新分支,不跟自己的数据库合并;  
+ `git whatchanged`    #查看两个分支的变化;  
+ `git branch`         #创建分支,查看分支,删除分支  
+ `git checkout`       #切换分支  
+ `git merge`          #合并分支,把目标分支合并到当前分支  
+ `git config`         #配置相关信息,例如email和name  
+ `git log`            #查看版本历史  
+ `git show`           #查看版本号对应版本的历史;如果参数是HEAD查看最新版本;  
+ `git tag`            #标定/删除标签/版本号;  
+ `git reset`          #恢复到之前的版本 
> + ----mixed是git-reset的默认选项,它的作用是重置索引内容,将其定位到指定的项目版本,而不改变工作树中的所有内容,只是提示有哪些文件还未更新; 
> + --soft选项既不触动索引的位置,也不改变工作树中的任何内容;该选项会保留在工作树中的所有更新并使之处于待提交状态;相当于在--mixed基础上加上git add .;
> + --hard把整个目录还原到一个版本,包括所有文件;  
+ `git clean`          #删除所有没有tracked过的文件
+ `git push`           #向其他数据库推送自己的数据库;  
+ `git status`         #显示当前的状态;  
+ `git mv`             #重命名文件或者文件夹;  
+ `git rm`             #删除文件或者文件夹;  
+ `git help`           #查看帮助,还有几个无关紧要的命令,请自己查看帮助

为方便使用git命令,增加git命令的自动补全功能,可以将git source tree目录的 `/contrib/completion/git-completion.bash` 放于`${HOME}`目录在`.bashrc`中加载;

# 1 git Constructure
## git系统3个对象
+ 提交(commit), 
+ 树(tree),
+ 原文(blob);
> 每一个对象都是以文件的方式保存,文件的名称为该对象的sha-1标识;
> + 每一个commit中包含了所修改的文件列表,用树(tree)表示,同时包含了作者(author),提交者(committer),备注(comment)等信息;
> + tree是一个对象地址引用,指向了包含该tree信息的对象;tree中列出本次提交具体包含的文件列表,包括源文件名称,以及该文件的对象地址引用;每个文件类标对应一个blob对象;
> + blob是具体的原文,比如对C程序文件而言就是源码本身;
> + 每次用户提交时,git系统就生成一个快照(snapshot);

git仓库存放在当前工作目录的`.git`文件夹下,主要子目录包括以下几项
+ `.git/objects` 所有的对象,包括commit,tree,blob;
+ `.git/refs/heads` 所有的分支,分支仅仅是一个地址引用,指向最后一次提交记录;
+ `.git/refs/tags` 所有的标签(tag)可以为某次提交打上标签,方便以后查看;
+ `.git/logs/HEAD` HEAD的变化历史
+ `.git/logs/refs/heads` 除master分支以外其他分支的HEAD变化历史;
> 每次clone仓库的时候实际拷贝的就是这个目录里面的数据;

## git系统中的4个区域
+ workspace工作区     用户看到的文件夹下的内容;
+ index/stage暂存区   保存即将提交到repository的内容;
+ repository版本库    安全存放数据的位置,其中HEAD指向最新放入仓库的版本;
+ remote远程仓库      托管代码的服务器;

```
+-------------------------------------+          pull
|              workspace              |<=======================+
+-------------------------------------+                        |
        |                    ^                                 |
        |add                 |check out                        |
        V                    |                                 |
+-------------+        +--------------+   clone/fetch  +==============+
|             |        |              |<===============|              |
| index/stage |        |  repository  |                |   remote     |
|             |------->|              |===============>|              |
+-------------+ commit +--------------+     push       +==============+
```

open below sequence with [plantuml](http://www.plantuml.com/plantuml)
```
@startuml
title: git transfer sequence
box workspace area.
boundary untracked
boundary unmodified
boundary modified
end box

box index/stage
boundary staged
end box

box repository
boundary committed
end box

box remote server
boundary remote
endbox

untracked->untracked:new <file>
untracked->staged:git add <file>
staged->untracked:git rm -- cached <file>
untracked->untracked:new <file>
untracked->staged:git add <file>
staged->committed:git commit
committed->modified:edit <file>

remote->unmodified:git clone
unmodified->modified:edit <file>
modified->unmodified:git checkout -- <file>
unmodified->modified:edit <file>

modified->staged:git add <file>
staged->modified:git reset HEAD <file>

modified->staged:git add <file>
staged->committed:git commit
committed->remote:git push
@enduml
```

### 1. 工作区(working area/working tree);
git的工作目录保存当前正在工作的文件,和working tree是相同的意思;
在这个目录中的文件可能会在切换branch时被git删除或替换;
这个目录是临时目录,临时存储从git库中取出的文件,这些文件一直会被保存,直到下次提交;

工作目录下面的所有文件都不外乎这两种状态:已跟踪或未跟踪;
已跟踪的文件是指本来就被纳入版本控制管理的文件,在上次快照中有它们的记录,工作一段时间后,它们的状态可能是未更新,已修改或者已放入暂存区;
在编辑过某些文件之后,git将这些文件标为已修改;逐步把这些修改过的文件放到暂存区,直到提交所有这些暂存起来的文件,如此重复;

HEAD是当前活跃分支的游标,通俗的讲就是你现在在哪,HEAD就指向哪;
HEAD并不是只指向分支的顶端,实际上它可以指向任何一个节点,它就是git内部用来追踪当前位置的指针;
ORIG_HEAD指向HEAD的上一个位置;

git本地文件的状态
+ untracked
+ unmodified
+ modified
+ staged

哪些操作会改变工作区中的内容?
+ `git add` 命令会把工作区更新的内容放到stage area;
+ `git checkout -- <file>` 可以恢复已修改的文件,用库中的版本替换;
+ `git checkout branch_name` 可以从仓库中检出对应分支的内容到工作区;
+ `git rm --cached <file>` 可以将新add到暂存区的文件从暂存区移除并恢复为未追踪状态;

### 2. 暂存区(stage area/git index);
暂存区是工作区和git仓库之间的区域,和staging area是相同的意思;
git index和git staging area是同一个意思,都指已经被add但尚未commit的那些内容所在的区域;

查看目前暂存区中内容的方法是使用git status命令;
+ "Changes not staged for commit"或"Changed but not updated"所列内容在工作区,add之后将进入暂存区;
+ "Changes to be committed"所列内容在暂存区,commit后进入git仓库;
+ "Untracked files"所列内容是尚未被git跟踪的内容,add之后进入暂存区;

哪些操作会改变暂存区中的内容?
+ `git add <path>...`        会将工作区中`<path>`的内容加入暂存区;
+ `git reset HEAD <path>...` 会将暂存区中path内容删除,重新放回工作区;
+ `git commit` 命令会把stage area的内容推送到本地仓库,并清空stage area;
+ `git commit -a` 可以提交所有修改及删除的文件,但不包含新增文件;
+ `git rm --cached <file>` 可以将新add到暂存区的文件从暂存区移除并恢复为未追踪状态;
> stage area的内容由git add添加,每次git commit后又会清空;

### 3. 版本库(git directory);
项目的所有历史提交都被保存在了git仓库中,只要不做回滚操作,就不会丢失;

### 4. 远程仓库
托管代码的服务器,可以简单的认为是项目组的一台用于远程数据交换电脑;

## GIT与SVN的几个区别
1. GIT是分布式的，SVN不是，这是最核心的区别。
GIT跟SVN一样有自己的集中式版本库或服务器。但GIT更常用于分布式模式，也就是每个开发人员从中心版本库/服务器上chect out代码时都会在自己的机器上克隆一个自己的版本库。
可以这样说，即使在一个不能连接网络的地方，仍然能够提交文件（只提交到本地不能推送到服务器），查看历史版本记录，创建项目分支等，而SVN则需要连接中心服务器才可做这些事情。

2. GIT把内容按元数据方式存储，而SVN是按文件。
所有的资源控制系统都是把文件的元信息隐藏在一个类似.svn,.cvs等的文件夹里。.git目录保存了一个克隆版的版本库，它拥有中心版本库上所有的东西，例如标签，分支，版本记录等。

3. GIT分支和SVN的分支不同。
分支在SVN中一点不特别，就是版本库中的另外的一个目录。如果想知道是否合并了一个分支，需要手动运行像`svn propget svn:mergeinfo`这样的命令来确认代码是否被合并。所以，经常会发生有些分支被遗漏的情况。
然而，处理GIT的分支却是相当的简单和有趣。可以从同一个工作目录下快速的在几个分支间切换。很容易发现未被合并的分支，能简单而快捷的合并这些文件。

4. GIT没有一个全局的版本号，而SVN有。
目前为止这是GIT比SVN相比缺少的最大的一个特征。SVN的版本号实际是每个源代码提交相应时间的快照。
我们可以使用GIT的SHA-1来唯一的标识一个代码快照。这个并不能完全代替SVN里容易阅读的全局数字版本号。

5. GIT的内容完整性要优于SVN。
GIT的内容存储使用的是SHA-1哈希算法，能确保代码内容的完整性，确保在遇到磁盘故障或网络问题时降低对版本库的破坏。

# 2 Install git and gitk
ubuntu
```
sudo apt-get install git-core gitk
```

CentOS/fedora
```
sudo yum install git-core gitk
```
> 其中gitk可以方便的查看提交的历史;

# 3 git init 创建仓库
创建仓库分两种方式,一种是对本地的某个文件夹进行git管理;另一种是从某个远程仓库中clone;

## 第1种方式
```
cd mydir
git init #初始化git仓库
git add * ##添加跟踪的文件,并把目标文件快照放入暂存区域;
git status
git commit -a -m "first created"
```
    git init初始化后,在当前目录下会出现一个名为.git的目录,所有git需要的数据和资源都存放在这个目录中;
    git add把目标文件快照放入暂存区,也就是add file into stage area,同时未曾跟踪过的文件标记为需要跟踪; 运行了git add之后又作了修订的文件,需要重新运行git add把最新版本重新暂存起来;
    要确定哪些文件当前处于什么状态,可以用git status命令列出修改过的文件,如果要查看具体修改了什么地方,可以用git diff命令;
    git commit命令会启动文本编辑器以便输入本次提交的备注;默认的文本编辑器是shell环境变量$EDITOR所指定的软件如vim/emacs等;编辑文本中以#开头的行是注释,退出编辑器时,将会丢掉注释行,将备注和本次更新提交到仓库;
    git commit的-a选项将自动暂存并提交所有修改过的文件及删除的文件,但不包括新添加的文件;-m选项即memo,意思是添加一个注释; 如果不使用-a选项的话,commit命令提交的是已经被添加到stage area区域的修改;
    提交是将暂存区的快照放入仓库,任何还未暂存的修改仍然保持已修改状态,可以在下次提交时纳入版本管理;
    有时候我们提交完了才发现漏掉了几个文件没有加,或者提交信息写错了;想要撤消刚才的提交操作,可以使用--amend选项重新编辑提交备注并提交,但将要提交的文件快照和之前的一样;
    git commit -m 'first commit'
    git add forgotten_file
    git commit --amend #修改最后一次提交记录
    上面的三条命令最终只产生一个提交,第二个提交命令修正了第一个的提交内容;

## 第2种方式clone远程仓库
```
git clone remote-address [local_dir]
```
该命令从`remote-address`地址中clone一份远程仓库到本地local_dir目录下,同时把仓库中的当前分支对应的内容checkout出来放到工作区;
git支持许多数据传输协议;除了`git://` 协议,也可以用`http(s)://` 或`user@server:/path.git`表示的SSH传输协议;

# 4 git diff
    git diff可以比较:
        工作区与暂存区之间;
        暂存区和git仓库之间;
        工作区和git仓库之间;
        git仓库中不同commit之间的差异;
    git diff [<path>...]    #查看工作区和暂存区的差异;
    git diff --cached [<path>...]   #查看暂存区和git仓库的差异;
    git diff --cached [<commit>] [<path>...]    #查看暂存区和git仓库中指定commit的差异;
    git diff <commit> [<path>...]   #查看工作目录和指定<commit>之间的差别;
        如果要和git仓库中最新版比较差别,则<commit>=HEAD;
        如果要和某一个branch比较差别,<commit>=分支名字;
    git diff <commit> <commit> [<path>...]  #查看git仓库中任意两个<commit>之间的差别;如果想比较任意一个<commit>和最新版的差别,把其中一个<commit>换成HEAD即可;
        -M选项检查重命名;
        -C选项检查复制和重命名;
        -w, --ignore-all-space选项可以忽略所有空白;
        --ignore-space-at-eol忽略行尾的空白;
        -b, --ignore-space-change忽略空白字符的变化;

    若要查看尚未暂存的文件更新了哪些部分,直接运行git diff命令;
    若要查看已经暂存的文件和上次提交时的快照之间的差异,可以用git diff --cached命令;git 1.6.1 及更新版本还允许使用git diff --staged命令,效果是相同的;
    git diff命令还可以比较两个分支的差异
    git diff dev1 dev2 ##比较两个分支的差异;
    git diff master...dev1 ##查看dev1与两个分支共同父节点之间的差异;
    git diff master..dev1 ##查看master分支有而dev1分支没有的内容;
    git merge-base dev1 dev2 ##可以查看二者的共同父节点;
    git diff another_branch ##可以查看与另一个分支的差异;
    git diff --stat ##仅仅比较统计信息;
    git diff的输出是典型的patch,可以直接把这个输出重定向到一个Patch文件;然后到应用它的分支并放入该Patch文档,执行git apply <patch>命令可以直接应用该patch文件;

# 5 git log
    git log命令可以查看提交历史;
    git log会按提交时间列出所有的更新,最近的更新排在最上面;每次更新都有一个SHA-1校验和,提交者的名字和电子邮件地址,提交时间,最后一个段落显示提交备注;
    git log <file>     #查看file的每次提交记录
    --pretty选项可以指定使用完全不同于默认格式的方式展示提交历史;
        比如用oneline将每个提交放在一行显示,在提交数很大时非常有用,另外还有short,full和fuller可以用,展示的信息或多或少有些不同;
        format可以定制要显示的记录格式,这样的输出便于后期编程提取分析;
    git log --pretty=oneline
    git log --pretty=format:"%cn, %ce: %s"
    git log --graph ##图形化显示分支的分化衍生情况;
    限制长度输出
        -(n)    仅显示最近的n条提交
        --since --after 仅显示指定时间之后的提交;
        --until --before 仅显示指定时间之前的提交;
        --author 仅显示指定作者相关的提交;
        --committer 仅显示指定提交者相关的提交;
    查看Git仓库中,2008年10月期间,gitster提交的记录
    git log --pretty="%s" --committer=gitster --since="2008-10-01" --before="2008-11-01"
    git log --oneline --graph --decorate ##显示整个project的history line;
    git log <file>     #查看file的每次提交记录

    -p选项可以列出本次提交前后具体的差异;
    git log -p ##可以列出具体差异
    git log -p <file>  #查看file的每次详细修改内容的diff;
    git log -p -1      #查看最近1次的详细修改内容的diff;
    git log -p -2      #查看最近2次的详细修改内容的diff;
    git log --stat     #查看提交统计信息;
    git log --autor=linus #查看指定作者的记录;
    git log --since="2015-08-01 00:00:00" --before="2015-08-20 24:00:00"
    -name-only #查看指定时间段内的提交记录;

一个很酷炫的显示
```
git log --graph --pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr) %C(bold blue)<%an>%Creset' --abbrev-commit --date=relative
```

# 6 git reset
    reset用于将当前分支的头(HEAD)复位到之前任意的某个提交点,即忽略或者删除最近的某些提交;reset常用的选项有两个--soft和--hard;
    --soft选项告知系统,仅仅复位提交区,而工作区和暂存区的内容保持不变;
    soft的语义是把过去的某些提交合并为一个提交,所以一般在软复位后,还需要调用commit命令再提交依次,因为工作区和暂存区的内容依然保持了最后的状态;
    --hard选项和--soft选项的区别在于,除了复位提交区,同时复位工作区和暂存区;

    加入发现刚才的commit是错误的,可以用下面这条撤销本次commit并清空所有最近的修改;
    git reset --hard HEAD^ ##恢复最近的一次操作;

    如果发现刚才的git reset --hard是误操作,可以使用以下步骤再恢复过来;
    使用git reflog可以解决这个问题,简单的说他会记录所有的HEAD历史,也就是所当执行commit,checkout,reset等操作的时候,这些操作会被记录在reflog中;
    执行git reflog可以查看哪次是我们需要回去的状态比如上次reset前的状态SHA-a的前7位是a378003,可以执行
    git reset --hard a378003
    就可以恢复到上次reset之前的状态;
    如果因为reset等操作丢失一个提交的时候,可以用上面这个方法找回来;除非操作已经被git当作垃圾清理掉了,一般是30天以后;

6.1 撤销git add
    有两个修改过的文件,我们想要分开提交,但不小心用git add . 全加到了暂存区域;
    该如何撤消暂存其中的一个文件;
    git status 的命令输出已经告诉了我们该怎么做git reset HEAD ...;如
```
(use "git reset HEAD <file>..." to unstage)
```
    git reset HEAD test2.txt
    test2.txt文件又回到了之前已修改未暂存的状态;
    等价于git reset -- test2.txt
    等价于git reset test2.txt #从暂存区恢复到工作区;

6.2 撤销工作区的修改
    如果又觉得刚才对test2.txt的修改完全没有必要,该如何取消修改,回到之前的状态(也就是修改之前的版本)呢? git status同样提示了具体的撤消方法;
```
(use "git add <file>..." to update what will be committed)
(use "git checkout -- <file>..." to discard changes in working directory)
```
    git checkout -- test2.txt
    这条命令有些危险,文件会回到修改前的状态,丢失该文件所有最新的修改;所以在用这条命令前,请务必确定真的不再需要保留刚才的修改;
    如果只是想回退版本,同时保留刚才的修改以便将来继续工作,可以用git stash和分支来处理,应该会更好些;

6.3 删除所有没有tracked过的文件
    git clean经常和git reset --hard一起结合使用;
    git reset只影响被track过的文件,所以需要clean来删除没有track过的文件;
    结合使用这两个命令能让你的工作目录完全回到一个指定的`<commit>`的状态;
    git clean -n #是一次clean的演习,提示哪些文件会被删除.不会真正的删除文件,只是一个提醒;
    git clean -f #删除当前目录下所有没有track过的文件.不会删除.gitignore文件指定的文件夹和文件,不管这些文件有没有被track过;
    git clean -f `<path>` #删除指定路径下的没有被track过的文件;
    git clean -df #删除当前目录下没有被track过的文件和文件夹;
    git clean -xf #删除当前目录下所有没有track过的文件,不管他是否是.gitignore文件里面指定的文件夹和文件;
    git clean -nxdf #建议在用git clean前,先加上-n选项,先看看会删掉哪些文件,防止重要文件被误删;
    git reset --hard;git clean -df #结合使用能让工作目录完全回退到最近一次commit的状态;

# 7 git remote
    要查看当前配置有哪些远程仓库,可以用git remote命令,它会列出每个远程库的简短名字,如果有多个远程仓库,此命令将全部列出;在克隆完某个项目后,至少可以看到一个名为origin的远程库,git默认使用这个名字来标识所克隆的原始仓库;
    git remote -v ##-v选项显示对应的克隆地址
    要添加一个新的远程仓库,可以指定一个简单的别名,以便将来引用,运行
    git remote add <shortname> <url>
    如git remote add pb git://github.com/paulboone/ticgit.git
    现在可以用字串pb指代对应的仓库地址了;比如说,要抓取所有pb有但本地仓库没有的信息,可以运行git fetch pb
    pb的主干分支(master)已经完全可以在本地访问了,对应的名字是pb/master,可以将它合并到自己的某个分支,或者切换到这个分支;
    git remote show [remote-name] ##查看远程仓库[remote-name]中的分支;
    git remote rename pb paul命令修改远程仓库在本地的简短名称;
    对远程仓库的重命名,也会使对应的分支名称发生变化,原来的pb/master分支现在成了paul/master;
    碰到远端仓库服务器迁移,或者原来的克隆镜像不再使用,又或者某个参与者不再贡献代码,那么需要移除对应的远端仓库,可以运行git remote rm remote-name命令;
    假设remote仓库的别名是origin,现在需要更换远端服务器的地址;
    git remote set-url <name> <newurl>
    git remote set-url origin <remote-git-address>

# 8 git push
    项目进行到一个阶段,要同别人分享目前的成果,可以将本地仓库中的数据推送到远程仓库;实现这个任务的命令很简单
    git push [remote-name] [local-branch]:[remote-branch]
    如:
    git push origin master:refs/for/master
    即是将本地的master分支推送到远程主机origin上的对应master分支, origin 是远程主机名;
    refs/for 的意义在于我们提交代码到服务器之后是需要经过code review 之后才能进行merge的，而refs/heads 不需要;
    如果省略了:[remote-branch]将会推送到远端仓库的与[local-branch]同名的分支;
    如果要把本地的master分支推送到origin服务器上的master分支(克隆操作会自动使用默认的origin/master),可以运行下面的命令;
    git push origin master:master #或者直接省略:master
    git push origin master

    如果当前分支与远程分支存在追踪关系,则本地分支和远程分支都可以省略,将当前分支推送到origin主机的对应分支:
    git push origin

    如果当前分支只有一个远程分支,那么主机名可以省略:(可以使用git branch -r 查看远程的分支名)
    git push

    只可以push到所克隆的服务器上,或者同一时刻没有其他人在推数据,这条命令才会如期完成任务;
    如果在推数据前,已经有其他人推送了若干更新,推送操作就会被驳回;必须先把他们的更新抓取到本地,合并到自己的项目中,然后才可以再次推送;
    如果不再需要某个远程分支了,比如搞定了某个特性并把它合并进了远程的master分支(或任何其他存放稳定代码的地方),可以用这个非常无厘头的语法来删除它:
    git push [remote-name] :[branch-name]
    如果想在服务器上删除test分支,运行下面的命令;
    git push origin :test
    注意git push的用法git push [remote-name] [本地分支]:[远程分支] ,如果省略 [本地分支],那就等于是在说"在这里提取空白然后把它变成[远程分支]";
    相当于git push origin --delete test

    不带任何参数的git push默认只推送当前分支,这叫simple方式;
    还有一种matching方式,会推送所有有对应的远程分支的本地分支;git 2.0之前默认采用matching,现在默认采用simple方式;如果要修改这个设置,可以执行git config命令;
    git config --global push.default matching
    或者
    git config --global push.default simple
    还有一种情况,不管远程是否存在对应的分支,将本地的所有分支都推送到远程仓库,这是需要使用--all选项;
    git push origin --all
    注意git push默认不会推送标签;
    推送标签的命令格式如同推送分支,运行git push origin [tagname] 即可;
    git push origin v1.5
    如果要一次推送所有本地新增的标签上去,可以使用--tags选项;
    git push origin --tags

    如果在git push前没有获取仓库的最新内容,如下所示状态;
                  X---Y---Z  [master]
                 /
        o---o---o---A---B  [master on origin]
                ^
                [origin/master]
    在本地实际上看到的是
        o---o---o---X---Y---Z  [master]
                ^
                [origin/master]
    只执行git push origin master时,实际上是在试图将远程仓库变成如下状态;
                  A---B  (unnamed branch)
                 /
        o---o---o---X---Y---Z  master
    结果将会推送失败,并返回"Non fast-forward update"非快进式更新;
    如果在执行时使用-f选项强制推送,别且服务器允许的情况下,将会把本地master内容推送到远程仓库,那么别人的提交如A和B将丢失,除非真的有必要这样做;
    因此在推送时,要采用直进式"fast-forwards"更新,即首先获取远程的更新,在本地合并后再提交,这样就可以保证记录的完整合延续性;
    在git push之前首先要git pull --rebase改变基点;相当于git fetch然后git rebase;

    推送本地分支 local_branch 到远程分支 remote_branch 并建立关联关系
    a.远程已有remote_branch分支并且已经关联本地分支local_branch且本地已经切换到 local_branch 分支；
    # git push
    
    b.远程已有 remote_branch 分支但未关联本地分支 local_branch 且本地已经切换到 local_branch 分支；
    # git push -u origin/remote_branch
    
    c.远程没有 remote_branch 分支，本地已经切换到 local_branch 分支
    # git push origin local_branch:remote_branch


# 9 git fetch
    从远程仓库抓取数据到本地,保存一份远程仓库数据的拷贝到本地的[远程仓库别名]/[远程分支];
    git fetch [remote-name]
    此命令会到远程仓库中拉取所有本地仓库中还没有的数据;运行完成后,就可以在本地访问该远程仓库中的所有分支,如将其中某个分支合并到本地或取出某个分支;
    如果是克隆了一个仓库,此命令会自动将远程仓库归于origin名下;
    fetch命令只是将远端的数据拉到本地保存一份拷贝到[远程仓库别名]/[远程分支],并不自动合并到当前工作分支,只能手工合并;pull命令除了拥有fetch的功能外,还会把新取出来的内容merge到本地跟踪(tracking)分支;
    origin/master中origin为远程仓库的别名,master为远程仓库的分支名称,本地以remote-name/branch的格式引用远程分支;
    master为一个本地分支,也叫origin/master分支的跟踪(tracking)分支,即所有的修改首先在master分支中进行,修改完成后,调用remote相关命令就可以把master分支中的内容更新到origin/master分支上;
    git checkout master
    git fetch origin
    在执行fetch后,仅仅改变了本地仓库分支origin/master的内容,而本地跟踪分支的内容没有改变;
    如果要把该内容合并到当前分支,可以运行git merge origin/master;
    如果想要一份自己的dev来开发,可以在远程分支的基础上分化出一个新的分支来;
    git checkout -b dev origin/master

    从远程的拷贝分支([远程仓库别名]/[远程分支])checkout出来到本地的分支,称为跟踪分支(tracking branch); 跟踪分支是一种和远程分支有直接联系的本地分支;
    在跟踪分支里执行git push, 会将跟踪分支的内容同步上传到远程仓库; git会自行推断应该向哪个服务器的哪个分支推送数据;
    反过来,在这些分支里执行git pull会从远程仓库抓取数据到本地,保存到本地的[远程仓库别名]/[远程分支],并把它们的数据都合并到本地对应分支中来;

# 10 git checkout
    git checkout <branch>
    从仓库中提取出指定的分支或路径到当前工作区;
    git checkout master
    可以从仓库中提取出master分支或者将当前工作区切换到master分支的工作区;
    git checkout -b|-B <new_branch> [<start point>]
    git checkout -b dev master
    -b选项可以创建新的分支,并从对应的分支中提取到新建的分支中;
    git checkout -- <file> ##丢弃工作区file的修改
    git checkout <SHA-a>  #把某次历史提交记录checkout出来,但无分支信息;
    git checkout <new-branch> <SHA-1> #从某次提交记录创建分支并checkout;

# 11 git tag
    可以对某一时间点上的版本打上标签;在发布某个软件版本(比如v1.0等等)的时候,经常这么做;
    列出现有标签的命令非常简单,直接运行git tag;
    创建一个含附注类型的标签用-a (annotated的首字母)指定标签名字即可;
    git tag -a v1.4 -m 'my version 1.4'
    -m选项则指定了对应的标签说明,git会将此说明一同保存在标签对象中;
    git show命令查看相应标签的版本信息,并连同显示打标签时的提交对象;
    git show #显示某次提交的内容;
    git show v1.4
    git tag -d v1.4 ##删除一个已经存在的标签
    如果有自己的私钥,还可以用GPG来签署标签,只需要把之前的-a改为-s (signed的首字母)即可;
    git tag -s v1.5 -m 'my signed 1.5 tag'
    可以使用git tag -v [tag-name] (verify首字母)的方式验证已经签署的标签;此命令会调用GPG来验证签名,所以需要有签署者的公钥,存放在keyring中,才能验证;
    git tag -v v1.4.2.1
    若是没有签署者的公钥,会报告错误;
    git tag -a v1.2 -m "my version v1.2" sha-1
    可以给某个commit添加标签;
    git push并不会把标签传送到远端服务器上,只有通过显式命令才能分享标签到远端仓库;
    推送标签的命令格式如同推送分支,运行git push origin [tagname] 即可;
    git push origin v1.5
    如果要一次推送所有本地新增的标签上去,可以使用--tags选项;
    git push origin --tags
    如果删除远程的tag,可以使用以下命令
    git push origin --delete tag v1.5

# 12 git describe
    git不会为每次提交自动附加类似’v123’的递增序列,所以如果你想要得到一个便于理解的提交号可以运行git describe命令;
    git将会返回一个字符串,由三部分组成:最近一次标定的版本号,加上自那次标定之后的提交次数,再加上一段SHA-1值of the commit you’re describing;
    git describe master
    如果在一个刚刚打完标签的提交上运行describe命令,只会得到这次标定的版本号,而没有后面两项信息;
    git describe命令只适用于有标注的标签(通过-a或者-s选项创建的标签),所以发行版的标签都应该是带有标注的,以保证git describe能够正确的执行;你也可以把这个字符串作为checkout或者show命令的目标;
    最近Linux内核为了保证SHA-1值的唯一性,将位数由8位扩展到10位,这就导致扩展之前的git describe输出完全失效了;

# 13 git archive
    当需要发布一个新的版本时;首先要将代码的压缩包归档,方便那些可怜的还没有使用git的人们;可以使用git archive;
    git archive --format=zip --prefix=git-docs/ HEAD:Documentation/ > git-1.4.0-docs.zip
    git archive -o latest.zip HEAD
    git archive --prefix='project/' --format=tar master | gzip > `git describe master`.tar.gz
    ls *.tar.gz
    v1.6.2-rc1-20-g8c5b85c.tar.gz
    如果建立一个zip压缩包,需要在git archive加上--format=zip选项;
    git archive --prefix='project/' --format=zip master > `git describe master`.zip

# 14 git shortlog
    使用git shortlog命令可以方便快捷的制作一份修改日志(changelog),告诉大家上次发布之后又增加了哪些特性和修复了哪些bug;实际上这个命令能够统计给定范围内的所有提交;假如你上一次发布的版本是v1.0.1,下面的命令将给出自从上次发布之后的所有提交的简介;
    git shortlog [branch] --no-merges --not [tag]
    git shortlog master --no-merges --not v1.0.1
    以上命令会生成自从v1.0.1版本以来的所有提交的简介,内容按照作者分组,以便你能快速的发e-mail给他们;

# 15 git config
    git可以通过命令行配置,也可以直接修改相关配置文件;配置的内容主要包括3部分,第一部分,基本信息类配置;第二部分,工具类配置;第三部分,gitignore配置;
    git配置文件包括三个,三者只是作用范围不同,其中包含的配置内容则完全相同;
    /etc/gitconfig 文件   系统配置文件，影响所有用户。若使用 git config 时用 --system 选项，读写的就是这个文件。
    ~/.gitconfig 文件 用户配置文件，只影响当前用户。若使用 git config 时用 --global 选项，读写的就是这个文件。
    .git/config 文件 仅针对当前工作的仓库有效;
    在 Windows 系统对应主目录下的 .gitconfig 文件。主目录即 %USERPROFILE% 变量指定的目录;git 还会尝试寻找 /etc/gitconfig 文件，以安装目录作为起始地址来定位。
    每一个级别的配置都会覆盖上层的相同配置,所以.git/config里的配置会覆盖/etc/gitconfig中的同名变量;
    git config --list ##可以查看当前的配置列表;

## 15.1 基本信息配置
    每次 Git 提交时都会引用这两条信息，说明是谁提交了更新，会被记录在历史记录中：
    如果用了 --global 选项，会被保存在~/.gitconfig；以后所有的项目都会默认加载这里配置的用户信息。
    如果要在某个特定的项目中使用其他名字或者邮箱，只要去掉 --global 选项重新配置即可将新的配置保存在当前项目的 .git/config 文件;
        git config --global user.name 'username'
        git config --global user.email 'user@example.com'
## 15.2 工具配置
15.2.1 merge/diff工具配置
    当进行各种merge操作时,如果有冲突,git系统会提示用户merge失败;此时,用户可以立即调用git mergetool命令启动相应的merge工具;
    当修改完后,再次调用git mergetool修改下一个冲突的地方,直到全部修改完毕;用户可以调用git commit重新提交然后再重新进行merge操作;
        git config --global merge.tool meld
        添加以下设置,不保留冲突后的遗留文件;
        git config --global mergetool.keepBackup fasle
    git 可以理解 kdiff3，tkdiff，meld，xxdiff，emerge，vimdiff，gvimdiff，ecmerge，和 opendiff 等合并工具的输出信息。
15.2.2 git默认文本编辑器
    当需要输入一些额外消息的时候，git会自动调用操作系统设置的默认编辑器，一般可能会是 geditor 或者 Vim。也可以重新设置：
    $ git config --global core.editor vim
15.2.3 查看配置信息
    要检查已有的配置信息，可以使用 git config --list 命令：
    $ git config --list
    如果看到重复的变量名，说明它们来自不同的配置文件（比如 /etc/gitconfig 和 ~/.gitconfig），不过最终 Git 实际采用的是最后一个。
    也可以直接查看某个环境变量的设置，只要把变量名字跟在后面即可：
```
git config user.name
```

## 15.3 .gitignore配置
    .gitignore文件指定哪些类型的文件不接受git管理;
    .gitignore的语法支持通配符和排除操作;
    要注意半路出家的问题,即在建立仓库是把所有的文件添加到了git仓库,而半路上意识到需要排除某些类型的文件,那么此时修改.gitignore是不会起作用的;
    要解决半路出家的问题,首先需要删除仓库中所有需要排除的文件,使用如下命令
        git rm -r --cached bin ##--cached表示只删除仓库中的内容,工作目录中的文件保持不变;
    有时某些类型的文件会存在于工作目录的各个目录下,则可以使用以下连环命令,比如删除所有目录下的.svn目录
        find . -iname ".svn" | xargs git rm -r --cached
    git rm与git add作用相反,git add告知git系统把文件添加到git管理仓库中,而git rm是告知git从仓库中移除某个文件;
    只有用git add加入仓库的文件,才可以执行git rm删除,否则提示错误;
    git add -p #可以分片段添加并提交修改,并显示更改内容的diff;
    git rm <file> #从管理仓库中删除文件;
    git rm <file> --cached #从版本库中移除文件,但不删除工作区的文件;

# 16 git branch
    每次commit后,git内部会生成一个快照保存所有修改过的文件,并生成一个40位的SHA-1哈希字符串表示本次提交;每一个提交有且仅有一个父提交,多次连续的提交就构成一个分支;
    分支也用一个40位的SHA-1标识,该标识指向分支中最后一次提交,同时为了便于操作每个分支都有一个名称,比如主分支master,副分支develop,临时性分支如功能feature,预发布release,修订fixbug分支等;
    git中的分支,其实本质上仅仅是个指向commit对象的可变指针;git会使用master作为分支的默认名字;在若干次提交后,已经有了一个指向最后一次提交对象的master分支,它在每次提交的时候都会自动向前移动;
    git是如何知道你当前在哪个分支上工作的呢?它保存着一个名为HEAD的特别指针;一个指向你正在工作中的本地分支的指针;
    git branch命令如果不加任何参数,它会列出当前所有分支的清单;
    分支前的*字符表示当前所在的分支;也就是说,如果现在提交更新,master分支将随着开发进度前移;
    若要查看各个分支最后一个提交对象的信息,运行git branch -v;
    git branch -av ##可以查看所有的分支,包括远程分支;
    git branch --merged ##查看哪些分支已被并入当前分支;
    git branch --no-merged ##查看尚未合并的工作;
    创建分支可以基于某个已有分支,也可以基于某次提交;
    git branch tmp  ##基于当前分支/提交创建tmp分支;
    git branch tmp master ##基于master分支创建tmp分支;
    git branch tmp sha-1  ##基于sha-1标识的节点(commit)创建tmp分支;
    运行git branch命令,仅仅是建立了一个新的分支,但不会自动切换到这个分支中去;
    要切换到其他分支,需要执行git checkout命令;
    git checkout -b tmp ##创建tmp分支并提取到当前工作区;
    git checkout -b tmp master ##基于master分支创建tmp分支并提取到工作区;
    git checkout -b tmp sha-1  ##基于sha-1标识的节点(commit)创建tmp分支并提取到工作区;
    git checkout tmp就会将当前工作区切换到tmp的工作区
    我们可以在不同分支里反复切换,并在时机成熟时把它们合并到一起;而所有这些工作,仅仅需要branch和checkout这两条命令就可以完成;
    git branch -m oldbranch newbranch ##修改分支名字
    如果删除分支tmp可以使用命令
    git branch -d tmp ##会检查该分支是否已被完整的merge到上游分支;
    git branch -D tmp ##会忽略工作区状态或merge状态;
    注意删除分支,仅仅是删除分支的名称而已,在git branch命令中看不到该分支,但不会删除该分支所包含的任何commit记录;删除分支后,原分支所引用的提交记录就有可能成为"无引用"的状态;
    如果分支中还包含着尚未合并进来的工作成果,所以简单地用git branch -d 删除该分支会提示错误,因为那样做会丢失数据;
    如果你确实想要删除该分支上的改动,可以用大写的删除选项-D强制执行;
    git branch -r  #查看远程分支
    git branch <new-branch> #创建新的分支
    git branch -v  #查看各个分支最后提交信息;
    git branch --merged   #查看已经被合并到当前分支的分支;
    git branch --no-merged  #查看尚未合并到当前分支的分支;

# 17 git merge
    merge即为合并之意,merge的对象为两个活多个不同的分支,同一个分支的多个提交之间不存在merge之说;
    最容易的整合分支的方法是merge命令,它会把两个分支最新的快照以及二者最新的共同祖先进行三方合并,合并的结果是产生一个新的提交对象;
    git checkout master
    git merge --no-ff develop
    merge的语义是merge from,即从指定的分支合并到当前分支,以上代码的含义就是把develop分支中的改动合并到master中;
    --no-ff选项会在master分支生成一个新节点;为保证版本演进的清晰,我们希望采用这种做法;默认情况下,git执行"快进式合并"(fast-farward merge),会直接将master分支指向develop分支;
    –no–ff参数会执行正常合并,在master分支上生成一个新节点;
    fast-forward式的合并完成后,视图为一个扁平状,看不出分支开发的具体信息;
    --no-ff选项会加入分支开发的完整提交链并生成一个merge commit记录;
    在master合并develop分支之前最好先让master更新到服务器最新版本;
    git checkout master
    git fetch origin #获取远程最新数据;
    git merge origin/master #合并到本地master分支;
    或将以上两条命令替换为git pull --rebase origin master #将origin的master更新到本地仓库并对本地master执行git rebase改变基点;
    git merge --no-ff develop
    git push origin master #推送master到远程仓库
    注意git push的时候不要采用非直进式update(会覆盖远程仓库的历史记录),也不要使用-f强制推送,可能会覆盖他人数据;

    git merge --squash develop ##将develop的提交合并为一个并添加到暂存区;
    git commit #编辑提交日志并提交;

    如果合并中出现冲突,git系统会自动以一种特殊的格式将冲突写入源文件;
```
<<<<<<< HEAD
this is a
=======
this is b
>>>>>>> develop
```
    7个=是冲突的分隔符,前面是当前分支中的内容,后面是指定分支的内容;
    解决冲突的方法只能是手动修改;
    git mergetool可以调用设置的工具辅助合并;
    如果合并时没有冲突,系统会自动将合并后的内容提交;
    merge发生冲突的条件是同一个地方(相同行相同列)包含不同的容;
    当手动merge成功后,需要再次调用git commit将所作修改进行提交,merge才能生效;
    git merge <commit> [HEAD|<commit>]
    git merge --squash <commit> [HEAD|<commit>]
    git merge --no-ff <commit> [HEAD|<commit>]
    git merge --ff-only <commit> [HEAD|<commit>]

    git鼓励在工作流程中频繁使用分支与合并;
    实际开发中可以同时拥有多个开放的分支;建议仅在master分支中保留完全稳定的代码,即已经发布或即将发布的代码;与此同时,还有一个名为develop或next的平行分支,专门用于后续的开发,或仅用于稳定性测试,不过一旦进入某种稳定状态,便可以把它合并到master里;这样在确保这些已完成的特性分支(临时分支)能够通过所有测试,并且不会引入更多错误之后,就可以并到主干分支中,等待下一次的发布;
    其实,常设分支只需要master和develop这两条就够了,不需要其他了;
    在任何规模的项目中都可以使用特性(topic)分支;一个特性分支是指一个短期的,用来实现单一特性或与其相关工作的分支;
    除了常设分支以外,还有一些临时性分支,用于应对一些特定目的的版本开发;临时性分支主要有三种:
        功能(feature)分支
        预发布(release)分支
        修补bug(fixbug)分支
    这三种分支都属于临时性需要,使用完以后,应该删除,使得代码库的常设分支始终只有master和develop;

## 17.1 功能分支
    为了开发某种特定功能,从develop分支上面分出来的;开发完成后,要再并入develop; 功能分支的名字,可以采用feature-*的形式命名;
        #创建一个功能分支:
            git checkout -b feature-x develop
        #开发完成后,将功能分支合并到develop分支:
            git checkout develop
            git merge --no-ff feature-x
        #删除feature分支:
            git branch -d feature-x
            如果对feature分支不满意,不予采纳,直接删除分支
            git branch -D feature-x
## 17.2 预发布分支
    指发布正式版本之前(即合并到master分支之前),我们可能需要有一个预发布的版本进行测试;
    预发布分支是从develop分支上面分出来的,预发布结束以后,必须合并进develop和master分支;它的命名可以采用release-*的形式;
        #创建一个预发布分支:
            git checkout -b release-1.2 develop
        #working on release-1.2
        #确认没有问题后,合并到master分支:
            git checkout master
            git merge --no-ff release-1.2
        #对合并生成的新节点,做一个标签
            git tag -a 1.2
        #再合并到develop分支:
            git checkout develop
            git merge --no-ff release-1.2
        #最后,删除预发布分支:
            git branch -d release-1.2
## 17.3 修补bug分支
    软件正式发布以后,难免会出现bug;这时就需要创建一个分支,进行bug修补;
    修补bug分支是从master分支上面分出来的;修补结束以后,再合并进master和develop分支;它的命名可以采用fixbug-*的形式;
        #创建一个修补bug分支:
            git checkout -b fixbug-0.1 master
        #修补结束后,合并到master分支:
            git checkout master
            git fetch origin
            git merge origin/master ##更新本地master分支
            git merge --no-ff fixbug-0.1
            git tag -a 0.1.1
        #再合并到develop分支:
            git checkout develop
            git merge --no-ff fixbug-0.1
        #最后,删除"修补bug分支":
            git branch -d fixbug-0.1

# 18 git rebase
    假如test分支是基于master的dev1阶段开发的测试分支,当测试完局部功能后接到master分支owner的通知,说master已经有了新的改动,所有局部功能必须基于最新的master状态,此时对于test来讲,就需要对自己当前的分支"改变基点";
    可以使用git rebase命令改变基点;
    假如目前的状态如下图
               A---B---C test
              /
         D---E---F---G master
    当执行以下命令
        git checkout test
        git rebase master
        #以上两条命令等价于git rebase master test
                       A´--B´--C´ test
                      /
         D---E---F---G master
    如果master分支上已经包含了子分支的某次提交比如A;
               A---B---C test
              /
         D---E---A´---F master
    执行git rebase时将会跳过已经包含的提交;
                        B´---C´ test
                       /
         D---E---A´---F master

    rebase后如果有冲突,其解决方法和merge相同,冲突解决后执行如下命令;
        git add <conflict-file>
        git rebase --continue
        或直接调用git commit进行提交即可;
    如果中途退出rebase可以执行git rebase --abort
    rebase后之前的提交并没有删除,而会依然存在,并添加新的提交;
    rebase的具体过程是找到两个分支的共同父节点,然后逐个找到当前分支中每个节点与父节点的差异,并保存一个临时文件,然后把这个临时文件应用到目标基点上;
    rebase的理念是改变局部小功能的基点,merge的理念是相同重量级功能的合并;
    rebase一般是局部小功能发起的,而merge则是主体大功能或相同重量级发起的;
    rebase一般是临时性合并,当前只是和基础代保持一致;merge一般是阶段性合并,合并后有可能不再针对该功能单独开发,并且合并后会删除临时性的分支;

    rebase除了改变当前分支的基点外,还可以改变当前分支中某段修改的基点;
    当开发某个局部功能分支devlep时,假设该局部功能由2个小功能组成,next和topic;现接到通知暂时停止next,使用topic基于master最新内容继续开发;
    已知topic基于next分支;
    当前状态图如下
```
         o---o---o---o---o  master
              \
               o---o---o---o---o  next
                                \
                                 o---o---o  topic
```
    当执行一下命令
        git rebase --onto master next topic
        --onto选项是把从next分支fork(派生)出来的topic分支rebase到master分支上(变为由master分支fork出来);
         o---o---o---o---o  master
              \           \
               \           o´--o´--o´  topic
                o---o---o---o---o  next

    还可以利用git rebase重整历史提交,如删除某段历史记录及变更;
         E---F---G---H---I---J  topicA
    然后执行命令git rebase --onto topicA~5 topicA~3 topicA
         E---H´---I´---J´  topicA
    结果将移除F和G对应的提交;
    警告:改变历史,影响未来;
    命令git rebase --onto <newbase> <since> <till>的执行过程
        git checkout <till>
        <since>..<till>
        git reset --hard <newbase>
        git cherry-pick from (2)

    也可以使用-i选项交互式rebase
    如将下面的提交记录变为后一行
         A---B---C---D---E---F
         A---B---E´---F´
        首先执行git rebase -i B
        然后进入编辑界面,手动删除C和D对应的前2行,即E和F前面的两行
        保存退出即可自动完成;
    如对C的提交说明不够满意,可以执行以下命令重新编辑;
         A---B---C---D---E---F
        然后进入一个编辑界面,将C对应的一行(第1行)前面的pick更改为reword;
        保存退出,即可打开另一个编辑界面,重新编辑,保存退出即可;

    为了脉络清晰的漂亮历史记录,多用rebase,少用merge;
    git rebase --onto <newbase> <since> <till>
    git rebase --onto <newbase> <since> [HEAD]
    git rebase [--onto <since>] <since> <till>
    git rebase [--onto <since>] <since> [HEAD]
    git rebase --continue
    git rebase --skip
    git rebase --abort

# 19 git cherry-pick
    git cherry-pick <SHA-1>
    寻找最好的樱桃,把某次提交的状态单独应用于当前分支;

# 20 Protocol
    git可以使用四种主要的协议来传输数据:本地传输,SSH协议,git协议和HTTP协议;
    除了HTTP协议外,其他所有协议都要求在服务器端安装并运行git;
## 20.1 Local protocol
    所谓的远程仓库在该协议中的表示,就是硬盘上的另一个目录;
    要添加一个本地仓库作为现有git项目的远程仓库,可以这样做
    git remote add local_proj /opt/git/project.git
    然后就可以像在网络上一样向这个远程仓库推送和获取数据了;
    基于文件仓库的优点在于它的简单,同时保留了现存文件的权限和网络访问权限;这种方法的缺点是,与基本的网络连接访问相比,难以控制从不同位置来的访问权限;

## 20.2 SSH Protocol
    SSH也是唯一一个同时支持读写操作的网络协议;另外两个网络协议(HTTP和git)通常都是只读的,所以虽然二者对大多数人都可用,但执行写操作时还是需要SSH;
    通过SSH克隆一个git仓库,可以像下面这样给出ssh:// 的URL
    git clone ssh://user@server:project.git
    git clone user@server:project.git ##git默认使用ssh协议
    如果不指明用户,git会默认使用当前登录的用户名连接服务器;
    通过SSH进行访问是安全的,所有数据传输都是加密和授权的;最后,和git及本地协议一样,SSH也很高效,会在传输之前尽可能压缩数据;
    SSH的限制在于不能通过它实现仓库的匿名访问,这使得SSH不利于开源的项目;
    如果想允许对项目的匿名只读访问,那么除了为自己推送而架设SSH协议之外,还需要支持其他协议以便他人访问读取;

## 20.3 Git Protocol
    git协议是一个包含在git软件包中的特殊守护进程;它会监听一个提供类似于SSH服务的特定端口(9418)而无需任何授权;打算支持git协议的仓库,需要先创建git-export-daemon-ok文件,它是协议进程提供仓库服务的必要条件;但除此之外该服务没有什么安全措施;要么所有人都能克隆git仓库,要么谁也不能;
    也意味着该协议通常不能用来进行推送;
    git协议是现存最快的传输协议;它使用与SSH协议相同的数据传输机制,但省去了加密和授权的开销;
    git协议消极的一面是缺少授权机制;用git协议作为访问项目的唯一方法通常是不可取的;一般的做法是,同时提供SSH接口,让几个开发者拥有推送(写)权限,其他人通过git://拥有只读权限;
    git协议可能也是最难架设的协议;它要求有单独的守护进程,需要定制;该协议还要求防火墙开放9418端口,而企业级防火墙一般不允许对这个非标准端口的访问;大型企业级防火墙通常会封锁这个少见的端口;

## 20.4 HTTP/S Protocol
    HTTP或HTTPS协议的优美之处在于架设的简便性;基本上只需要把git的裸仓库文件放在HTTP的根目录下,配置一个特定的post-update挂钩(hook)就可以搞定;此后,每个能访问git仓库所在服务器上web服务的人都可以进行克隆操作;
    使用HTTP协议的好处是易于架设;几条必要的命令就可以让全世界读取到仓库的内容;花费不过几分钟;
    下面的操作可以允许通过HTTP对仓库进行读取;
        cd /var/www/htdocs/
        git clone --bare /path/to/git_project gitproject.git
        cd gitproject.git
        mv hooks/post-update.sample hooks/post-update
        chmod a+x hooks/post-update
    这样就可以了;git附带的post-update挂钩会默认运行合适的命令(git update-server-info)来确保通过HTTP的获取和克隆正常工作;这条命令在你用SSH向仓库推送内容时运行;之后,其他人就可以用下面的命令来克隆仓库;
        git clone http://example.com/gitproject.git
    HTTP协议的消极面在于,相对来说客户端效率更低;克隆或者下载仓库内容可能会花费更多时间,而且HTTP传输的体积和网络开销比其他任何一个协议都大;因为它没有按需供应的能力,传输过程中没有服务端的动态计算,因而HTTP协议经常会被称为傻瓜(dumb)协议;

# 21 创建裸仓库
    开始架设git服务器前,需要先把现有仓库导出为裸仓库,即一个不包含当前工作目录的仓库;做法直截了当,克隆时用--bare选项即可;裸仓库的目录名一般以.git结尾;
    git clone --bare [源仓库地址]  [裸仓库目录名]
    [源仓库地址]可以是一个本地仓库,也可以是一个远程仓库;
    如git clone --bare project.git project_backup.git
    如git clone --bare ./  project_bk.git
    也可以使用git init --bare命令默认在当前目录创建空白的裸仓库;
    有了裸仓库的副本后,剩下的就是把它放到服务器上并设定相关协议;
    把裸仓库上传到服务器的方法可以是直接拷贝到相关目录,也可以使用命令推送;
    cd project_backup.git
    git push --mirror [remote-url] ##以镜像的方式上传;

# 22 提交指南
    首先,请不要在更新中提交多余的白字符(whitespace);git有种检查此类问题的方法;在提交之前,先运行git diff --check会把可能的多余白字符修正列出来;
    接下来,请将每次提交限定于完成一次逻辑功能;并且可能的话,适当地分解为多次小更新,以便每次小型提交都更易于理解;请不要在周末穷追猛打一次性解决五个问题,而最后拖到周一再提交;就算是这样也请尽可能利用暂存区,将之前的改动分解为每次修复一个问题,再分别提交和加注说明;
    最后需要谨记的是提交说明的撰写;写得好可以让大家协作起来更轻松;一般来说,提交说明最好限制在一行以内,50个字符以下,简明扼要地描述更新内容,空开一行后,再展开详细注解;
    另外,提交说明应该用祈使现在式语态,比如不要说成"I added tests for"或"Adding tests for"而应该用"Add tests for";
    下面是来自tpope.net的Tim Pope原创的提交说明格式模版,供参考;
```
1   本次更新的简要描述(50个字符以内);
2
3   如果必要,此处展开详尽阐述;段落宽度限定在72个字符以内;
4   某些情况下,第一行的简要描述将用作邮件标题,其余部分作为邮件正文;
5   其间的空行是必要的,以区分两者(当然没有正文另当别论);
6   如果并在一起,rebase这样的工具就可能会迷惑;
7
8   另起空行后,再进一步补充其他说明;
9
10   - 可以使用这样的条目列举式;
11
12   - 一般以单个空格紧跟短划线或者星号作为每项条目的起始符;每个条目间用一空行隔开;不过这里按自己项目的约定,可以略作变化;
```
    git clone git://git.kernel.org/pub/scm/git/git.git
    clone git的项目源代码参考其提交历史的说明是如何撰写的;
    git push之前需要先在本地合并其他人员已经提交的更新,并测试一下代码是否仍然正常工作,然后才能继续推送数据;

# 23 参考源代码
git源代码
    git clone git://git.kernel.org/pub/scm/git/git.git
linux kernel源代码
    git clone git://git.kernel.org/torvalds/linux.git
    git clone git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git
RT linux kernel源代码
    git clone git://git.kernel.org/pub/scm/linux/kernel/git/rt/linux-stable-rt.git

# 24 一般的协作流程
    先在自己的特性分支中工作一段时间,完成后合并到自己的master分支;然后下载合并origin/master上的更新(如果有的话),再推回远程服务器;
    git fetch origin ##先把远程仓库更新到origin/master分支
    git checkout master ##切换到本地的master分支
    git merge issue54  ##把自己开发的分支代码合并到master
    git merge origin/master ##把origin/master分支的内容合并到master
    git push origin master ##把本地master分支的内容推送到远程origin的master;

# 25 大项目的合并流程
    git项目本身有四个长期分支:
        用于发布的master分支;
        用于合并基本稳定特性的next分支;
        用于合并仍需改进特性的pu分支(proposed updates);
        用于除错维护的maint分支(maintenance);
    维护者可以将贡献者的代码引入为不同的特性分支,然后测试评估,看哪些特性能稳定工作,哪些还需改进;
    稳定的特性可以并入next分支,然后再推送到公共仓库,以供其他人试用;
    仍需改进的特性可以先并入pu分支;直到它们完全稳定后再并入master,同时一并检查下next分支,将足够稳定的特性也并入master;
    所以一般来说,master始终是在快进,next偶尔做下rebase,而pu则是频繁rebase;
    并入master后的特性分支,已经无需保留分支索引,放心删除好了;
    git项目还有一个maint分支,它是以最近一次发行版为基础分化而来的,用于维护除错补丁;所以克隆git项目仓库后会得到这四个分支,通过检出不同分支可以了解各自进展,或是试用前沿特性,或是贡献代码;而维护者则通过管理这些分支,逐步有序地并入第三方贡献;

# 26 rebase衍合与挑拣(cherry-pick)的流程
    一些维护者更喜欢rebase或者挑拣贡献者的代码,而不是简单的合并,因为这样能够保持线性的提交历史;
    如果你完成了一个特性的开发,并决定将它引入到主干代码中,你可以转到那个特性分支然后执行rebase命令,在你的主干分支上(也可能是develop分支之类的)重新提交这些修改;如果这些代码工作得很好,你就可以快进master分支,得到一个线性的提交历史;

    另一个引入代码的方法是挑拣;
    挑拣类似于针对某次特定提交的rebase;它首先提取某次提交的补丁,然后试着应用在当前分支上;如果某个特性分支上有多个commits,但你只想引入其中之一就可以使用这种方法;也可能仅仅是因为你喜欢用挑拣,讨厌rebase;
    加入dev分支是基于master的某个早期版本,现在只希望采纳dev分支中某次提交,若希望采纳的这次提交的SHA-1为e43a6fd3e94888d76779ad79fb568ed180e5fcdf,就可以用以下命令将该次提交应用到master分支上;
    git checkout master
    git cherry-pick e43a6fd3e94888d76779ad79fb568ed180e5fcdf
    这将会引入e43a6的代码,但是会得到不同的SHA-1值,因为应用日期不同;
    完成挑拣之后就可以删除原来的特性分支并丢弃不想要的那些commit;

# 27 git工具
## 27.1 简短的SHA
    如果想git show提交,下面的命令是等价的(假设简短的版本没有歧义);
    git show 1c002dd4b536e7479fe34593e72e6c6c1819e53b
    git show 1c002dd4b536e7479f
    git show 1c002d
    通常在一个项目中,使用8到10个字符来避免SHA-1歧义已经足够了;最大的git项目之一,Linux内核,目前也只需要最长40个字符中的12个字符来保持唯一性;
## 27.2 引用日志
    可以使用git reflog来查看引用日志;
    如果想查看仓库中HEAD在五次前的值,可以使用引用日志的输出中的@{n}引用
    git show HEAD@{5}
    也可以使用这个语法来查看一定时间前分支指向哪里;
    git show master@{yesterday}
    它就会显示昨天分支的顶端在哪;
    这项技术只对还在你引用日志里的数据有用,所以不能用来查看比几个月前还早的提交;
## 27.3 祖先引用
    如果你在引用最后加上一个^,git将其理解为此次提交的父提交;
    想看上一次提交,可以使用HEAD^,意思是"HEAD 的父提交";
    git show HEAD^
    也可以在^后添加一个数字例如d921970^2,意思是"d921970的第2父提交";这种语法只在合并提交时有用,因为合并提交可能有多个父提交;第一父提交是合并时所在分支,而第二父提交是所合并的分支;
    另外一个指明祖先提交的方法是~;这也是指向第一父提交,所以HEAD~和HEAD^是等价的;
    当指定数字的时候^和~的意义就不一样了;
    HEAD~2是指"第一父提交的第一父提交",也就是"祖父提交",它会根据指定的次数检索第一父提交;
    HEAD~3也可以写成HEAD^^^,同样是第一父提交的第一父提交的第一父提交;
    可以混合使用这些语法,可以通过HEAD~3^2指明先前引用的第二父提交(假设它是一个合并提交);
```
G   H   I   J
 \ /     \ /
  D   E   F
   \  |  / \
    \ | /   \
     \|/     \
      B       C
       \     /
        \   /
         \ /
          A
A =      = A^0
B = A^   = A^1     = A~1
C = A^2  = A^2
D = A^^  = A^1^1   = A~2
E = B^2  = A^^2
F = B^3  = A^^3
G = A^^^ = A^1^1^1 = A~3
H = D^2  = B^^2    = A^^^2  = A~2^2
I = F^   = B^3^    = A^^3^
J = F^2  = B^3^2   = A^^3^2
```
```
B是A的父提交
C是A的第二父提交
D是A的父提交的第一父提交
E是B的第二父提交即A的父提交的第二父提交
F是B的第三父提交即A的父提交的第三父提交
G是D的父提交即A的(第一)父提交的父提交的父提交
H是D的第二父提交即A的(第一)父提交的父提交的第二父提交
I是F的父提交
J是F的第二父提交
```

## 27.4 提交范围
27.4.1 双点
    最常用的指明范围的方法是双点的语法;这种语法主要是让git区分出可从一个分支中获得而不能从另一个分支中获得的提交;即查看后者比前者更新的内容;
    想要查看dev分支上哪些没有被提交到主分支,那么就可以使用master..dev来让git显示这些提交的日志,这句话的意思是"所有可从dev分支中获得而不能从master分支中获得的提交";注意中间是2个点;
    git log master..dev
    如果想看相反的,即所有在master而不在dev分支中的,可以交换分支的名字;dev...master显示所有可在master获得而不在dev中的提交;
    git log dev..master
    如果查看将把什么推送到远程;
    git log origin/master..HEAD
27.4.2 多点
    git允许在引用前使用^字符或者--not指明不希望提交被包含其中的分支;因此下面三个命令是等同的;
    git log refA..refB
    git log ^refA refB
    git log refB --not refA
    如果你想查找所有从refA或refB包含的但是不被refC包含的提交;
    git log refA refB ^refC
    git log refA refB --not refC
    这建立了一个非常强大的修订版本查询系统,可以查询分支里包含了什么;
27.4.3 三点
    可以指定被两个引用中的一个包含但又不被两者同时包含的分支;如果想查看master或者dev中包含的但不是两者共有的引用;即查看两者的差异;
    git log master...dev
    log命令的一个常用参数是--left-right,它会显示每个提交到底处于哪一侧的分支;这使得数据更加有用;
    git log --left-right master...dev
## 27.5 交互式暂存
    运行git add时加上-i或者--interactive选项,git就进入了一个交互式的shell模式;
## 27.6 储藏Stashing
    当正在进行项目中某一部分的工作,里面的东西处于一个比较杂乱的状态,而又想转到其他分支上进行一些工作;问题是又不想提交进行了一半的工作,否则以后无法回到这个工作点;解决这个问题的办法就是git stash命令;
    "储藏"可以获取工作目录的中间状态,也就是修改过的被追踪的文件和暂存的变更,并将它保存到一个未完结变更的堆栈中,随时可以重新应用;
    如果修改了工作区的内容但未提交,又想切换分支,但是不想提交正在进行中的工作,所以储藏这些变更;为了往堆栈推送一个新的储藏,只要运行git stash;
    执行了git stash命令后工作区就干净了;
    这时可以方便地切换到其他分支工作,变更都保存在栈上;要查看现有的储藏,可以使用git stash list;
    可以重新应用刚刚实施的储藏,所采用的命令就是之前在原始的stash命令的帮助输出的提示:git stash apply
    如果想应用更早的储藏,可以通过名字指定它,git stash apply stash@{2}
    如果不指明git stash apply默认使用最近的储藏并尝试应用它;

    对文件的变更被重新应用,但是被暂存的文件没有重新被暂存;想那样的话,必须在运行git stash apply 命令时带上一个 --index的选项来告诉命令重新应用被暂存的变更;
    git stash apply 只尝试应用储藏的工作,储藏的内容仍然在栈上;要移除它,可以运行git stash drop 加上你希望移除的储藏的名字;
    也可以运行git stash pop来重新应用储藏,同时立刻将其从堆栈中移走;
    可以运行git stash branch创建一个新的分支,检出储藏工作时的所处的提交,重新应用到工作,如果成功,将会丢弃储藏;
## 27.7 重写历史
    改变提交的次序/改变说明/修改提交中包含的文件,将提交归并/拆分/完全删除,这一切在尚未开始将工作和别人共享前都是可以的;
27.7.1 修改提交日志
    如果你只想修改最近一次提交说明
    git commit --amend
    然后会打开一个编辑器,就可以修改以前的提交日志信息了;
    如果完成提交后又想修改被提交的快照,增加或者修改其中的文件,可能因为最初提交时忘了添加一个新建的文件,这个过程基本上一样;通过修改文件然后对其运行git add或对一个已被记录的文件运行git rm,随后的git commit --amend会获取当前的暂存区并将它作为新提交对应的快照;
    但这个只以修改最后一个提交的信息,如何修以前的信息,比如最近3次;
```
git rebase -i HEAD~3
```
    将会打开一个编辑器,把将要修改的日志前面标记改成edit然后退出;
    再交替执行3遍以下2条命令;
    git commit --amend
    git rebase --continue
27.7.2 核弹级命令filter-branch
    对版本库进行外科手术;
    全局性地修改电子邮件地址或者将一个文件从所有提交中删除;这个命令是filter-branch,这个会大面积地修改提交历史;
27.7.2.1 从所有提交中删除一个文件
    也许不小心提交了一个包含密码的文件;要从整个历史中删除一个名叫password.txt的文件,可以在filter-branch上使用--tree-filter选项;
    git filter-branch --tree-filter 'rm -f password.txt' HEAD
    --tree-filter选项会在每次检出项目时先执行指定的命令然后重新提交结果;在这个例子中,会在所有快照中删除一个名叫password.txt的文件,无论它是否存在;如果想删除所有不小心提交上去的编辑器备份文件,可以运行类似`git filter-branch --tree-filter 'rm -f *~' HEAD`的命令;
    一个比较好的办法是在一个测试分支上做这些然后在确定产物真的是所要的之后,再hard-reset主分支;要在所有的分支上运行filter-branch的话,可以传递一个--all选项;
27.7.2.2 将一个子目录设置为新的根目录
    假设完成了从另外一个代码控制系统的导入工作,得到了一些没有意义的子目录(trunk, tags等等);如果想让trunk子目录成为每一次提交的新的项目根目录;
    git filter-branch --subdirectory-filter trunk HEAD
    git会自动地删除不对这个子目录产生影响的提交;
27.7.2.3 全局性地更换电子邮件地址
```
git filter-branch --commit-filter '
        if [ "$GIT_AUTHOR_EMAIL" == "centos@localhost" ];
        then
            GIT_AUTHOR_NAME="user";
            GIT_AUTHOR_EMAIL="user@example.com";
            git commit-tree "$@";
        else
            git commit-tree "$@";
        fi' HEAD
```
    这个会遍历并重写所有提交使之拥有新地址;因为提交里包含了它们的父提交的SHA-1值,这个命令会修改历史中的所有提交,而不仅仅是包含了匹配的电子邮件地址的那些;
## 27.8 文件标注
    如果追踪代码中的缺陷想知道这是什么时候为什么被引进来的,文件标注会是最佳工具;它会显示文件中对每一行进行修改的最近一次提交;如果发现自己代码中的一段代码存在缺陷,可以用git blame来标注那段代码,查看那段代码的每一行分别是由谁在哪一天修改的;
    git blame -C -L 12,22 test.c
    -L选项来限制输出范围在第12至22行;
    -C可以让它找出所有的代码移动,分析标注的文件然后尝试找出其中代码片段的原始出处;
## 27.9 二分查找
    bisect会在提交历史中进行二分查找来尽快地确定哪一次提交引入了错误;
    首先执行git bisect start启动,然后用git bisect bad来告诉系统当前的提交已经有问题了;然后告诉bisect已知的最后一次正常状态是哪次提交,使用git bisect good [good_commit];
    git bisect start
    git bisect bad
    git bisect good v1.0
    Bisecting: 6 revisions left to test after this
    [ecb6e1bc347ccecc5f9350d878ce677feb13d3b2] error handling on repo
    git发现在标记为正常的提交(v1.0)和当前的错误版本之间有大约12次提交,于是它检出中间的一个;在这里,可以运行测试来检查问题是否存在于这次提交;如果是,那么它是在这个中间提交之前的某一次引入的;否则问题是在中间提交之后引入的;假设这里是没有错误的,那么就通过git bisect good来告诉git然后继续下面的旅程;
    git bisect good
    Bisecting: 3 revisions left to test after this
    [b047b02ea83310a70fd603dc8cd7a6cd13d15c04] secure this thing
    现在在另外一个提交上了,在刚刚测试通过的和一个错误提交的中点处;假如再次运行测试然后发现这次提交是错误的,因此你通过git bisect bad来告诉git;
    git bisect bad
    Bisecting: 1 revisions left to test after this
    [f71ce38690acf49c1f3c9bea38e09d82a5ce6014] drop exceptions table
    假如这次提交是好的,那么git就获得了确定问题引入位置所需的所有信息;它提示第一个错误提交的SHA-1值并显示一些提交说明以及哪些文件在那次提交里修改过,这样可以找出缺陷被引入的根源;
    完成之后,应该运行git bisect reset来重设HEAD到开始前的地方,否则会处于一个诡异的地方;
    git bisect reset
## 27.10 子模块
    子模块允许将一个git仓库当作另外一个git仓库的子目录;允许克隆另外一个仓库到项目中并且保持提交相对独立;
    假设想把Rack库(一个Ruby的web服务器网关接口)加入到项目中,可能既要保持自己的变更,又要延续上游的变更;首先要把外部的仓库克隆到子目录中;通过git submodule add将外部项目加为子模块;
    git submodule add git://github.com/chneukirchen/rack.git rack
    现在就在项目里的rack子目录下有了一个Rack项目;可以进入那个子目录,进行变更,加入自己的远程可写仓库来推送变更,从原始仓库拉取和归并等等;
    首先注意到有一个.gitmodules文件;这是一个配置文件,保存了项目URL和拉取到的本地子目录;
    如果有多个子模块,这个文件里会有多个条目;很重要的一点是这个文件跟其他文件一样也是处于版本控制之下,就像你的.gitignore文件一样;它跟项目里的其他文件一样可以被推送和拉取;这是其他克隆此项目的人获知子模块项目来源的途径;
    尽管rack是工作目录里的子目录,但git把它视作一个子模块,当不在那个目录里时并不记录它的内容;取而代之的是,git将它记录成来自那个仓库的一个特殊的提交;当在那个子目录里修改并提交时,子项目会通知那里的HEAD已经发生变更并记录当前正在工作的那个提交;通过那样的方法,当其他人克隆此项目,他们可以重新创建一致的环境;
    这是关于子模块的重要一点:记录他们当前确切所处的提交,不能记录一个子模块的master或者其他的符号引用;
    所有的git命令都在两个子目录里独立工作;
## 27.11 克隆一个带子模块的项目
    克隆一个带子模块的项目;当接收到这样一个项目,将得到包含子项目的目录,但里面没有文件;
    git clone git://github.com/schacon/myproject.git
    rack目录存在了,但是是空的;必须运行两个命令git submodule init来初始化本地配置文件,git submodule update来从那个项目拉取所有数据并检出上层项目里所列的合适的提交;
    现在rack子目录就处于先前提交的确切状态了;如果另外一个开发者变更了rack的代码并提交,拉取那个引用然后归并之;归并来的仅仅上是一个指向你的子模块的指针,但是它并不更新子模块目录里的代码;必须再次运行git submodule update;
    每次从主项目中拉取一个子模块的变更都必须这样做,看起来很怪但是管用;
    在子模块目录里工作时应使用git checkout -b work创建一个分支;当再次在子模块里更新的时候,它仍然会覆盖你的工作,但是至少你拥有一个可以回溯的指针;

# 28 git自定义配置
    请首先参考前面的git config命令的使用;
    用git config配置git,要做的第一件事就是设置名字和邮箱地址;
        git config --global user.name "username"
        git config --global user.email "user@example.com"
## 28.1 客户端基本配置
    如果想查看当前git能识别的选项列表,请运行git config --help
28.1.1 core.editor
    git默认会调用环境变量EDITOR定义的值作为文本编辑器,如果没有定义,会调用Vi来创建和编辑提交以及标签信息,可以使用core.editor改变默认编辑器;
        git config --global core.editor gvim
    现在无论环境变量EDITOR被定义成什么,git都会调用gvim编辑信息;
28.1.2 commit.template
    如果把此项指定为系统上的一个文件,当commit提交的时候,git会默认使用该文件定义的内容;例如创建了一个模板文件 ${HOME}/.gitcommit.template.txt;
```
#Description:

#Solution:
#Commit Reason:
#SOC:
#Code Base:
#Function Area:
```
    设置commit.template如下
        git config --global commit.template ${HOME}/.gitcommit.template.txt
    如果有特定的策略要运用在提交信息上,在系统上创建一个模板文件,设置git默认使用它,这样当提交时,策略每次都会被运用;
28.1.3 core.pager
    core.pager指定git运行诸如log/diff等所使用的分页器,能设置成用more或者任何自己喜欢的分页器(默认用的是less),当然也可以什么都不用,设置空字符串;
        git config --global core.pager ''
    这样不管命令的输出量是多少,都会在一页显示所有内容;
28.1.4 help.autocorrect
    该配置项只在git 1.6.1及以上版本有效;
    如果把help.autocorrect设置成1(启动自动修正),那么在只有一个命令被模糊匹配到的情况下,git会自动运行该命令;
28.1.5 git中的着色
    git能够为输出到终端的内容着色,以便凭直观进行快速,简单地分析,有许多选项能供我们使用以符合自己的偏好;
    git会按照需要自动为大部分的输出加上颜色,我们能明确地规定哪些需要着色以及怎样着色,设置color.ui为true来打开所有的默认终端着色;
        git config --global color.ui true
28.1.6 外部的合并与比较工具
    cat /usr/local/bin/extMerge
    #!/bin/sh
    /Applications/p4merge.app/Contents/MacOS/p4merge $*
    默认情况下,git传递以下7个参数给diff;
        path old-file old-hex old-mode new-file new-hex new-mode
    由于仅仅需要old-file和new-file参数,用diff包装脚本来传递它们;
    $ cat /usr/local/bin/extDiff
    #!/bin/sh
    [ $# -eq 7 ] && /usr/local/bin/extMerge "$2" "$5"
        git config --global merge.tool extMerge
        git config --global mergetool.extMerge.cmd 'extMerge "$BASE" "$LOCAL" "$REMOTE" "$MERGED"'
        git config --global mergetool.trustExitCode false
        git config --global diff.external extDiff
    当设法合并两个分支结果却有冲突时,运行git mergetool,git会调用P4Merge通过图形界面来解决冲突;设置包装脚本的好处是能简单地改变diff和merge工具,例如把extDiff和extMerge改成KDiff3,要做的仅仅是编辑extMerge脚本文件;
    $ cat /usr/local/bin/extMerge
    #!/bin/sh
    /Applications/kdiff3.app/Contents/MacOS/kdiff3 $*
    现在git会使用KDiff3来做比较,合并和解决冲突;
    git预先设置了许多其他的合并和解决冲突的工具,而不必设置cmd;
    可以把合并工具设置为:kdiff3/opendiff/tkdiff/meld/xxdiff/emerge/vimdiff/gvimdiff;如果你不想用到KDiff3的所有功能,只是想用它来合并,那么kdiff3正符合你的要求,运行;
        git config --global merge.tool kdiff3
    如果运行了以上命令,没有设置extMerge和extDiff文件,git会用KDiff3做合并,让通常内置的比较工具来做比较;
28.1.7 格式化与空白
    格式化与空白是许多开发人员在协作时,特别是在跨平台情况下,遇到的令人头疼的细小问题;由于编辑器的不同或者Windows程序员在跨平台项目中的文件行尾加入了回车换行符,一些细微的空格变化会不经意地进入大家合作的工作或提交的补丁中;
28.1.7.1 core.autocrlf
    Windows使用回车('\r')和换行('\n')两个字符来结束一行,而Mac和Linux只使用换行一个字符;虽然这是小问题,但它会极大地扰乱跨平台协作;git可以在提交时自动地把行结束符CRLF转换成LF,而在签出代码时把LF转换成CRLF;用core.autocrlf来打开此项功能,如果是在Windows系统上把它设置成true,这样当签出代码时,LF会被转换成CRLF;
        git config --global core.autocrlf true
    Linux或Mac系统使用LF作为行结束符,因此不想git在签出文件时进行自动的转换;当一个以CRLF为行结束符的文件不小心被引入时你肯定想进行修正,把core.autocrlf设置成input来告诉git在提交时把CRLF转换成LF,签出时不转换;
        git config --global core.autocrlf input
    这样会在Windows系统上的签出文件中保留CRLF,会在Mac和Linux系统上,包括仓库中保留LF;
    如果你是Windows程序员,且正在开发仅运行在Windows上的项目,可以设置false取消此功能,把回车符记录在库中;
        git config --global core.autocrlf false
28.1.7.2 core.whitespace
    git预先设置了一些选项来探测和修正空白问题,其4种主要选项中的2个默认打开,另2个关闭;
    默认被打开的2个选项是trailing-space和space-before-tab;trailing-space会查找每行结尾的空格,space-before-tab会查找每行开头的制表符前的空格;
    默认被关闭的2个选项是indent-with-non-tab和cr-at-eol;indent-with-non-tab会查找8个以上空格(非制表符)开头的行,cr-at-eol让git知道行尾回车符是合法的;
    设置core.whitespace,选项以逗号分割;通过逗号分割的链中去掉选项或在选项前加-来关闭;例如,如果想要打开除了cr-at-eol之外的所有选项;
        git config --global core.whitespace trailing-space,space-before-tab,indent-with-non-tab
    如果提交了有空白问题的文件但还没推送到上游,可以运行带有--whitespace=fix选项的rebase来让git在重写补丁时自动修正它们;

## 28.2 服务端基本配置
28.2.1 receive.fsckObjects
    git默认情况下不会在推送期间检查所有对象的一致性;如果想在每次推送时git都检查一致性,设置receive.fsckObjects为true来强迫它这么做;
        git config --system receive.fsckObjects true
    现在git会在每次推送生效前检查库的完整性,确保有问题的客户端没有引入破坏性的数据;
28.2.2 receive.denyNonFastForwards
    如果对已经被推送的提交历史做rebase继而再推送,又或者以其它方式推送一个提交历史至远程分支,且该提交历史没在这个远程分支中,这样的推送会被拒绝;这通常是个很好的禁止策略,但有时在做rebase并确定要更新远程分支,可以在push命令后加-f标志来强制更新;要禁用这样的强制更新功能,可以设置receive.denyNonFastForwards;
        git config --system receive.denyNonFastForwards true
    也可直接将以下语句写入服务端仓库的config文件
```
[receive]
    denyNonFastForwards = true
```
    用服务器端的接收钩子也能达到同样的效果;这个方法可以做更细致的控制,例如禁用特定的用户做强制更新;
28.2.3 receive.denyDeletes
    规避denyNonFastForwards策略的方法之一就是用户删除分支,然后推回新的引用;
        git config --system receive.denyDeletes true
    这样会在推送过程中阻止删除分支和标签,没有用户能够这么做;要删除远程分支,必须从服务器手动删除引用文件;

# 29 git属性
    一些设置项也能被运用于特定的路径中,这些设置项被称为Git属性;可以在目录中的.gitattributes文件内进行设置(通常是项目的根目录),当不想让这些属性文件和项目文件一同提交时在.git/info/attributes进行设置;
    使用属性可以对个别文件或目录定义不同的合并策略,让git知道怎样比较非文本文件,在提交或签出前让git过滤内容;
## 29.1 比较二进制文件
    首先要解决的是最令人头疼的问题,对Word文档进行版本控制;
    不能直接比较两个不同版本的Word文件,除非进行手动扫描;git属性能很好地解决此问题,把下面的行加到.gitattributes文件;
        *.doc diff=word
    当要看比较结果时,如果文件扩展名是"doc",git调用"word"过滤器;什么是"word"过滤器呢?其实就是git使用strings程序把Word文档转换成可读的文本文件,之后再进行比较;
        git config diff.word.textconv strings
    现在如果在两个快照之间比较以.doc结尾的文件,git对这些文件运用"word"过滤器,在比较前把Word文件转换成文本文件;
    如果能找到或自己写个Word到纯文本的转换器的话,效果可能会更好;strings可以在大部分Mac和Linux系统上运行,所以它是处理二进制格式的第一选择;
    还能用这个方法比较图像文件;比较时,对JPEG文件运用一个过滤器,它能提炼出EXIF信息,大部分图像格式使用的元数据;如果下载并安装了exiftool程序,可以用它参照元数据把图像转换成文本;比较的不同结果将会用文本展示;
    echo '*.png diff=exif' >> .gitattributes
    git config diff.exif.textconv exiftool
## 29.2 关键字扩展
    在.gitattributes文件中设置"indent"过滤器过滤*.c文件
        *.c     filter=indent
    然后通过以下配置,让git知道"indent"过滤器在遇到"smudge"和"clean"时分别该做什么;
        git config --global filter.indent.clean indent
        git config --global filter.indent.smudge cat
    于是当暂存*.c文件时,indent程序会被触发;在把它们签出之前,cat程序会被触发;但cat程序在这里没什么实际作用;这样的组合使C源代码在暂存前被indent程序过滤,非常有效;
## 29.3 导出仓库
    当产生一个归档时,可以设置git不导出某些文件和目录;
    如果不想在归档中包含一个子目录或文件,但想把他们纳入项目的版本管理中,能对应地设置export-ignore属性;例如,在test/子目录中有一些测试文件,在项目的压缩包中包含他们是没有意义的;因此,可以增加下面这行到git属性文件中;
        test/ export-ignore
    当运行git archive来创建项目的压缩包时,那个目录不会在归档中出现;
    export-subst还能对归档做一些简单的关键字替换;
    如果想在项目中包含一个叫作LAST_COMMIT的文件,当执行git archive时,最后提交日期自动地注入进该文件,可以这样设置;
        echo 'Last commit date: $Format:%cd$' > LAST_COMMIT
        echo "LAST_COMMIT export-subst" >> .gitattributes
        git add LAST_COMMIT .gitattributes
        git commit -am 'adding LAST_COMMIT file for archives'

# 30 git挂钩
    当某些重要事件发生时,git会调用自定义脚本;
    有两组挂钩:客户端和服务器端;
    客户端挂钩用于客户端的操作,如提交和合并;
    服务器端挂钩用于git服务器端的操作,如接收被推送的提交;
## 30.1 安装一个挂钩
    挂钩都被存储在git目录下的hooks子目录中,即大部分项目中的.git/hooks;git默认会放置一些脚本样本在这个目录中,除了可以作为挂钩使用,这些样本本身是可以独立使用的;所有的样本都是shell脚本,其中一些还包含了Perl的脚本,不过,任何正确命名的可执行脚本都可以正常使用,可以用Ruby或Python或其他;在Git 1.6版本之后,这些样本名都是以.sample结尾,因此必须重新命名;
    把一个正确命名且可执行的文件放入git目录下的hooks子目录中,可以激活该挂钩脚本,此后他一直会被git调用;
## 30.2 客户端挂钩
    有许多客户端挂钩,以下把他们分为:
        提交工作流挂钩;
        电子邮件工作流挂钩
        其他客户端挂钩;
30.2.1 提交工作流挂钩
    有4个挂钩被用来处理提交的过程;
    pre-commit挂钩在键入提交信息前运行,被用来检查即将提交的快照,例如,检查是否有东西被遗漏,确认测试是否运行,以及检查代码;当从该挂钩返回非零值时,git放弃此次提交,但可以用git commit --no-verify来忽略;该挂钩可以被用来检查代码错误(运行类似Init的程序),检查尾部空白(默认挂钩是这么做的),检查新方法(程序的函数)的说明;
    prepare-commit-msg挂钩在提交信息编辑器显示之前,默认信息被创建之后运行;因此,可以有机会在提交作者看到默认信息前进行编辑;该挂钩接收一些选项:拥有提交信息的文件路径,提交类型,如果是一次修订的话,提交的SHA-1校验和;该挂钩对通常的提交来说不是很有用,只在自动产生的默认提交信息的情况下有作用,如提交信息模板/合并/压缩/修订提交等;可以和提交模板配合使用,以编程的方式插入信息;
    commit-msg挂钩接收一个参数,此参数是包含最近提交信息的临时文件的路径;如果该挂钩脚本以非零退出,git放弃提交,因此,可以用来在提交通过前验证项目状态或提交信息;
    post-commit挂钩在整个提交过程完成后运行,他不会接收任何参数,但可以运行git log -1 HEAD来获得最后的提交信息;总之,该挂钩是作为通知之类使用的;
    提交工作流的客户端挂钩脚本可以在任何工作流中使用,他们经常被用来实施某些策略,但值得注意的是,这些脚本在clone期间不会被传送;可以在服务器端实施策略来拒绝不符合某些策略的推送,但这完全取决于开发者在客户端使用这些脚本的情况;所以,这些脚本对开发者是有用的,由他们自己设置和维护,而且在任何时候都可以覆盖或修改这些脚本;
30.2.2 E-mail工作流挂钩
    有3个可用的客户端挂钩用于e-mail工作流;当运行git am命令时,会调用他们,因此,如果没有在工作流中用到此命令,可以跳过本节;
    如果通过e-mail接收由git format-patch产生的补丁,这些挂钩也许对你有用;
    首先运行的是applypatch-msg挂钩,他接收一个参数:包含被建议提交信息的临时文件名;如果该脚本非零退出,git放弃此补丁;可以使用这个脚本确认提交信息是否被正确格式化,或让脚本编辑信息以达到标准化;
    下一个在git am运行期间调用是pre-applypatch挂钩;该挂钩不接收参数,在补丁被运用之后运行,因此,可以被用来在提交前检查快照;能用此脚本运行测试,检查工作树;如果有些什么遗漏,或测试没通过,脚本会以非零退出,放弃此次git am的运行,补丁不会被提交;
    最后在git am运行期间调用的是post-applypatch挂钩;可以用他来通知一个小组或获取的补丁的作者,但无法阻止打补丁的过程;
30.2.3 其他客户端挂钩
    pre-rebase挂钩在rebase前运行,脚本以非零退出可以中止rebase的过程;可以使用这个挂钩来禁止rebase已经推送的提交对象,git pre-rebase挂钩样本就是这么做的;
    该样本假定next是定义的分支名,因此,可能要修改样本,把next改成定义过且稳定的分支名;
    在git checkout成功运行后,post-checkout挂钩会被调用;他可以用来为项目环境设置合适的工作目录;例如:放入大的二进制文件,自动产生的文档或其他一切不想纳入版本控制的文件;
    最后,在merge命令成功执行后,post-merge挂钩会被调用;他可以用来在git无法跟踪的工作树中恢复数据,诸如权限数据;该挂钩同样能够验证在git控制之外的文件是否存在,因此,当工作树改变时,希望这些文件可以被复制;
## 30.3 服务器端挂钩
    可以使用两个服务器端的挂钩对项目实施各种类型的策略;
    这些挂钩脚本可以在提交对象推送到服务器前被调用,也可以在推送到服务器后被调用;推送到服务器前调用的挂钩可以在任何时候以非零退出/拒绝推送/返回错误消息给客户端,还可以如你所愿设置足够复杂的推送策略;

# 31 git典型应用

## 31.1 合并多个提交
    假设要合并最后的2个提交,可以按如下命令进行:
    1-> git rebase –i HEAD~2
    2-> 将第2个pick修改为squash或者s,然后输入":wq"退出;
    3-> 这时git会自动把第二个提交合并到第一个中去;并提示更新commit message;
    4-> 编辑commit message, 然后保存退出;
    5-> 此时git log可以查看本地的(HEAD中)最后两次提交已经被合并为一个;

    假设要合并最后的3个提交,可以按如下命令进行:
    1-> git rebase –i HEAD~3
    2-> 将后面2个pick修改为squash或者s,然后输入":wq"退出;
    3-> 这时git会自动把第二个提交合并到第一个中去;并提示更新commit message;
    4-> 编辑commit message, 然后保存退出;
    5-> 此时git log可以查看本地的(HEAD中)最后两次提交已经被合并为一个;
 
## 31.2 修改最近一次提交日志
    git commit --amend
    然后会打开一个编辑器,修改以前的提交日志信息;
    如果仅仅修改作者git commit --amend --reset-author
    但只会以修改最后一个提交的信息;

## 31.3 修改更早的提交日志
    git rebase -i HEAD~3
    会打开一个编辑器,把将要修改的日志前面改成edit,然后保存退出;
    再运行:
    git commit --amend
    编辑完后,再继续:
    git rebase --continue
           
## 31.4 放弃本地更改并更新
    当使用git pull命令的时候如果提示有本地文件修改了,无法合并的时候,我们可以放弃本地修改,然后再更新;
    git reset --hard
    git pull

## 31.5 提交最新修改
31.5.1 如果修改的代码在master分支
    先获取远程分支最新内容到本地,合并,提交;
    git checkout master
    git fetch origin
    git merge origin/master
    git commit
31.5.2 合并最新修改
    如果master分支未动,修改了master的子分支dev;
    一般情况下为保护master分支,我们常建立临时分支来修改;
    git checkout master
    git fetch origin
    git merge origin/master
    git checkout dev
    git rebase master
    git checkout master
    git merge dev
    git commit

## 31.6 应用补丁
31.6.1 git diff与git apply
    最初有一个文件a,内容是"This is the file a.",放置在master分支中;为了修改代码,我们一般的做法是建立一个新分支;
    git branch Fix
    git checkout Fix
    接下来我们在a文件里面追加一行,然后执行git diff;
    echo 'Fix!!!' >> a
    git diff
```
diff --git a/a b/a
index 4add65f..0d295ac 100644
--- a/a
+++ b/a
@@ -1 +1,2 @@
This is the file a.
+Fix!!!
```
    git diff命令的输出可以直接作为一个Patch:
    git commit -a -m "Fix" ##提交修改到Fix分支;
    git diff master > fix.patch ##与master分支比较得出fix.patch;
    git checkout master
    当然,不用提交也可以直接导出patch
    git diff > fix.patch
     
    当然,实际应用中,我们不会这样在一个分支生成patch,到另一个分支去应用,因为只要merge一下就好了; 我们现在权当没有这个Fix分支;
    一般情况下,为保护master,我们会建立一个专门处理新提交的patch的分支:
        git branch PATCH
        git checkout PATCH
    #Does not apply the patch, show what it’ll do;
        git apply --stat fix.patch
    #test the patch before actually apply it;
        git apply --check fix.patch
    #If no errors, the patch can be applied cleanly;
        #git apply --reject fix.patch
        git am --signoff < fix.patch
    #use git-am instead of git-apply. because git-am allows sign off an applied patch. This may be useful for later reference;
        git commit -a -m "Patch Apply"
    现在在PATCH分支中应用了这个补丁;
    即使有多个文件git diff也能生成一个patch;

31.6.2 git format-patch与git am
    我们也可以在Fix分支中的a添加了一行之后,用git format-patch生成一个patch;
    git checkout Fix
    echo 'Fix!!!' >> a
    git commit -a -m "Fix1"
    git format-patch -M master
        -M选项检查重命名;
        -C选项检查复制和重命名;
        -w, --ignore-all-space选项可以忽略所有空白;
        --ignore-space-at-eol忽略行尾的空白;
        -b, --ignore-space-change忽略空白字符的变化;
    然后生成patch文档0001-Fix1.patch
    cat 0001-Fix1.patch
```
 From 6991743354857c9a6909a253e859e886165b0d90 Mon Sep 17 00:00:00 2011
 From: Sweetdumplings <linmx0130@163.com>
 Date: Mon, 29 Aug 2011 14:06:12 +0800
 Subject: [PATCH] Fix1
 ---
 a |    1 +
 1 files changed, 1 insertions(+), 0 deletions(-)
 diff --git a/a b/a
 index 4add65f..0d295ac 100644
 --- a/a
 +++ b/a
 @@ -1 +1,2 @@
 This is the file a.
 +Fix!!!
 --
 1.7.4.1
```
    git format-patch生成的patch不仅有diff的信息,还有提交者,时间等等;
    而且是个E-mail的文件,可以直接用git send-email发送它;
    git send-email用法:
        http://www.kernel.org/pub/software/scm/git/docs/git-send-email.html
    中文:http://www.cnblogs.com/wwang/archive/2011/04/01/1951742.html

    git format-patch生成的patch,要用git am来应用;
    git checkout master
    git branch PATCH
    git checkout PATCH
    git am --abort
    在使用git-am之前,要首先git am --abort放弃掉以前的am信息,这样才可以进行一次全新的am;
    先检查patch文件
    git apply --stat 001-Fix1.patch
    检查能否应用成功
    git apply --check 001-Fix1.patch
    应用patch
    git am 0001-Fix1.patch
    git commit -a -m "PATCH apply"
    在提交了补丁之后,可以再查看目前文件a的情况:
    cat a
```
This is the file a.
Fix!!!
```
    果然,多了一个Fix!!!
    不过要注意的是,如果master与Fix分支中间有多次提交,它会针对每次提交都生成一个patch;
    如果报错,我们可以根据报错信息,修改对应文件如file;然后再git-apply;
    vi file
    git apply 0001-Fix1.patch
    git add file
    git am --resolved
    如果这个冲突是无法解决的,要撤销整个am的东西,可以运行git am --abort;
    如果只是忽略这一个patch,可以运行git am --skip来跳过这个patch;
 
 
31.6.3 使用git format-patch生成所需要的patch
    当前分支所有超前master的提交:
        git format-patch -M master
    某次提交以后的所有patch:
        git format-patch 4e16         #4e16指的是commit的SHA-1;
    从根到指定提交的所有patch:
        git format-patch --root 4e16
    某两次提交之间的所有patch:
        git format-patch 365a..4e16   #365a和4e16分别对应两次提交的名称
    某次提交(含)之前的几次提交:
        git format-patch –n 07fe      #-n指patch数,07fe对应提交的名称
    故,单次提交为:
        git format-patch -1 07fe
    git format-patch生成的补丁文件默认从1开始顺序编号,并使用对应提交信息中的第一行作为文件名;
    如果使用了--numbered-files选项,则文件名只有编号,不包含提交信息;
    如果指定了--stdout选项,可指定输出位置,如把所有patch输出到一个文件;
    还可以指定`-o <dir>`指定patch的存放目录;

31.6.4 应用patch
    先检查patch文件
        git apply --stat newpatch.patch
    检查能否应用成功
        git apply --check  newpatch.patch
    打补丁
        git am --signoff < newpatch.patch
    使用-s或--signoff选项,可以在commit信息中加入Signed-off-by信息;

    如果应用patch出现问题,参考git am PATCH失败的处理方法
        http://blog.csdn.net/sunnylgz/article/details/7660638
    请注意,git apply是一个事务性操作的命令,也就是说,要么所有补丁都打上去,要么全部放弃;
    对于传统的diff命令生成的补丁,只能用git apply处理;
    对于format-patch制作的新式补丁,应当使用git am命令;

31.6.5  git am `<PATCH>`失败的处理方法
    如果冲突发生git只输出错误信息,然后就停下来;一个小冲突会导致整个patch都不会被集成;处理这种问题的最简单方法是先使用git am --abort,然后手动添加此patch, patch -p1 < PATCH,手动解决掉代码冲突,最后使用 git commit -a提交代码;但是这样做有个问题就是会失去PATCH中原本包含的commit信息;
    事实上,可以使用git apply命令打patch;如同使用patch -p1命令时一样,然后手动解决代码冲突(检视生成的.rej文件,与冲突文件比较,修改冲突内容,并最终把文件加入到index中);
    在使用git-am之前,要首先git am --abort放弃掉以前的am信息,这样才可以进行一次全新的am;
    git am --abort
    git apply --stat PATCH #检查PATCH状态
    git apply --check PATCH
    git am --signoff < PATCH ##应用补丁
    如git-am命令执行失败,可以继续下面的操作解决冲突;
    git apply PATCH --reject ##获取冲突记录
    [edit edit edit] 根据.rej文件手动解决所有冲突;
    git add FIXED_FILES ##重新添加到暂存区
    git am --resolved  ##告知git-am冲突已经结局;
    git am并不改变index,需要使用git apply --reject打patch(保存在.git/rebase-apply),手动解决代码冲突(使用git status查看所涉及文件);然后把所有文件(不仅仅是引起冲突的文件)添加到(git add)index,最后告诉git am已经解决(--resolved)冲突;
    这样做的好处是不需要重新编辑commit信息;而且如果正在打的是一系列patch(就是说在打的是多个patch,比如git am *.patch)不需要反复使用git am --abort又git am就可以使用所有补丁;

31.6.6 冲突的解决
    很多命令都可能出现冲突,但冲突的直接来源是merge和patch,其他命令是执行这两个操作后导致的;
    git rebae #重新设置基准,然后应用补丁;
    git pull #会自动merge
    repo sync #会自动rebase
    git cherry-pick #会应用补丁
    git am *.patch #会引用补丁
    一般来讲,出现冲突时都会有"CONFLICT"字样;但是也有例外repo sync的报错可能并不是直接提示冲突,如error: project mini/sample
    无论是否存在冲突,只要本地修改不是基于服务器最新的,它都可能报告这个错误;
    冲突产生后,文件系统中产生冲突的文件会有文件冲突标记;
    如果产生了大量冲突,可以用以下命令查找冲突文件;
        grep "<<<<<<< HEAD" . -rn
31.6.6.1 merge/patch的冲突解决
    先编辑冲突文件,然后把修改添加到暂存区,最后再git commit提交;
31.6.6.2 rebase冲突的解决
    rebase冲突解决的过程就是解决每个应用补丁冲突的过程;
    解决完一个补丁应用的冲突之后,执行下面的命令标记冲突已经解决;
        git add -u
    -u表示把所有已track的文件的最新修改加入暂存区,但不加入新的文件;
    执行下面的命令继续rebase;
        git rebase --continue
    如果还有冲突,重复以上步骤,直到rebase完成;如果遇到中间某个补丁不需要应用,可以用以下命令忽略;
        git rebase --skip
    如果向回到rebase前的状态,可以执行;
        git rebase --abort
    rebase后不需要执行commit,也不存在新的修改需要提交,都是git自动完成;
36.6.7 编辑冲突文件
    最简单的编辑冲突的办法就是直接编辑冲突文件,把冲突标记删除,把冲突解决正确就可以了;手动解决完冲突之后,需要使用git add告诉git已经处理完毕;
    如果安装了Tortoise工具可以在冲突的文件右击,选择[Edit conflict]选项;冲突处理完毕要点击冲突已处理按钮;
    也可以使用[meld]或[Beyond compare]等图形化工具;

## 31.7 git reset回滚操作
    如果发现错误的将staging中的文件意外的add进index之后,想回退取消,则可以使用命令:git reset HEAD <file>...,同时git add完毕之后,git也会做相应的提示,比如:
```
# Changes to be committed:
#   (use "git reset HEAD <file>..." to unstage)
#
# new file:   Test.scala
```
```
git reset [--hard|soft|mixed|merge|keep] [<commit>|HEAD]
```
    将当前的分支重设(reset)到指定的<commit>或HEAD(如果不显式指定commit,默认是HEAD,即最新的一次提交),并且根据[mode]有可能更新index和工作区;mode的取值可以是hard/soft/mixed/merged/keep;
    下面来详细说明每种模式的意义和效果;
    --hard  重设(reset)index和工作区,从<commit>以来在工作区中的改变都被丢弃,并把HEAD指向<commit>;
    --soft  index和工作区中的内容不作任何改变,仅仅把HEAD指向<commit>;这个模式的效果是,执行完毕后,从<commit>以来的所有改变都会显示在git status的"Changes to be committed"中;可以把<commit>以来的内容重新add并提交;
    --mixed 仅重设index,但是不reset工作区;这个是默认模式,即当不显式告知git reset模式时,会使用mixed模式;这个模式的效果是,工作区的修改不会丢弃,也不会被标记成"Changes to be committed",但是会打出还未被更新的报告;报告如下:
```
    #Unstaged changes after reset:
    #M Test.Scala
    #M test.txt
```
    --merge和--keep用的不多,在下面的例子中说明;

### git reset的典型的应用场景
31.7.1 回滚add并更新仓库但保留工作区更改
```
    $ edit                               (1)
    $ git add test.txt
    $ mailx                              (2)
    $ git reset                          (3)
    $ git pull origin dev                (4)
```
    (1) 编辑文件test.txt做了一些更改,并把更改添加到了index;
    (2) 查看邮件,发现某人要你pull,有一些改变需要你merge下来;
    (3) 然而,index已经乱了,因为index同HEAD commit不匹配了,但是即将pull的东西不会影响已经修改的test.txt,因此可以revert这两个文件的改变;revert后,那些改变应该依旧在工作区中,因此执行git reset;
    (4) 然后,执行了pull之后,自动merge,test.txt的改变依然在工作区中;

31.7.2 回滚最近一次commit
```
$ git commit ...
$ git reset --soft HEAD^      (1)
$ edit                        (2)
$ git commit -a -c ORIG_HEAD  (3)
```
    (1) 当提交了之后,又发现代码没有提交完整,或者又想重新编辑提交的commint,执行git reset --soft HEAD^,工作区还跟reset之前一样,不作任何改变;
    HEAD^指向HEAD之前最近的一次commit;
    (2) 对工作区的文件再次修改;
    (3) 然后使用reset之前那次commit的注释/作者/日期等信息重新提交;
    注意,当执行git reset命令时,git会把老的HEAD拷贝到文件.git/ORIG_HEAD中,在命令中可以使用ORIG_HEAD引用这个commit;
    commit命令中-a选项告诉git自动把所有修改的和删除的文件都放进stage area,未被git跟踪的新建的文件不受影响;
    commit命令中`-c <commit>`或者`-C <commit>`是拿已经提交的commit对象中的信息(作者,提交者,注释,时间戳等)提交;
    这条commit命令把所有更改的文件加入stage area,并使用上次的提交信息重新提交;

31.7.3 回滚最近几次commit
    回滚最近几次commit,并把这几次commit放到叫做topic的branch上去;
```
    $ git branch topic/wip     (1)
    $ git reset --hard HEAD~3  (2)
    $ git checkout topic/wip   (3)
```
    (1) 已经提交了一些commit,但是此时发现这些commit还不够成熟,不能进入master分支,但你希望在新的branch上润色这些commit改动;因此执行了git branch命令在当前的HEAD上建立了新的叫做topic/wip的分支;
    (2) 然后回滚master branch上的最近三次提交;`HEAD~3`指向当前HEAD-3个commit的commit;`git reset --hard HEAD~3`即删除最近的3个commit(删除HEAD, `HEAD^`, `HEAD~2`),将HEAD指向HEAD~3;
    (3) 将工作区切换到topic/wip分支,由于该分支是基于reset前创建的,保留了master上删除的内容;

31.7.4 永久删除最后几个commit
```
$ git commit ...
$ git reset --hard HEAD~3   (1)
```
    (1) 最后3个commit(即HEAD, HEAD^和HEAD~2)提交有问题,想永久删除这三个commit;

31.7.5 回滚merge和pull操作
```
$ git pull                         (1)
Auto-merging nitfol
CONFLICT (content): Merge conflict in nitfol
Automatic merge failed; fix conflicts and then commit the result.
$ git reset --hard                 (2)
$ git pull . topic/branch          (3)
Updating from 41223... to 13134...
Fast-forward
$ git reset --hard ORIG_HEAD       (4)
```
    (1) 从origin拉下来一些更新,但是产生了很多冲突,暂时没有这么多时间去解决这些冲突,因此决定稍候有空的时候再重新pull;
    (2) 由于pull操作产生了冲突,因此所有pull下来的改变尚未提交,仍然在stage area中,这种情况下git reset --hard与git reset --hard HEAD意思相同,即都是清除暂存区和工作区中被搞乱的东西;
    (3) 将topic/branch合并到当前的branch,这次没有产生冲突,并且合并后的更改自动提交;
    (4) 但是此时又发现将topic/branch合并过来为时尚早,因此决定退滚merge,执行git reset --hard ORIG_HEAD回滚刚才的pull/merge操作;
    执行git reset时,git会把reset之前的HEAD放入.git/ORIG_HEAD文件中,命令行中使用ORIG_HEAD引用这个commit;
    同样的,执行pull和merge操作时,git都会把执行操作前的HEAD放入ORIG_HEAD中,以防回滚操作;

31.7.6 在被污染的工作区中回滚merge或者pull
```
$ git pull                         (1)
Auto-merging nitfol
Merge made by recursive.
nitfol                |   20 +++++----
...
$ git reset --merge ORIG_HEAD      (2)
```
    (1) 即便已经在本地更改了一些工作区,也可安全的git pull,前提是知道将要pull的内容不会覆盖工作区中的内容;
    (2) git pull完后,发现这次pull下来的修改不满意,想要回滚到pull之前的状态;
    我们可以执行git reset --hard ORIG_HEAD,但是这个命令有个副作用就是清空工作区,即丢弃本地未add的那些改变;
    为了避免丢弃工作区中的内容,可以使用git reset --merge ORIG_HEAD,注意其中的--hard换成了--merge,这样就可以避免在回滚时清除工作区;

31.7.7 被中断的工作流程
    假如正在开发一个大的feature,此时来了一个紧急的bug需要修复,但是目前在工作区中的内容还没有成型,还不足以commit,但是又必须切换的另外的branch去fix bug;请看下面的例子
```
$ git checkout feature ;# working on "feature" branch and
$ work work work       ;# got interrupted
$ git commit -a -m "snapshot WIP"                 (1)
$ git checkout master
$ fix fix fix
$ git commit ;# commit with real log
$ git checkout feature
$ git reset --soft HEAD^ ;# go back to WIP state  (2)
$ git reset                                       (3)
```
    (1) 这次属于临时提交,因此随便添加一个临时注释即可;
    (2) 这次reset删除了WIP commit,但是暂存区和工作区保持reset前的状态;
    (3) 此时,在index中依然遗留着"snapshot WIP"提交时所做的uncommit changes;git reset将会清理index成为尚未提交"snapshot WIP"时的状态便于接下来继续工作;

31.7.8 回滚git add的一个单独文件
    假设已经add一个文件进入index,但是又不打算把这个文件提交;
    此时可以使用`git reset -- <file>`把这个文件从index中移除但保留工作区文件的更改;
```
    $ git reset -- test.c                      (1)
    $ git commit -m "Commit files in index"    (2)
    $ git add test.c                           (3)
```
    (1) 把文件test.c从index中去除;
    (2) 把index中的文件提交;
    (3) 再次把test.c加入index;

31.7.9 保留工作区并丢弃一些之前的commit
    假设正在编辑一些文件,并且已经提交,接着继续工作,但是现在发现当前在工作区中的内容应该属于另一个branch,与这之前的commit没有什么关系;此时,可以开启一个新的branch,并且保留工作区中的内容;
```
    $ git tag start
    $ git checkout -b branch1
    $ edit
    $ git commit ...                            (1)
    $ edit
    $ git checkout -b branch2                   (2)
    $ git reset --keep start                    (3)
```
    (1) 把在branch1中的改变提交了;
    (2) 此时发现,之前的提交不属于branch1,新建branch2,并切换到了branch2上;
    (3) 此时可以用git reset --keep start把在start之后的commit清除掉,但是保持工作区不变;

## 31.8 git revert记录式回滚
    git revert用于回滚一些commit;对于一个或者多个已经存在的commit,去除由这些commit引入的改变,并且用一个新的commit来记录这个回滚操作;这个命令要求工作区必须是干净的;
    git revert和git reset的功能很相似,但是有区别,具体如下;
    git revert用于用一个commit来记录并回滚早前的commit,经常是一些错误的提交;如果你想干脆扔掉工作区中的东西,可以使用git reset --hard
    比如
    A-> git revert HEAD~3
    丢弃最近的3个commit,把状态恢复到最近的第4个commit,并且提交一个新的commit来记录这次改变;
    B-> git revert -n master~5..master~2
    丢弃从最近的第5个(包含)到第2个(不包含)commit,但是不自动生成commit,这个revert仅仅修改工作区和index;

    git revert和git reset的区别
    1-> git revert是用一次新的commit来回滚之前的commit,git reset是直接删除指定的commit;
    2-> 在回滚这一操作上看,效果差不多;但是在日后继续merge以前的老版本时有区别;因为git revert是用一次逆向的commit"中和"之前的提交,因此以后合并老的branch时,导致这部分改变可能不会再出现,但是git reset是直接把某些commit在某个branch上删除,因而和老的branch再次merge时,这些被回滚的commit应该还会有效;
    3-> git reset 是把HEAD向后移动了一下,而git revert是HEAD继续前进,只是新的commit的内容和要revert的内容正好相反,能够抵消要被revert的内容;

## 31.9 删除远程分支
    删除远程分支就是将本地的空分支push到远程即可;
    git ls-remote idc #查看远程分支
```
HEAD
refs/heads/cpp-1.0
refs/heads/cpp-1.0.RC1
refs/heads/cpp-1.1
refs/heads/distri
```
    git push idc :distri #删除远程叫做diftri的分支
    git ls-remote idc #确认远程分支被删除
```
HEAD
refs/heads/cpp-1.0
refs/heads/cpp-1.0.RC1
refs/heads/cpp-1.1
```

## 31.10 删除本地分支
    使用git branch命令就可以删除本地分支,比如
    git branch -d toBeDelBranch

如何clone(克隆)远程仓库中的指定分支,而非默认的master分支
    在git clone 命令中使用-b参数指定分支名字即可,比如将远端trade.git上的level-1.1分支克隆下来:
    `git clone -b level-1.1 username@192.168.4.40:trade.git`

## 31.11 删除ref之前的历史记录
    删除ref之前的历史记录,把ref到HEAD的历史记录保留下来;
    git checkout --orphan tmp $REF
    这条命令会创建一个叫tmp的分支,该分支没有任何记录,但是所有文件都会存在;可以据此重新提交,构成一个新的记录;完成之后可以把原来的分支直接废弃;
    git commit -m "new start commit"
    git rebase --onto new_start $REF master
    该命令把$REF到$REF与master共父的那一段rebase到new_start新分支上;
    master将会是起始于一个全新开始的new_start,并追加了$REF到master最新内容的分支;到这里master已经是一个删除了过去历史的分支;而且当前位于master分支;
    git branch -D tmp
    最后删除辅助的临时分支tmp即可;
    以上操作可以把某段历史记录分离出来,不可见的那部分用git reflog仍然可见;换言之还有恢复的可能;如果真要彻底丢掉这段历史,执行git gc --prune那就再也找不回来了;

## 31.12 your branch is ahead of ...
31.12.1 Your branch is ahead of 'xxx' by 2 commits.
`[git] your branch is ahead of ...` 解决办法
出现该问题表示在次之前已经有X个commite了，执行命令
`git reset --hard HEAD~X`
解决其中X表示有多少次提交，此命令的意思是回退到x个commit之前。
`git reset --hard HEAD~X`
`git pull --rebase`

# 32 repo
    repo只是google用python脚本编写的调用git的一个脚本,主要是用来下载,管理Android项目的几百个软件仓库;
## 32.1 repo init
    -u选项执行一个URL,从这个URL中获取repository的manifest文件;
    -m选项选择repository中的某个特定的manifest文件,如果不具体指定,表示为默认的manifest文件default.xml;
    -b选项用来指定某个manifest分支;
    repo init -u git://android.git.kernel.org/platform/manifest.git -m dalvik-plus.xml -b releas-1.0
    repo init -u git://github.com/ossxp-com/manifest.git
    repo init -u git://github.com/ossxp-com/manifest.git -b refs/tags/v0.1.2
## 32.2 repo sync
    如果是第一次执行repo sync,相当于git clone,会把repository中的所有内容都clone到本地;
    如果不是第一次执行repo sync,相当于更新合自动合并动作;
    repo sync会更新.repo下面的文件;如果在merge过程中出现冲突,需要手动运行git rebase --continue;
## 32.3 repo forall -c command
    如果需要对所有项目执行一个git command命令,这个命令相当好用;
## 32.4 repo start newbranchname
    创建新的branch分支,针对所有的git库;"."代表当前工作的branch分支;
## 32.5 repo status
    显示所有Git仓库的状态;

# 33 版本库使用规范
## 33.1 提交规范
    使用一致的用户ID,不要随便变更用户名合邮箱;
    在提交说明,文件名合目录名中尽量不要使用中文;
    提交说明的格式:50/72原则;
        第1行不超过50个字符;
        第2行要空行;
        第3行开始大段的详尽描述,在72字节处换行;可以有多个段落;
        和缺陷跟踪系统整合,输入BugID和关键字;
## 33.2 夸平台
    字符集问题
        尽量不要使用中文书写提交说明或用作文件名目录名;
    文件名大小写
        Max(默认)和Windows不区分文件名大小写;
        在大小写敏感的平台如Linux,设置:
            sudo git config --system core.ignorecase true
        可以在克隆版本库时指定参数;
            git clone -c core.ignorecase=true <repository>
    换行符问题
        两类换行符CRLF(Windows),LF(Linux,Mac OS X);
        导致同一版本库不同文件甚至同一文件换行符不一致;
    自动换行符转换
        创建属性文件.gitattributes,参考<<Git权威指南>>P549;
        设置所有文件自动类型判断,文本文件则使用本地换行符;
            * text=auto
    文件属性
        Linux/Mac通过文件属性rwx判断;
        Windows会自动取消文件属性判断;
            git config --system core.fileMode false
            但如果从Linux克隆库直接拷贝会出现错误;
## 33.3 控制文件
    .gitignore控制文件,如
```
# 这是注释行--被忽略
*.a     #忽略所有以.a为后缀的文件;
!lib.a  #但是lib.a文件或目录不要忽略,即使前面设置了对*.a的忽略;
/TODO   #只忽略根目录下的TODO文件,子目录的TODO文件不忽略;
build/  #忽略所有build/目录下的文件;
doc/*.txt   #忽略文件如doc/notes.txt,但是如doc/server/arch.txt不被忽略;
```
    .gitattributes主要针对文件换行符转换,如
```
*.txt       text
*.vcproj    eol=crlf
*.sh        eol=lf
*.jpg       -text
*.jpeg      binary
```

## 33.4 产品发布
    建立签名里程碑
        git tag -s <tagname> <commit>
    签名里程碑的验证
        git tag -v <tagname>
    公钥导出
        导出到公共PGP公钥服务器;
        保存在项目文件中;
    私钥的维护
        谁签名谁保管;

