"" File name: ~/.vimrc
"" Created by longbin <beangr@163.com>
"" Released Date:	2014-12-05
"" Last Modified:	2018-08-06
"" Please put this file and .vim/ directory to your $HOME
"" Debug vim plugins use below option:
"" vim test.txt --startuptime time.log

" set fileformats="unix,dos"
" set ff=unix
"" line number setting
set fileencodings=usc-bom,utf-8,GB18030,gbk,big5
set guifont=Ubuntu\ Mono:h13
set nu

"" the general indentation setting
set tabstop=4
set shiftwidth=4
set softtabstop=4
set expandtab
set noautoindent
" set autoindent
" set cindent
" set smartindent

" set fillchars=vert:\ ,stl:\ ,stlnc:\  

"" mouse function setting
" set mouse=a

"" avoid pressing Q into Ex mode
map Q <nop>

"" match and display for searching
set ignorecase
set smartcase
set incsearch
set nohlsearch
" set nowrapscan
" match parenthethese
set showmatch
set matchtime=5
set magic

set autoread

"" scroll off board below or above
set scrolloff=9

"" Underlined the chars in which single line is more than 80 chars
" au BufWinEnter * let w:m2=matchadd('Underlined', '\%>' . 80 . 'v.\+', -1)
" set report=0

"" Use Vim defaults instead of 100% vi compatibility
set nocompatible

"" fold enable of code sections setting
" set foldenable
"" fold method: indent/syatax/marker
" set foldmethod=indent
" set foldcolumn=0
" setlocal foldlevel=100
" set foldclose=all
"" press zi to open/close all fold
"" press zo/zc to open/close fold under current cursor
" nnoremap <Space> @= ((foldclosed(line('.')) < 0) ? 'zc' : 'zo')<CR>


"" show status information
set laststatus=2
" highlight StatusLine cterm=bold ctermfg=NONE ctermbg=NONE guifg=SlateBlue guibg=yellow
" highlight StatusLine cterm=bold ctermfg=white ctermbg=black
" highlight StatusLineNC cterm=bold ctermfg=white ctermbg=black
set statusline=[%n]\ \ %F%m%r%h\ %=%{&encoding}\ %l,%c\ %p%%\ %L\ 

if $TERM != 'ansi'
	set t_Co=256
else
	"" for SecureCRT
	" set t_Co=8
endif

syntax reset
syntax on

"" current color scheme setting
if $TERM == 'ansi'
	"" for SecureCRT
	" colorscheme evening
	" colorscheme light "good"
	" colorscheme morning
	colorscheme oldlace "good"
	" colorscheme peachpuff "good"
	" colorscheme torte
elseif $TERM == 'xterm'
	" colorscheme light   "good"
	colorscheme oldlace  "good"
	" colorscheme peachpuff
	" colorscheme torte
else
	"" if current term's type is not xterm, then syntax off
	" colorscheme blue
	" colorscheme darkblue
	" colorscheme default
	" colorscheme delek
	" colorscheme desert
	" colorscheme elflord
	" colorscheme evening
	" colorscheme koehler
	" colorscheme morning
	" colorscheme murphy
	colorscheme oldlace "good"
	" colorscheme pablo   "good"
	" colorscheme peachpuff "good"
	" colorscheme ron
	" colorscheme shine
	" colorscheme slate
	" colorscheme torte
	" colorscheme zellner
	" syntax off
endif


"" show sursor line and highlight cusor line
set cursorline
" highlight CursorLine cterm=NONE ctermbg=LightGray ctermfg=NONE
highlight CursorLine cterm=NONE ctermbg=LightGray ctermfg=None


"" close the direction key in the normal mode and insert mode
" nnoremap <up>  <nop>
" nnoremap <down> <nop>
" nnoremap <left> <nop>
" nnoremap <right> <nop>
" inoremap <up>  <nop>
" inoremap <down> <nop>
" inoremap <left> <nop>
" inoremap <right> <nop>


"" define mapleader <leader> as "\" key
let mapleader="\\"

" set fileformats=unix,dos,mac
" nmap <leader>fd :set fileformat=dos<CR>
" nmap <leader>fu :set fileformat=unix<CR>

"" for buffer explorer setting
"" <leader>be   show buffer in the edit window
"" <leader>bd   close current buffer
"" <leader>bw   close current buffer and write into
let g:bufExplorerShowDirectories=1
"" show the relative path not the absolute path
let g:bufExplorerShowRelativePath=0
let g:bufExplorerSplitOutPathName=0

"" for cSyntaxAfter plugin setting
" autocmd! BufRead,BufNewFile,BufEnter *.{c,cpp,h,javascript} call CSyntaxAfter()


"" for neocomplcache setting
" let g:neocomplcache_enable_at_startup=1
" set completeopt+=longest
" let g:neocomplcache_enable_auto_select=1
" let g:neocomplcache_disable_auto_complete=1


"" for NERD_commenter setting
"" <leader>ci add/del commenter toggle
"" <leader>cs add block sexy commenter
"" <leader>cm minimal add commenter
"" <leader>cc add commenter only
"" <leader>cu uncommenter only
"" <leader>cA add commenter to the end and go to insert mode
"" <leader>ca alternate the delimiters will be used
let NERDSpaceDelims=1


"" for NERD_Tree setting
"" ? to show help information
" map <F2> :NERDTreeMirror<CR>
" map <F2> :NERDTreeToggle<CR>
let NERDTreeWinPos='right'
let NERDTreeShowHidden=0
let NERDChrismasTree=1
let NERDTreeWinSize=26
let NERDTreeDirArrows=1
let NERDTreeQuitOnOpen=0
let NERDTreeIgnore=['.*\.o$','.*\.ko$','.*\.gz$','.*\.bz2$','.*\.rar']
let NERDTreeShowLineNumbers=0
let NERDTreeAutoDeleteBuffer=1
let NERDTreeHighlightCursorline=1


"" for Trinity plugin setting
"" need to judge whether the trinity.vim plugin exists or not
" if !exists("loaded_trinity")
if glob("x`ls ${HOME}/.vim/plugin/trinity.vim 2>/dev/null`") != "x"
	"" <F2> to toggle Taglist
	"" <F3> to toggle SourceExplorer
	"" <F4> to toggle NERDTree
	"" <C-j> to go to next define location
	"" <C-z> to come back from the definition
	"" Ctrl+] to jump to the definition
	"" Ctrl+t to come back from the jump
	" nmap <F8> :TrinityToggleAll<CR>
	if glob("x`ls ${HOME}/.vim/plugin/taglist.vim 2>/dev/null`") != "x"
		nmap <special> <F2> :TrinityToggleTagList<CR>
	endif
	if glob("x`ls ${HOME}/.vim/plugin/srcexpl.vim 2>/dev/null`") != "x"
		nmap <special> <F3> :TrinityToggleSourceExplorer<CR>
		" nmap <special> <F3> :call g:SrcExpl_PrevDef()<CR>
		nmap <C-J> :call g:SrcExpl_NextDef()<CR>
		nmap <C-Z> :call g:SrcExpl_GoBack()<CR>
		" nmap <C-J> <C-W>j :call g:SrcExpl_Jump()<CR>
	endif
	if glob("x`ls ${HOME}/.vim/plugin/NERD_tree.vim 2>/dev/null`") != "x"
		nmap <special> <F4> :TrinityToggleNERDTree<CR>
	endif
endif


"" for *.txt file syntax setting
filetype plugin on
filetype indent on
if !exists("autocommands_loaded")
	let autocommands_loaded = 1
	au BufEnter,BufNewFile,BufRead *.txt setlocal ft=txt
	au BufEnter,BufNewFile,BufRead *.log* setlocal ft=log
	au BufEnter,BufNewFile,BufRead *.bugreport setlocal ft=bugreport
	au BufEnter,BufNewFile,BufRead *.{md,mkd} setlocal ft=markdown
	au BufEnter,BufNewFile,BufRead *.py set expandtab

	"" set for python
	au FileType python set tabstop=4 softtabstop=4 shiftwidth=4 expandtab
	"" set for aptiv cpp
	"" au BufEnter,BufNewFile,BufRead *.cpp set tabstop=3 softtabstop=3 shiftwidth=3 expandtab

	""" This autocommand jumps to the last known position in a file
	"""	just after opening it, please verify the permission of ~/.viminfo
	""" Or just rm ~/.viminfo use: sudo rm ~/.viminfo
	au BufReadPost * if line("'\"") > 0 && line("'\"") <= line("$") | exe "normal! g`\"" | endif

	" add header comments for .h .c .hpp .cpp .mk .sh new file
	" auto call SetTitle func
	au BufNewFile *.[ch],*.hpp,*.cpp exec ":call SetTitle()"
	au BufNewFile *.mk,[Mm]akefile exec ":call SetTitle()"
	au BufNewFile *.sh,*.py exec ":call SetTitle()"

endif

"" for markdown.vim plugin setting
let g:vim_markdown_frontmatter=1


"" for taglist.vim plugin setting
let Tlist_Auto_Open=0
let Tlist_Auto_Update=1
let Tlist_Ctags_Cmd='/usr/bin/ctags'
" let Tlist_Ctags_Cmd='C:\Program Files\ctags\ctags.exe'
" let Tlist_Ctags_Cmd='%HOMEPATH%\vimfiles\plugin\ctags.exe'
let Tlist_Show_One_File=1
let Tlist_Exit_OnlyWindow=1
let Tlist_Use_Right_Window=0
let Tlist_Max_Tag_Length=30
let Tlist_File_Fold_Auto_Close=1
let Tlist_Enable_Fold_Column = 0
let Tlist_Inc_Winwidth=1
let Tlist_Process_File_Always=1
let Tlist_GainFocus_On_ToggleOpen=1
let Tlist_Sort_Type="order"
" let Tlist_Sort_Type="name"


"" search and load the tags automaticly for ctags plugin
"" use this command to generate tags file: ctags -R *
"" should install ctags firstly
" if executable("/usr/bin/ctags")
if glob("x`which ctags 2>/dev/null`") != "x"
	set tags=tags;
endif

"" automatically change directory
"" It will change to the directory containing the file
"" which was opened or selected.
if exists("+autochdir")
	set autochdir
else
	if has("autocmd")
		autocmd BufEnter * silent! lcd %:p:h:gs/ /\\/
	endif
endif


"" for cscope plugin setting
"" should install cscope firstly
if has("cscope")
	"" before use please generate the cscope.out file by:
	"" cscope-indexer -r
	set csprg=/usr/bin/cscope
	"" use :cstag instead of the default :tag behavior
	"" use ":cs find g" not ":tag"
	"" cst is the condensation of cscopetag
	set cst
	"" if csto=1 <C-]> use tags file firstly, then cscope.out file
	"" we recommend tags file first when scanning C language code
	set csto=1
	"" do not display msg when add cscope.out file
	set nocsverb
	"" display the depth of matched file path
	set cspc=6

	"" search and add cscope.out file automaticlly
	if filereadable("cscope.out")
		cs add cscope.out
	else
   	"" search cscope.out anywhere
		let cscope_file=findfile("cscope.out",".;")
		let cscope_pre=matchstr(cscope_file,".*/")
		if !empty(cscope_file) && filereadable(cscope_file)
			exe "cs add" cscope_file cscope_pre
		endif
		set csverb
	endif

	"" map the shortcut key
	"" find symbol
	nmap <leader>s :cs find s <C-R>=expand("<cword>")<CR><CR>
	"" find global definition
	nmap <leader>g :cs find g <C-R>=expand("<cword>")<CR><CR>
	"" find called by this function
	nmap <leader>d :cs find d <C-R>=expand("<cword>")<CR><CR>
	"" find callers
	nmap <leader>c :cs find c <C-R>=expand("<cword>")<CR><CR>
	"" find text string
	nmap <leader>t :cs find t <C-R>=expand("<cword>")<CR><CR>
	"" find egrep pattern
	nmap <leader>e :cs find e <C-R>=expand("<cword>")<CR><CR>
	"" find and open file
	nmap <leader>f :cs find f <C-R>=expand("<cfile>")<CR><CR>
	"" find files #include this file
	nmap <leader>i :cs find i <C-R>=expand("<cfile>")<CR>$<CR>

endif


" add comment for cpp
func SetComment_ch()
	call setline(1,"/*****************************************")
	call append(line("."),   "* Copyright (C) ".strftime('%Y')." * Ltd. All rights reserved.")
	call append(line(".")+1, "* ")
	call append(line(".")+2, "* File name   : ".expand('%:t'))
	call append(line(".")+3, "* Author      : longbin")
	call append(line(".")+4, "* Created date: ".strftime('%F %T'))
	call append(line(".")+5, "* Description : ")
	call append(line(".")+6, "*")
	call append(line(".")+7, "*******************************************/")
	call append(line(".")+8, "")
endfunc

" add comment for shell,Makefile
func SetComment_sh()
	call setline(3, "####################################################")
	call setline(4, "# Author      : longbin")
	call setline(5, "# Created date: ".strftime('%F %T'))
	call setline(6, "####################################################")
	call setline(7, "")
endfunc

" SetTitle func, add comment
func SetTitle()
	if &filetype == 'make'
		call setline(1,"#target: prerequisites ")
		call setline(2,"#\tcommand")
		call SetComment_sh()

	elseif &filetype == 'sh'
		call setline(1,"#! /bin/bash")
		call setline(2,"# ")
		call SetComment_sh()

	elseif &filetype == 'python'
		call setline(1,"#! /usr/bin/env python")
		call setline(2,"# coding=utf-8")
		call SetComment_sh()

	else
		call SetComment_ch()
		if expand("%:e") == 'hpp'
			call append(line(".")+9,  "")
			call append(line(".")+10, "#ifndef _".toupper(expand("%:t:r"))."_H")
			call append(line(".")+11, "#define _".toupper(expand("%:t:r"))."_H")
			call append(line(".")+12, "#ifdef __cplusplus")
			call append(line(".")+13, "extern \"C\"")
			call append(line(".")+14, "{")
			call append(line(".")+15, "#endif")
			call append(line(".")+16, "")
			call append(line(".")+17, "#ifdef __cplusplus")
			call append(line(".")+18, "}")
			call append(line(".")+19, "#endif")
			call append(line(".")+20, "#endif //".toupper(expand("%:t:r"))."_H")

		elseif expand("%:e") == 'h'
			call append(line(".")+9, "#pragma once")

		elseif &filetype == 'c'
			call append(line(".")+9,  "#include <stdio.h>")
			call append(line(".")+10, "#include \"".expand("%:t:r").".h\"")

		elseif &filetype == 'cpp'
			call append(line(".")+9,  "#include <iostream>")
			call append(line(".")+10, "")
			"call append(line(".")+10, "#include \"".expand("%:t:r").".h\"")
			call append(line(".")+11, "#include <string>")
			call append(line(".")+12, "#include <map>")
			call append(line(".")+13, "#include <vector>")
			call append(line(".")+14, "using namespace std;")
			call append(line(".")+15, "int main() {")
			call append(line(".")+16, "\treturn 0;")
			call append(line(".")+17, "}")

		endif

	endif
endfunc

"" Quick run
func! QuickCompileRun()
	exec "w" 
	" echo &filetype
	if &filetype == 'c' 
		exec '!g++ % -o %<'
		exec '!time ./%<'
	elseif &filetype == 'cpp'
		exec '!g++ -std=c++11 % -o %<'
		exec '!time ./%<'
	elseif &filetype == 'java' 
		exec "!javac %" 
		exec "!time java %<"
	elseif &filetype == 'python'
		exec '!time python %'
	elseif &filetype == 'sh'
		exec '!time bash %'
	elseif &filetype == 'html'
		exec "!firefox % &"
	elseif &filetype == 'go'
		exec "!go build %<"
		exec "!time go run %"
	elseif &filetype == 'markdown'
		exec "!~/.vim/plugin/markdown.pl % > %.html &"
		exec "!firefox %.html &"
		exec "!rm %.html "
	endif
endfunc 
map <C-F5>  :call QuickCompileRun()<CR>
" map <C-R> :call QuickCompileRun()<CR>

