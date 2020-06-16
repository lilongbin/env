#! /bin/bash
# ~/.my_bash_alias
# we need add below sentence to ~/.bashrc
# [ -f ~/.my_bash_alias ] && . ~/.my_bash_alias

function __load_PS1_cmd_script_XXX()
{
	# short path
	export PS1='[\[\033[01;32m\]\w\[\033[00m\]]\$ '
	return
	read -p "use long(l) or short(s) path? <l/S>" user_select
	if [ "${user_select/L/l}" != "l" ] ;then
		# long path
		export PS1='[\[\033[01;32m\]\W\[\033[00m\]]\$ '
	else
		# short path
		export PS1='[\[\033[01;32m\]\w\[\033[00m\]]\$ '
	fi
}
__load_PS1_cmd_script_XXX

function cindex()
{
	if [ "$(which indent 2> /dev/null)" = "" ] ;then
		return
	fi
	indent -npro -kr -i8 -ts4 -sob -l80 -ss -ncs -br -ce -cdw -brs -brf -bap -cp1 -npsl $*
}

function pyhelp()
{
	local script=~/toolkit/bin/app/pyhelp.py
	if [ "" = "${script}" ] ;then
		return
	fi
	if [ -f ${script} ] ;then
		python ${script} $*
	fi
}

function ftpd()
{
	local script=~/toolkit/bin/ftpd/ftpd.py
	if [ "" = "${script}" ] ;then
		return
	fi
	if [ -f ${script} ] ;then
		python ${script} $*
	fi
}

function calculator()
{
	local script=~/toolkit/bin/app/calculator.py
	if [ "" = "${script}" ] ;then
		return
	fi
	if [ -f ${script} ] ;then
		python ${script} $*
	fi
}

function add_alias_for_cmd()
{
	local myfunctionname=add_myalias_to_bashrc
	if [ "$#" != '3' ]; then
		echo "Usage:"
		echo -e "\t ${myfunctionname} <alias_cmd> <script_shell> <script_file>"
		echo -e "\nexample:"
		echo -e "\t ${myfunctionname} tbox_prj bash tbox_prj.sh"
		return
	fi
	local target_cmd=$1
	local script_shell=$2
	local script_file=$3
	if [ "${script_file:0:1}" != '/' ] ;then
		script_file=$(pwd)/${script_file}
	fi
	if [ ! -f ${script_file} ] ;then
		echo "Warning: ${script_file}: No such file"
	fi
	local cmd_str="alias ${target_cmd}=\"${script_shell} ${script_file} \""
	local dst_file=~/.bashrc

	if [ "" = "${cmd_str}" ] ;then
		echo "ERROR: empty parameter."
		return
	fi

	touch ${dst_file}

	# check if the alias cmd already exists
	local line_num=$(sed -n '$=' ${dst_file})
	local n=1
	local script_line=0
	local ret=0
	while [ ${n} -le ${line_num} ]
	do
		ret=$(sed -n "${n} p" ${dst_file})
		if [ "${ret}" = "${cmd_str}" ] ;then
			script_line=${n}
			echo "Warning: the command already exists in ${dst_file}:${script_line}"
			return
		fi
		let n+=1
	done

	# add alias cmd to .bashrc file
	if [ -f ${dst_file} ] ;then
		echo -e "Add command to ${dst_file}:\r\n${cmd_str}"
		echo ${cmd_str} >> ${dst_file}
		echo -e "successfully"
	else
		echo "ERROR: ${dst_file}: No such file."
		return
	fi
}
