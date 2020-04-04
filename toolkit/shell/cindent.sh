#! /bin/bash
## /usr/src/kernels/2.6.32-504.el6.i686/scripts/Lindent
## Author:			longbin <beangr@163.com>
## Release Date:	2015-05-28
## Release Version:	1.18.823
## Last Modified:	2018-08-23
##

# 使用的indent参数 值 含义 
# --blank-lines-after-declarations  bad 变量声明后加空行  
# --blank-lines-after-procedures  bap 函数结束后加空行  
# --blank-lines-before-block-comments  bbb 块注释前加空行  
# --break-before-boolean-operator  bbo 较长的行，在逻辑运算符前分行 
# --blank-lines-after-commas  nbc 变量声明中，逗号分隔的变量不分行 
# --braces-after-if-line  bl "if"和"{"分做两行 
# --brace-indent 0  bli0  "{"不继续缩进 
# --braces-after-struct-decl-line  bls 定义结构，"struct"和"{"分行 
# --comment-indentationn  c33 语句后注释开始于行33 
# --declaration-comment-columnn  cd33 变量声明后注释开始于行33 
# --comment-delimiters-on-blank-lines  ncdb 不将单行注释变为块注释 
# --cuddle-do-while  ncdw  "do ---while"的"while"和其前面的"}"另起一行  
# --cuddle-else  nce "else"和其前面的"}"另起一行 
# --case-indentation 0  cli0 switch中的case语句缩进0个空格 
# --else-endif-columnn  cp33 #else, #endif后面的注释开始于行33 
# --space-after-cast  cs 在类型转换后面加空格  
# --line-comments-indentation n  d0 单行注释（不从1列开始的），不向左缩进 
# --break-function-decl-args  nbfda 关闭：函数的参数一个一行 
# --declaration-indentationn  di2 变量声明，变量开始于2行，即不必对齐 
# --format-first-column-comments  nfc1 不格式化起于第一行的注释 
# --format-all-comments  nfca 不开启全部格式化注释的开关 
# --honour-newlines  hnl  Prefer tobreak long lines at the position of newlines in the input. 
# --indent-leveln  i4 设置缩进多少字符，如果为tab的整数倍，用tab来缩进，否则用空格填充。 
# --parameter-indentationn  ip5 旧风格的函数定义中参数说明缩进5个空格 
# --line-length 75  l75  非注释行最长75 
# --continue-at-parentheses  lp 续行从上一行出现的括号开始 
# --space-after-procedure-calls  pcs 函数和"("之间插入一个空格 
# --space-after-parentheses  nprs 在"（"后"）"前不插入空格 
# --procnames-start-lines  psl 将函数名和返回类型放在两行定义 
# --space-after-for  saf  for后面有空格 
# --space-after-if  sai  if后面有空格 
# --space-after-while  saw while后面有空格  
# --start-left-side-of-comments  nsc 不在生成的块注释中加* 
# --swallow-optional-blank-lines  nsob 不去掉可添加的空行  
# --space-special-semicolon  nss 一行的for或while语句，在";"前不加空。 
# --tab-size  ts4 一个tab为4个空格（要能整除"-in"） 
# --use-tabs  ut  使用tab来缩进

KERNEL_STYLE_PARAM="-npro -kr -i8 -ts8 -sob -l80 -ss -ncs -cp1"
LINUX_STYLE_PARAM="-npro -nbad -bap -nbc -bbo -hnl -br -brs -c33 -cd33 -ncdb -ce -ci4 -cli0 -d0 -di1 -nfc1 -i8 -ip0 -l80 -lp -npcs -nprs -npsl -sai -saf -saw -ncs -nsc -sob -nfca -cp33 -ss -ts8 -il1"
GNU_STYLE_PARAM="-npro -nbad -bap -nbc -bbo -bl -bli2 -bls -ncdb -nce -cp1 -cs -di2 -ndj -nfc1 -nfca -hnl -i2 -ip5 -lp -pcs -nprs -psl -saf -sai -saw -nsc -nsob"
KR_STYLE_PARAM="-npro -nbad -bap -bbo -nbc -br -brs -c33 -cd33 -ncdb -ce -ci4 -cli0 -cp33 -cs -d0 -di1 -nfc1 -nfca -hnl -i4 -ip0 -l75 -lp -npcs -nprs -npsl -saf -sai -saw -nsc -nsob -nss"
VS_STYLE_PARAM="-npro -kr -i8 -ts8 -sob -l80 -ss -ncs -bl -bli0 -nce -bls -blf -bap -cp1 -npsl"
UC_STYLE_PARAM="-npro -kr -i8 -ts8 -sob -l80 -ss -ncs -br -ce -cdw -brs -brf -bap -cp1 -npsl"
MY_KR_STYLE_PARAM="-npro -nbad -bap -bbo -nbc -br -brs -c33 -cd33 -ncdb -ce -ci4 -cli0 -cp33 -cs -di1 -nfc1 -nfca -hnl -i4 -ip0 -l100 -lp -npcs -nprs -npsl -saf -sai -saw -nsc -nsob -nss -bl -bli0 -nce -ncdw -bad"
# DEFAULT_STYLE_PARAM=${UC_STYLE_PARAM}
DEFAULT_STYLE_PARAM=${MY_KR_STYLE_PARAM}

INDENT_PARAM="${DEFAULT_STYLE_PARAM}"
FILE_TAIL=kr
## function check whether indent exists
function check_which_indent()
{
	local ret_val=$(which indent 2>/dev/null)
	if [ "x${ret_val}" == "x" ] ;then
		echo "ERR (127): Please install indent"
		exit 127
	fi
}
check_which_indent

## function indent function usage
function indent_func_usage()
{
	cat << EOF
Usage:
	bash ${0##*/} input_file1.c [input_file2.c ...]

	You will get the modified file(s): input_file1.c

EOF
}

## function select indent style and parameters
function select_indent_style()
{
	echo "Please select indent style: "
	# TMOUT=5

	PS3="Type a index number[1]: "
	select option in \
		"KERNEL STYLE" \
		"LINUX STYLE" \
		"GNU STYLE" \
		"KR STYLE" \
		"VS STYLE" \
		"UC STYLE"
	do
		case ${REPLY} in
		1)
			INDENT_PARAM="${KERNEL_STYLE_PARAM}"
			FILE_TAIL=kernel
			;;
		2)
			INDENT_PARAM="${LINUX_STYLE_PARAM}"
			FILE_TAIL=linux
			;;
		3)
			INDENT_PARAM="${GNU_STYLE_PARAM}"
			FILE_TAIL=gnu
			;;
		4)
			INDENT_PARAM="${KR_STYLE_PARAM}"
			FILE_TAIL=kr
			;;
		5)
			INDENT_PARAM="${VS_STYLE_PARAM}"
			FILE_TAIL=vs
			;;
		6)
			INDENT_PARAM="${UC_STYLE_PARAM}"
			FILE_TAIL=uc
			;;
		*)
			echo "invalid option."
			;;
		esac

		if [ "x${INDENT_PARAM}" != "x" ] ;then
			break
		fi
	done
	INDENT_PARAM="${INDENT_PARAM:=${UC_STYLE_PARAM}}"
}

##function set indent parameters
function set_indent_parameters()
{
	echo "skip set_indent_parameters" && return
	read -p "Press <Enter> to continue or <y> to reset indent paramters: " select
	if [ "x${select}" == "xy" ] ;then
		select_indent_style
	fi

	INDENT_REV=$(indent --version)
	INDENT_REV_1=$(echo ${INDENT_REV} | cut -d' ' -f3 | cut -d'.' -f1)
	INDENT_REV_2=$(echo ${INDENT_REV} | cut -d' ' -f3 | cut -d'.' -f2)
	INDENT_REV_3=$(echo ${INDENT_REV} | cut -d' ' -f3 | cut -d'.' -f3)

	if [ ${INDENT_REV_1} -gt 2 ] ; then
		INDENT_PARAM="${INDENT_PARAM} -il0"
	elif [ ${INDENT_REV_1} -eq 2 ] ; then
		if [ ${INDENT_REV_2} -gt 2 ] ; then
			INDENT_PARAM="${INDENT_PARAM} -il0" 
		elif [ ${INDENT_REV_2} -eq 2 ] ; then
			if [ ${INDENT_REV_3} -ge 10 ] ; then
				INDENT_PARAM="${INDENT_PARAM} -il0"
			fi
		fi
	fi
	if [ "x${INDENT_PARAM}" == "x" ] ;then
		echo "Set indent parameter ERROR!"
		exit -1
	fi
}
function check_and_indent_file()
{
	local FILE_LIST=$*
	for file in ${FILE_LIST}
	do
		if ! [ "x${file##*.}" != "xc" -o "x${file##*.}" != "xC" ] ;then
			echo "${file}: Not a C program file"
			exit -1
		fi
		echo -e "Formatting ${file} ... \c "
		if [ -e "${file}" -a -w "${file}" ] ;then
			INDENT_RESULT_FILE=${file%.*}_${FILE_TAIL}_$(date +%Y%m%d%H%M%S).${file##*.}
			# echo "indent ${INDENT_PARAM} ${file} -o ${INDENT_RESULT_FILE}"
			indent ${INDENT_PARAM} ${file} -o ${INDENT_RESULT_FILE}
			mv ${INDENT_RESULT_FILE} ${file}
			echo "OK"
		else
			echo "${file}: No such file or No permission to write"
			exit -1
		fi
	done
}
function check_and_indent_to_stdout()
{
	local FILE_LIST=$*
	for file in ${FILE_LIST}
	do
		if ! [ "x${file##*.}" != "xc" -o "x${file##*.}" != "xC" ] ;then
			echo "${file}: Not a C program file"
			exit -1
		fi
		if [ -e "${file}" -a -r "${file}" ] ;then
			indent ${INDENT_PARAM} -st ${file}
		else
			echo "${file}: No such file or No permission to read"
			exit -1
		fi
	done
}

## indent file main function
function indent_file_main_func()
{
	local CFILES=$*
	if [ ${#} -lt 1 ] ;then
		echo "ERR: No input file(s)"
		indent_func_usage
		exit
	elif [ ${#} -ge 1 ] ;then
		set_indent_parameters
		read -p "Press <y> (modify original files) or <Enter> (to screen): " select
		if [ "x${select}" == "xy" -o "x${select}" == "xY" ] ;then
			check_and_indent_file ${CFILES}
		else
			check_and_indent_to_stdout ${CFILES}
		fi
		if [ $? -eq 0 ] ;then
			echo "Working done !"
		fi
	fi
}
## indent [INDENT_PARAM] inputfiles
## indent [INDENT_PARAM] inputfile -o outputfile
## indent [INDENT_PARAM] inputfile -st > outputfile

indent_file_main_func $*
