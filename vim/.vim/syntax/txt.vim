"Script_name:	txt.vim
"Author:		<guoyoooping@163.com> 2011/08/07
"Release:		1.3.5
"Description:	syntax for plain/text.
"Last Modified:	longbin <beangr@163.com> 2015-05-13
"Language:		text/plain :)
"Location:		$HOME/.vim/syntax or $VIMRUNTIME/syntax/
"Install_detail:
        "1. put this file in $HOME/.vim/syntax or $VIMRUNTIME/syntax/ 
        "2. Add the following line in your .vimrc:
        "syntax on "syntax highlighting on
        "filetype plugin on
        "au BufRead,BufNewFile *.txt setlocal ft=txt "syntax highlight for txt.vim 

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
syn keyword txtTodo todo fixme note debug comment notice warning caution cannot
syn keyword txtError error bug dropped fail failed failure null overflow wrong invalid exception dump dumped crash
syn keyword ccppType auto bool char const double enum float int long register short signed static struct typedef union unsigned void volatile class
" syn keyword ccppWord break case continue default do else extern for goto if return sizeof switch while private public protect


"txtComment: Lines that start with '#'
"以#号打头的行为注释文本
syn match   txtComment '^#.*$' contains=txtTodo

"""logComment: Lines that start with '/*' end with '*/'
""" 单行的情况
syn match   logComment '/\*[^\n/]*\*/' contains=logTodo,logError
syn match   logComment '/\*[^\*]*/*[^\*]*\*/' contains=logTodo,logError
""" 有多次换行不超过20行,但没有/的情况
syn match   logComment '/\*\([^/]*\n\)\{1,20\}.*\*/' contains=logTodo,logError
""" 共2行,第2行第一个可见字符为*,以*/做行的结尾
syn match   logComment '/\*.*\n\(\s*\*.*\).*\*/' contains=logTodo,logError
""" 有多次换行,但每行以*字符开始,最后以*/做行尾
syn match   logComment '/\*.*\n\(\s*\*.*\n\)\{0,20\}\s*\*.*\*/' contains=logTodo,logError
""" 有多次换行,但每行以*字符开始,最后一行以*/结尾
syn match   logComment '/\*.*\n\(\s*\*.*\n\)\{0,20\}\s*\*/' contains=logTodo,logError
" syn region  txtComment start=+/\*+ end=+\*/+ contains=txtError,txtTodo
""" 注释//类型的单行
syn match   txtComment '//[^@]*$'

"txtTitle: Lines start with digit and '.'
"标题文本: 前面有任意个空格,数字.[数字.]打头, 并且该行里不含有,.。，等标点符号
syn match txtTitle "^\s\{,4\}\(\d\+ \)\{1,4\}\s\{,5\}[^:,。，]\+$"
syn match txtTitle "^\(\d\+\.\)\{1,4\}\s\{,5\}[^:,。，]\+$"
syn match txtTitle "^\(\d\+\.\)\{1,4\}\s\{,5\}[^:,。，]\+[,;]"

"txtTitle: Lines start with Chinese digit and '.'
"标题文本: 汉字数字加'.、'打头，且该行不含,.。，标点符号
" syn match txtTitle "^\s*\([零一二三四五六七八九十][、.]\?\)\+\s*[^,。，;；]\?$"
" syn match txtTitle "^\s*\([零一二三四五六七八九十][、.]\?\)\+\s*[^,。，;；]*$"
" syn match txtTitle "^\s*\([零一二三四五六七八九十][、.]\?\)\+\s*[^,。，;；]\+[,，。]\?"
syn match txtTitle "^\s\{,4\}\([零一二三四五六七八九十][、. ]\)\+\s*[^,。，;；]*[,，。]\?"

"txtTitle: Lines start with digit
"标题文本: 以数字打头, 中间有空格, 后跟任意文字. 且该行不含有,.。，标点符号
syn match txtTitle "^\d\s\{1,5\}[^,。，]\+[$,]"

" text block title: like
"
" +--------------+
" | text title   |
" +--------------+
" syn match txtBlockTt "^\s*\n+[-]\{5,60\}+\n|[^\n]*|\n+[-]\{5,60\}+$"
" syn match txtBlockTt "^\s*\n+[-]\{5,60\}+\n|[^\n]*|\n|[^\n]*|\n+[-]\{5,60\}+$"

"txtList: Lines start with space and then '-+*.'
"列表文本: 任意空格打头, 后跟一个[-+*.]
syn match txtList    '^\s*\zs[-+*.]\s[^ ]'me=e-1

"txtList: Lines start with space and then digit
"列表文本: 任意空格打头, 后跟一个(数字) 或 (字母) 打头的文本行
syn match txtList    '^\s*\zs(\=\([0-9]\+\|[a-zA-Z]\))'

"txtList: Lines start with space and then digit and '.'
"列表文本: 至少一个空格打头, [数字.]打头, 但随后不能跟数字(排除把5.5这样的文
"本当成列表) 
syn match txtList	"^\s\+\zs\d\+\.\d\@!"

" 以空格开头接$或#的命令
syn match txtCmd	'^\s\{1,2\}[#$].*$'
syn match txtCmd	'^\s\?\[[^\n]\{1,30\}\][ ]\?[#$].*$'

"txtApostrophe: text in the apostrophe
"单引号内文字, 作用范围最多两行.
" syn match   txtApostrophe  '\(\n\|[^a-zA-Z]\)\'[^\']\+\(\n\)\=[^\']\+\'\(\n\|[^a-zA-Z]\)' contains=txtUrl,txtReference

"txtQuotes: text in the quotoes
"双引号内文字, 包括全角半角, 作用范围最多两行
" syn match   txtQuotes     '["“][^"”]\+\(\n\)\=[^"”]*["”]' contains=txtUrl,txtReference
syn match  txtQuotes     '["][^\n"][^"]*\(\n\)\=[^"]*["]' contains=txtUrl,txtReference
syn match  txtQuotes     '[“][^\n”][^”]*\(\n\)\=[^”]*[”]' contains=txtUrl,txtReference
syn match  txtQuotes     '['][^\n'][^']*\(\n\)\=[^']*[']' contains=txtUrl,txtReference

"txtParentesis: text in the parentesis
"括号内文字, 不在行首(为了和txtList区别), 作用范围最多两行
syn match  txtParentesis "[(（][^)）]\+\(\n\)\=[^)）]*[)）]" contains=txtUrl,txtReference

"txtBrackets: text in the brackets
"其它括号内文字, 作用范围最多两行, 大括号无行数限制
syn match txtBrackets     '<[^<]\+\(\n\)\=[^<]*>' contains=txtUrl,txtReference
syn match txtBrackets     '\[[^\[]\+\(\n\)\=[^\[]*\]' contains=txtUrl,txtReference
syn match txtBrackets     '{[^{]\+\(\n\)\=[^{]*}' contains=txtUrl,txtReference
"syn region txtBrackets    matchgroup=txtOperator start="{"   end="}" contains=txtUrl,txtReference

"link url
" syn match txtUrl '\<[A-Za-z0-9_.-]\+@\([A-Za-z0-9_-]\+\.\)\+[A-Za-z]\{2,4}\>\(?[A-Za-z0-9%&=+.,@*_-]\+\)\='
syn match txtUrl	'\(\<mailto\>:\|\)\<[A-Za-z0-9_.-]\+@\([A-Za-z0-9_-]\+\.\)\+[A-Za-z]\{2,4}\>\(?[A-Za-z0-9%&=+.,@*_-]\+\)\='
syn match txtUrl	'\<\(\(https\=\|ftp\|news\|telnet\|gopher\|wais\)://\([A-Za-z0-9._-]\+\(:[^ @]*\)\=@\)\=\|\(www[23]\=\.\|ftp\.\)\)[A-Za-z0-9%._/~:,=$@-]\+\>/*\(?[A-Za-z0-9/%&=+.,@*_-]\+\)\=\(#[A-Za-z0-9%._-]\+\)\='

"email text:
" syn match txtEmailMsg '^\s*\(From\|De\|Sent\|To\|Para\|Date\|Data\|Assunto\|Subject\):.*'
syn match txtEmailMsg	'^\s*\(From\|De\|Sent\|To\|Para\|Date\|Data\|Assunto\|Subject\):.*'
"reference from reply email, quotes, etc.
syn match txtReference	'^[|>:]\(\s*[|>:]\)*'

" table and figure
syn match txtFig	'^\s*\(\<[tT]able\>\|\<[fF]igure\>\|\<[fF]ig\>\)\s*[A-Za-z0-9]*[\.|:].*$'
syn match txtFig	'^\s*\(表格\|表\|图片\|图\|插图\)\s*[A-Za-z0-9]*[\、|：].*$'

"" log number
syn match logNum	' \<[0-9.:/%_-]\{,7\}\>'
syn match logNum	' \<[0-9.:/%_-]\{,7\}[.:/%_-][0-9.:/%_-]\{,7\}\>'
syn match logNumX	'\<0x[0-9a-f]\+\>'
syn match logNumX	'\<[0-9a-f]\{8,16\}\>'
syn match logNumX	'\<[0-9]\{8,16\}\>'

""" log dir
" syn match logDir	' \(/[a-zA-Z][a-zA-Z0-9:_-]*\)\{1,10\}'
syn match logDir	' \(/[a-zA-Z0-9._][a-zA-Z0-9:._-]*\)\{1,10\}'

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"类html文本
"syn match   txtBold		'\*[^*[:blank:]].\{-}\*'hs=s+1,he=e-1
"syn match	 txtItalic		'^\s\+.\+$' "斜体文本

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" color definitions (specific)
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"hi txtUrl      term=bold     cterm=bold   ctermfg=blue    gui=underline   guifg=blue
"hi txtTitle	term=bold     cterm=bold   ctermfg=black   gui=bold        guifg=black
" hi link txtUrl		  Underlined"ModeMsg"Tabline"PmenuSbar
hi link txtUrl			Underlined"Tabline"PmenuSbar
" hi link txtTitle        Title"ModeMsg"Tabline"PmenuSbar
hi link txtTitle		Comment"ModeMsg"Special"Tabline"PmenuSbar
hi link txtBlockTt		Comment"ModeMsg"Special"Tabline"PmenuSbar
hi link txtList         SignColumn"Pmenu"DiffText"Statement
hi link txtCmd			Statement"Pmenu"DiffText
" hi link txtComment      Special"Comment
hi link txtComment      Special"MoreMsg
hi link txtReference    DiffAdd"Comment
" hi link txtQuotes       MoreMsg"String
hi link txtQuotes       Special"Comment
hi link txtApostrophe   MoreMsg"Special
" hi link txtParentesis   Special"Comment
hi link txtParentesis   MoreMsg"String
" hi link txtBrackets     Special
hi link txtBrackets		Character
hi link txtError		ErrorMsg
hi link txtTodo			Todo
hi link ccppType        Type
" hi link ccppWord        String
" hi link txtEmailMsg     Structure
hi link txtEmailMsg     ModeMsg
hi link txtFig			Comment"ModeMsg
hi link logNum          Comment
hi link logNumX         String
hi link logDir          Comment
hi link logComment		Special"MoreMsg

let b:current_syntax = 'txt'
let &cpo = s:cpo_save
unlet s:cpo_save
" vim:tw=0:et
