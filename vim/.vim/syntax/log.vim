"Script_name:	log.vim
"Author:		longbin <beangr@163.com> 2014/12/06
"Release:		1.6.2
"Description:	syntax for plain/log.
"Last Modified: 2015-05-12
"Language:		log/plain :)
"Location:		$HOME/.vim/syntax or $VIMRUNTIME/syntax/
"Install_detail:
        "1. put this file in $HOME/.vim/syntax or $VIMRUNTIME/syntax/ 
        "2. Add the following line in your .vimrc:
        "syntax on "syntax highlighting on
        "filetype plugin on
        "au BufRead,BufNewFile *.log setlocal ft=log "syntax highlight for log.vim 

" Quit when a (custom) syntax file was already loaded
if exists("b:current_syntax")
  finish
endif

let s:cpo_save = &cpo
set cpo&vim

"set case insensitive.
syn case ignore

"This script is encoded as utf8, and will convert to appropriate value when running.
scriptencoding utf-8

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" key words definition.
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"Keywords
syn keyword logTodo todo fixme note debug comment warning notice caution cannot
syn keyword logError error bug dropped fail failed failure null overflow wrong invalid exception dump dumped crash
syn keyword logDoing running run start starting loading calling enabling registering setting initializing restoring Switching
syn keyword AndroidOK ok okay success successfully succeeded enable enabled initial resume
syn keyword AndroidNO no not none disable disabled stop stopped suspend done end finished initialised initialized off


"""logComment: Lines that start with '#'
"""以#号打头的行为注释文本
" syn match   logComment '^#.*$' contains=logTodo,logError

"""logComment: Lines that start with '/*' end with '*/'
""" 单行的情况
" syn match   logComment '/\*[^\n/]*\*/' contains=logTodo,logError
" syn match   logComment '/\*[^\*]*/*[^\*]*\*/' contains=logTodo,logError
""" 有多次换行不超过20行,但没有/的情况
" syn match   logComment '/\*\([^/]*\n\)\{1,20\}.*\*/' contains=logTodo,logError
""" 共2行,第2行第一个可见字符为*,以*/做行的结尾
" syn match   logComment '/\*.*\n\(\s*\*.*\).*\*/' contains=logTodo,logError
""" 有多次换行,但每行以*字符开始,最后以*/做行尾
" syn match   logComment '/\*.*\n\(\s*\*.*\n\)\{0,20\}\s*\*.*\*/' contains=logTodo,logError
""" 有多次换行,但每行以*字符开始,最后一行以*/结尾
" syn match   logComment '/\*.*\n\(\s*\*.*\n\)\{0,20\}\s*\*/' contains=logTodo,logError
""" 注释//类型的单行
" syn match   logComment '//[^@]*$' contains=logTodo,logError

"logTitle: Lines start with digit and '.'
"标题文本: 前面有任意个空格,数字.[数字.]打头, 并且该行里不含有,.。，等标点符号
" syn match logTitle "^\s*\(\d\+ \)\+\s*[^,。，]\+$"
" syn match logTitle "^\(\d\+\.\)\+\s*[^,。，]\+$" contains=logError,logTodo,logUrl,logReference
" syn match logTitle "^\(\d\+\.\)\+\s*[^,。，]\+[,;]" contains=logError,logTodo,logUrl,logReference

"logTitle: Lines start with Chinese digit and '.'
"标题文本: 汉字数字加'.、'打头，且该行不含,.。，标点符号
" syn match logTitle "^\s*\([一二三四五六七八九十][、.]\?\)\+\s*[^,。，;；]\?$"
" syn match logTitle "^\s*\([一二三四五六七八九十][、.]\?\)\+\s*[^,。，;；]*$"
" syn match logTitle "^\s*\([一二三四五六七八九十][、.]\?\)\+\s*[^,。，;；]\+[,，。]\?"
" syn match logTitle "^\s*\([一二三四五六七八九十][、. ]\)\+\s*[^,。，;；]*[,，。]\?"

"logTitle: Lines start with digit
"标题文本: 以数字打头, 中间有空格, 后跟任意文字. 且该行不含有,.。，标点符号
" syn match logTitle "^\d\s\+.\+\s*[^,。，]$"
" syn match logTitle "^\d\s\+.\+\s*[^,。，][$,]" contains=logError,logTodo,logUrl,logReference

" text block title: like
"
" +--------------+
" | text title   |
" +--------------+
" syn match txtBlockTt "^\s*\n+[-]\{5,60\}+\n|[^\n]*|\n+[-]\{5,60\}+$"
" syn match txtBlockTt "^\s*\n+[-]\{5,60\}+\n|[^\n]*|\n|[^\n]*|\n+[-]\{5,60\}+$"

"logList: Lines start with space and then '-+*.'
"列表文本: 任意空格打头, 后跟一个[-+*.]
" syn match logList    '^\s*\zs[-+*.] [^ ]'me=e-1
" syn match logList    '^\s*\zs[-+*.]\s[^ ]'me=e-1

"logList: Lines start with space and then digit
"列表文本: 任意空格打头, 后跟一个(数字) 或 (字母) 打头的文本行
" syn match logList    '^\s*\zs(\=\([0-9]\+\|[a-zA-Z]\))'

"logList: Lines start with space and then digit and '.'
"列表文本: 至少一个空格打头, [数字.]打头, 但随后不能跟数字(排除把5.5这样的文
"本当成列表) 
" syn match logList		"^\s\+\zs\d\+\.\d\@!"

" 以空格开头接$或#的命令
" syn match logCmd	'^\s\{1,2\}[#$].*$'
" syn match logCmd	'^\s\?\[[^\n]\{1,30\}\][ ]\?[#$].*$'

"logApostrophe: text in the apostrophe
"单引号内文字, 作用范围最多两行.
" syn match   logApostrophe  '\(\n\|[^a-zA-Z]\)\'[^\']\+\(\n\)\=[^\']\+\'\(\n\|[^a-zA-Z]\)' contains=logUrl,logReference,logError,logTodo

"logQuotes: text in the quotoes
"双引号内文字, 包括全角半角, 作用范围最多两行
" syn match   logQuotes     '["“][^"”]\+\(\n\)\=[^"”]*["”]' contains=logUrl,logReference
syn match   logQuotes     '["][^\n"][^"]*\(\n\)\=[^"]*["]' contains=logUrl,logReference,logError,logTodo
syn match   logQuotes     '[“][^\n”][^”]*\(\n\)\=[^”]*[”]' contains=logUrl,logReference,logError,logTodo
syn match   logQuotes     '['][^\n'][^']*\(\n\)\=[^']*[']' contains=logUrl,logReference,logError,logTodo

"logParentesis: text in the parentesis
"括号内文字, 不在行首(为了和logList区别), 作用范围最多两行
" syn match   logParentesis "[(（][^)）]\+\(\n\)\=[^)）]*[)）]" contains=logUrl,logReference,logError,logTodo

"logBrackets: text in the brackets
"其它括号内文字, 作用范围最多两行, 大括号无行数限制
" syn match logBrackets     '<[^<]\+\(\n\)\=[^<]*>' contains=logUrl,logReference,logError,logTodo
" syn match logBrackets     '\[[^\[]\+\(\n\)\=[^\[]*\]' contains=logUrl,logReference
" syn region logBrackets    matchgroup=logOperator start="{"  end="}" contains=logUrl,logReference
syn match logBrackets     '{[^{]\+\(\n\)\=[^{]*}' contains=logUrl,logReference,logError,logTodo

"link url
" syn match logUrl	'\<[A-Za-z0-9_.-]\+@\([A-Za-z0-9_-]\+\.\)\+[A-Za-z]\{2,4}\>\(?[A-Za-z0-9%&=+.,@*_-]\+\)\='
syn match logUrl	'\(\<mailto\>:\|\)\<[A-Za-z0-9_.-]\+@\([A-Za-z0-9_-]\+\.\)\+[A-Za-z]\{2,4}\>\(?[A-Za-z0-9%&=+.,@*_-]\+\)\='
syn match logUrl	'\<\(\(https\=\|ftp\|news\|telnet\|gopher\|wais\)://\([A-Za-z0-9._-]\+\(:[^ @]*\)\=@\)\=\|\(www[23]\=\.\|ftp\.\)\)[A-Za-z0-9%._/~:,=$@-]\+\>/*\(?[A-Za-z0-9/%&=+.,@*_-]\+\)\=\(#[A-Za-z0-9%._-]\+\)\='

"email text:
" syn match logEmailMsg '^\s*\(From\|De\|Sent\|To\|Para\|Date\|Data\|Assunto\|Subject\):.*'
" syn match logEmailMsg '^\s*\(From\|De\|Sent\|To\|Para\|Date\|Data\|Assunto\|Subject\):.*'
"reference from reply email, quotes, etc.
" syn match logReference '^[|>:]\(\s*[|>:]\)*'

" table and figure
" syn match logFig '--regex-log=/^[ \t ]+(table[ \t ]+[0-9a-zA-Z]+([.: ]([ \t ]*.+)?)?$)/\1/t,tables/i'
" syn match logFig '^\s*\(\<[tT]able\>\|\<[fF]igure\>\|\<[fF]ig\>\)\s*[A-Za-z0-9]*[\.|:].*$'
" syn match logFig '^\s*\(表格\|表\|图片\|图\|插图\)\s*[A-Za-z0-9]*[\、|：].*$'

"" log number
syn match logNum	' \<[0-9.:/%_-]\{,7\}\>'
syn match logNum	' \<[0-9.:/%_-]\{,7\}[.:/%_-][0-9.:/%_-]\{,7\}\>'
syn match logNumX	'\<0x[0-9a-f]\+\>'
syn match logNumX	'\<[0-9a-f]\{8,16\}\>'
syn match logNumX	'\<[0-9]\{8,16\}\>'

""" log dir
" syn match logDir	' \(/[a-zA-Z][a-zA-Z0-9:_-]*\)\{1,10\}'
syn match logDir	' \(/[a-zA-Z0-9._][a-zA-Z0-9:._-]*\)\{1,10\}'

"" Android log
" syn match AndroidLOGE '^E/[^:$]\{3,36\}:'
" syn match AndroidLOGW '^W/[^:$]\{3,36\}:'
" syn match AndroidLOGD '^D/[^:$]\{3,36\}:'
" syn match AndroidLOGI '^I/[^:$]\{3,36\}:'
" syn match AndroidLOGV '^V/[^:$]\{3,36\}:'
syn match AndroidLOGE '^E/'
syn match AndroidLOGW '^W/'
syn match AndroidLOGD '^D/'
syn match AndroidLOGI '^I/'
syn match AndroidLOGV '^V/'
syn match AndroidLOGE '^[0-9]\([0-9:.-]*[ ]*\)\{2,4\}E[ /]' contains=logNum
syn match AndroidLOGW '^[0-9]\([0-9:.-]*[ ]*\)\{2,4\}W[ /]' contains=logNum
syn match AndroidLOGD '^[0-9]\([0-9:.-]*[ ]*\)\{2,4\}D[ /]' contains=logNum
syn match AndroidLOGI '^[0-9]\([0-9:.-]*[ ]*\)\{2,4\}I[ /]' contains=logNum
syn match AndroidLOGV '^[0-9]\([0-9:.-]*[ ]*\)\{2,4\}V[ /]' contains=logNum

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"类html文本
"syn match logBold		'\*[^*[:blank:]].\{-}\*'hs=s+1,he=e-1
"syn match logItalic	'^\s\+.\+$' "斜体文本

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" color definitions (specific)
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"" color define for android log
hi link AndroidLOGE  ErrorMsg
hi link AndroidLOGW  Todo
" hi link AndroidLOGD  String
" hi link AndroidLOGI  Comment
" hi link AndroidLOGV ErrorMsg
hi link AndroidOK   MoreMsg
hi link AndroidNO   String

"hi logUrl       term=bold    cterm=bold  ctermfg=blue    gui=underline     guifg=blue
"hi logTitle     term=bold    cterm=bold  ctermfg=black   gui=bold        guifg=black
hi link logUrl			Underlined"Tabline"PmenuSbar
" hi link logTitle        Comment"ModeMsg"Special"Tabline"PmenuSbar
" hi link logBlockTt      Comment"ModeMsg"Special"Tabline"PmenuSbar
" hi link logList         SignColumn"Pmenu"DiffText"Statement
" hi link logCmd          Pmenu"DiffText"Statement
" hi link logComment      Special"MoreMsg
" hi link logReference    DiffAdd"Comment
hi link logQuotes       Special"Comment
" hi link logApostrophe   MoreMsg"Special
" hi link logParentesis   MoreMsg"String
hi link logBrackets     Character
hi link logError        ErrorMsg
hi link logTodo         Todo
hi link logDoing        MoreMsg
" hi link logEmailMsg     ModeMsg
" hi link logFig          Comment"ModeMsg
hi link logNum          Comment
hi link logNumX         String
hi link logDir          Comment

let b:current_syntax = 'log'
let &cpo = s:cpo_save
unlet s:cpo_save
" vim:tw=0:et
