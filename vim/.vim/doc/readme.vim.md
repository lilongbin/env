
# change list
* 2014-12-05 created by Longbin Li
* 2021-01-12 update to markdown format

# 1. config vim scripts
we can use these scripts to explorer souce code.
Before doing this, we may need to do following steps:

## change directory to our home dir.
for instance, /home/mike; and then
```
tar -zxvf vim_cfg_bk.tgz -C ~/
```

## then change current directory to the source code location.
for instance, kernel source locates /opt/kernel/
```
cd /opt/kernel
```
and android source locates /home/mike/projects/p1/LINUX/android/
```
cd /home/mike/projects/p1/LINUX/android
```
then use below commands to generate cscope files and tags in order to exploring source code;
```
bash ~/toolkit/shell/gen_kernel_SrcExpl.sh      #for kernel
bash ~/toolkit/shell/gen_android_SrcExpl.sh     #for android
```
after this, we could see cscope.out and tags files under the source code directory;

# 2. use vim to open the file which you want to explorer.
for instance,
```
vim /opt/kernel/driver/input/touchscreen/atmel_mxt_ts.c
```
then we could do following by
+   `<F2>` toggle the taglist window
+   `<F3>` toggle the SrcExpl window, please do not open in android src
+   `<F4>` toggle the NERDTree window

the quick buttons were defined in `~/.vimrc` and `~/.vim/plugin/trinity.vim`
SrcExpl is based on ctags file tags;
```
ctags -R *
```

Following shottcuts could help us jump freely, but we only recommend you use them
when analysising kernel source;
but for android please do not use them since its source code too large amount;
when analysising android source code please use the find commands which will be
introduced a moment later;
+   `<C-j>`  show the next definition-------in SrcExpl window; need tags
+   `<C-z>`  back to last location----------in SrcExpl window; need tags
+   `<C-]>`  jump to definition in the edit window---need cscope.out/tags
+   `<C-t>`  jump back to the last location----------need cscope.out/tags

below methods help us to jump to the file or function defination where we want to go;
to jump back we need use `<C-t>`; and we should have cscope.out file firstly;
It's recommended that to use this method to jump freely, especialy in android;
```
cscope-indexer -r
```

when the cursor is on the string/filename/function,
we could use following key to find something shortcutly;
+    `\f`          find the file and open it
+    `\g`          find global definition
+    `\s`          find this symbol
+    `\d`          find out functions those called by this func
+    `\c`          find functions those calling/reference this func
+    `\i`          find file #include/reference this file
+    `\t`          find text string
+    `\e`          find egrep pattern

we could also use bellow commands by the underline mode of vim.
+   `:cs find f ME`    find ME and open it; ME is a file such as stdio.h
+   `:cs find g ME`    find ME; ME is a global definition
+   `:cs find s ME`    find ME; ME is a symbol
+   `:cs find d ME`    find who called ME, ME is a func or macro etc.
+   `:cs find c ME`    find the funcs; the funcs is calling/reference ME
+   `:cs find i ME`    find the files; the files #include/reference ME
+   `:cs find t ME`    find ME; ME is a text string
+   `:cs find e ME`    find ME; ME is an egrep pattern

# 3. auto completion system in vim.
write following into ~/.vimrc file
```
let g:neocomplcache_enable_at_startup=1
set completeopt+=longest
let g:neocomplcache_enable_auto_select=1
let g:neocomplcache_disable_auto_complete=1
```
when need auto completion just press `<C-n>` to select which you like;

# 4. comment or uncomment to the source code.
+    `\cc`         comment
+    `\cu`         uncomment
+    `\cm`         comment minimal
+    `\ci`         comment or uncomment
+    `\cs`         comment for block codes sexily
+    `\cA`         comment to the end and go to insert mode
+    `\ca`         alternate the delimiters will be used

# 5. regular operations in vim.

## 5.1 We can use following keys to fold and/or unfold the source code session.
foldenable
+    `zi`         open/close all fold
+    `zo`         open the fold under current cursor
+    `zc`         close the fold under current cursor
+    `<Space>`    open/close the fold under current sursor

## 5.2 hilight the word by txtbrowser script.
txtbrowser
+    `\h`         hilight the word under cursor in current file
+    `*`          search forward for the word under cursor
+    `#`          search backword for the word under cursor

## 5.3 Digraphs
Some characters are not on the keyboard.  For example, the copyright character (©).
To type these characters in Vim, you use digraphs, where two characters represent one.
To enter a ©, for example, you press three keys: `CTRL-K Co`
To find out what digraphs are available, use the following command: `:digraphs`
Vim will display the digraph table.  Here are three lines of it:
```
  AC ~_ 159  NS |  160  !I ¡  161  Ct ¢  162  Pd £  163  Cu ¤  164
  Ye ¥  165  BB ¦  166  SE §  167  ': ¨  168  Co ©  169  -a ª  170
  << «  171  NO ¬  172  -- ­  173  Rg ®  174  'm ¯  175  DG °  176
  +- ±  177  2S ²  178  3S ³  179 
```
This shows, for example, that the digraph you get by typing CTRL-K Pd is the
character (£).  This is character number 163 (decimal).

## 5.4 Hexedit
To view binary file could use the command
```
vim -b test.bin
```
then use below command to change to hex mode
```
:%!xxd
```
back to text mode use:
```
:%!xxd -r
```

## 5.5 set spell
To set spell, use
```
:set spell
```
then use `z=` to correct words

# 6. quick options
## 6.1 line option
copy lines(n1~n2) to the position below line n3:
```
:n1,n2 co n3
```
remove lines(n1~n2) to the position below line n3:
```
:n1,n2 m n3
```
write lines(n1~n2) to file:
```
:n1,n2 w file 
```
delete lines(n1~n2):
```
:n1,n2 d 
```

## 6.2 vim diff
open vim diff:
```
vim -d file1 file2
```
put diff patch to the other file: `dp`
get diff patch from the other file: `do`

## 6.3 man linux api
press `<Shift-k>` to view man info of linux api

## 6.4 record vim macro
enter normal mode, press `qa`(or qb/qc, etc.);
start to record your operation and save to a/b/c register;
press `Esc` back to normal mode, press `q` to stop record;
press `@a` or `N@a` (N is a number)to play your record;

## 6.5 write file as root
`:w !{cmd}` will excute cmd and regard current file buffer as standard input;
```
:w !sudo tee %
```
> and % is a read-only register indicate current file name;
>> sudo tee % will capture stdin to % file;

## 6.6 replace
```
:%s/\t/    /gc
```
> `%` means totle file, s means replace, `/1/2/3` means replace 1 by 2 with option 3,
> option 3 could use gc, g means whole line, c means need confirm by user.

replace with regular expression.
vim can support regular expression, when need reference re match result should use `\(\)` and `\N`
```
:%s/^\(.*\).vmdk$/\1.vmdk \1.vdi/gc
```

## 6.7 remove consecutive blank lines
```
:%s/\n\{3,\}/\r\r/
```
if blank lines contain space, remove spaces first;
```
:%s/^\s\+$//
```


