" bugreport.vim:	Utilities to browser android bugreport file.
" Release:			1.3.6
" Last modified:	longbin <beangr@163.com> 2015.05.12
" License:			GPL

" Line continuation used here
let s:cpo_save = &cpo
set cpo&vim

" ****************** Options *******************************************
" allow user to avoid loading this plugin and prevent loading twice                                                                     
if exists ("loaded_ftplugin_bugreport_vim")
     finish
endif
let loaded_ftplugin_bugreport_vim = 1

if !exists('bugreport_buffer_size_upper_lmt')
	""" if txt file size larger than 20MB, don't generate taglist
	let s:bugreport_buffer_size_upper_lmt = 1024 * 1024 * 20
endif
if getfsize(bufname("%")) >= s:bugreport_buffer_size_upper_lmt
	finish
endif

"When this file reload, only load TBrowser_Ctags_Cmd once.
if !exists('Tlist_Ctags_Cmd')
	echomsg 'TxtBrowser: Taglist(http://www.vim.org/scripts/script.php?script_id=273) ' .
				\ 'not found. Plugin is not loaded.'
	" Skip loading the plugin
	let loaded_taglist = 'no'
	let &cpo = s:cpo_save
	finish
endif
if !exists('TBrowser_Ctags_Cmd')
	let TBrowser_Ctags_Cmd = Tlist_Ctags_Cmd
endif

"Txt tag definition start.
let s:TBrowser_Config = ' --langdef=bugreport --langmap=bugreport:.bugreport '

"Title tag definition
let s:TBrowser_Config .= '--regex-bugreport="/^([-]{6})(.{3,20})(INFO)(.{3,50})([-]{6}$)/\2\3\4/I,INFO/" '
let s:TBrowser_Config .= '--regex-bugreport="/^([-]{6})(.{3,20})(LOG)(.{3,50})([-]{6}$)/\2\3\4/L,LOG/" '

"Pass parameters to taglist
let tlist_bugreport_settings = 'bugreport;I:INFO;L:LOG'
let Tlist_Ctags_Cmd = TBrowser_Ctags_Cmd . s:TBrowser_Config

" restore 'cpo'
let &cpo = s:cpo_save
unlet s:cpo_save

