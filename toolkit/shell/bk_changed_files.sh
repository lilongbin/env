#! /bin/bash
# use this scripts can find out and backup
# the files modified in ndays, and copy them
# to backup_dir then compress to .tgz file
# created by longbin <beangr@163.com>
# v1.0 <2015-01-07>
# v1.1 <2015-01-08>
# v1.2 <2015-01-09>

# -n modified_day - today < n days
# n  modified_day - today = n days
# +n modified_day - today > n days
ndays=-1
tmp_file_name=_findfiles_tmp.txt
backup_pkg=changed_files
backup_dir="__modified_${USER}"
cur_dir=$(pwd)

function find_files_changed()
{
	local ndays=$*
	if [ "$(echo ${ndays}| grep '[^0-9-]')" != "" ] ;then
		echo "ERROR: ${ndays}: Invalid parameter, should be Non-negtive integer."
		exit
	fi

	## find out files which modified in ndays, exclude backup_dir
	## find out files was last modified less than ndays.
	echo "find files modified in ${ndays} * 24 hours ..."
	find . -name "${backup_dir}" -prune -o -mtime "${ndays}" \
			 > ${tmp_file_name}
			 # | sed -n "s%^\.\/% %p" \
	
	## delete lines .
	sed -i '/^\.$/d'   ${tmp_file_name}
	## delete lines ./
	sed -i '/^\.\/$/d' ${tmp_file_name}
	## delete lines begin with ./__backup
	sed -i "/^\.\/${backup_dir}/d"  ${tmp_file_name}
	## delete lines begin with __backup
	sed -i "/${backup_dir}/d"       ${tmp_file_name}
	sed -i "/${tmp_file_name}/d"    ${tmp_file_name}
   	## delete ${backup_pkg}.tgz
	sed -i "/${backup_pkg}\.tgz/d" ${tmp_file_name}
	
	sed -i "/\.o$/d"      ${tmp_file_name}
	sed -i "/\.ko$/d"     ${tmp_file_name}
	sed -i "/\.a$/d"      ${tmp_file_name}
	sed -i "/\.dep$/d"    ${tmp_file_name}
	sed -i "/\.depend$/d" ${tmp_file_name}
	sed -i "/\.map$/d"    ${tmp_file_name}
	sed -i "/\.bin$/d"    ${tmp_file_name}
	sed -i "/cscope/d"    ${tmp_file_name}
	sed -i "/tags$/d"     ${tmp_file_name}
	
	## delete dir and link file name from ${tmp_file_name}
	local FILE=$(sed -n '1,$'p  ${tmp_file_name})
	line_nu=1
	for rm_line in ${FILE}
	do
		# echo "${rm_line}"
		if [ ! -f ${rm_line} ] ;then
			echo "delete line: ${rm_line}"
			sed -i "${line_nu}d" ${tmp_file_name}
			((line_nu-=1))
		fi
		((line_nu+=1))
	done

	## get total lines of file
	echo "find "0"$(sed -n '$=' ${tmp_file_name}) files changed."
}


function backup_find_files()
{
	## get file's data as array
	local FILE=$(sed -n '1,$'p ${tmp_file_name})
   	## display file's data
	#echo $FILE
	if [ "$FILE" = ""  ]; then
		echo "no changed files was found to backup."
		return
	fi

	read -p "press <ENTER> to backup the new files."

	## backup files ref file's data
	[ -d ${backup_dir} ] || mkdir ${backup_dir}

	count=0
	for file in ${FILE}
	do
		## get file's directory
		cp_file_dir=${file%/*}
		## get file's name
		cp_file_name=${file##*/}
		if [ -d ${cp_file_dir} ] ; then
			[ -d ${backup_dir}/${cp_file_dir} ] \
				|| mkdir -p ${backup_dir}/${cp_file_dir}
			echo "cp ${file} ${backup_dir}/${cp_file_dir}/${cp_file_name}"
			cp -fdu ${file} ${backup_dir}/${cp_file_dir} && ((count+=1))
		else
			echo "cp ${file} ${backup_dir}/${file}"
			cp -fdu ${file} ${backup_dir} && ((count+=1))
		fi
	done
	echo "${count} files backed up."
}

function compress_backup_dir()
{
	read -p "compress backed up files ?<Y/n> " select
	if [ "${select/N/n}" != "n" ];	then
		tar -zcvf ${backup_pkg}.tgz ${backup_dir}
		echo "bachup files to ${backup_pkg}.tgz finished."
	fi
}

function clean_tmp_file()
{
	[ -f ${tmp_file_name} ] && rm -f ${tmp_file_name}
}

find_files_changed ${ndays}
backup_find_files
compress_backup_dir
clean_tmp_file

