#! /bin/bash
# 
####################################################
# Author      : longbin
# Created date: 2018-04-16 20:10:11
####################################################

# below variable should be set firstly
cur_dir=$(pwd)
SERVICE_SHELL_FILES="
${cur_dir}/dtags.sh
${cur_dir}/tshark.sh
${cur_dir}/bugreport.sh
${cur_dir}/logcat.sh
"

function add_srvcmd_to_bashrc()
{
	local srv_file=$1
	local dst_file=~/.bashrc
	if [ "" = "${srv_file}" ] ;then
		echo "ERROR: empty parameter."
		return
	fi
	local cmd_str="[ -f ${srv_file} ] && source ${srv_file}"

	touch ${dst_file}

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

	if [ -f ${dst_file} ] ;then
		echo -e "Add command to ${dst_file}:\r\n${cmd_str}"
		echo ${cmd_str} >> ${dst_file}
		echo -e "successfully"
	else
		echo "ERROR: ${dst_file}: No such file."
		return
	fi
}

function add_srv_cmd()
{
	local dst_file=$1
	if [ "" = "${dst_file}" ] ;then
		echo "ERROR: parameter can not be empty."
		return
	fi

	if [ -f ${dst_file} ] ;then
		add_srvcmd_to_bashrc ${dst_file}
	else
		echo "ERROR: ${dst_file}: No such file."
		return
	fi
}

function srv_install()
{
	local dst_file
	for dst_file in ${SERVICE_SHELL_FILES}
	do
		if [ -f ${dst_file} ] ;then
			add_srv_cmd    ${dst_file}
		else
			echo "ERROR: ${dst_file}: No such file."
		fi
	done
}

srv_install


