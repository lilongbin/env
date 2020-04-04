#! /bin/bash
# 
####################################################
# Author      : longbin
# Created date: 2018-04-16 20:10:11
####################################################

function add_myalias_to_bashrc()
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

