#! /bin/bash
## self unpackage script header
## created by longbin <beangr@163.com>
## 2016-05-16

##################################################
##################################################
## This script can generate a package file and
## create a script head and tail to extract itself.
## You can write INSTALL.sh which can finish your
## job, and insert into the final script.
TMP_PKG_FILE=.__XXXtmp_$(date +%Y%m%d%H%M%S)
INSERT_SCRIPT_FILE=INSTALL.sh

function show_usage(){
	echo "*************************************"
	echo "* Usage: bash $0 Target_File File_Or_Dir[List]"
	echo "* e.g.: bash $0 test.sh tmp/test.* "
	echo "*************************************"
}

DEST_FILE=$1
if [ -z "$*"  ] ; then
	echo Parameter is null.
	show_usage
	exit 1
fi
shift
if [ -z "$*"  ] ; then
	echo Parameter is null.
	show_usage
	exit 1
fi
if [ ! -f "${DEST_FILE}" ] && [ -e "${DEST_FILE}" ] ;then
	echo "Parameter invalid, ${DEST_FILE} is not a normal file."
	show_usage
	exit
fi

if [ -f "${DEST_FILE}" ] ; then
	read -p "${DEST_FILE} has already exists, delete <y/N> ? " ANS
	if [ "${ANS/Y/y}" = "y"  ] ; then
		rm -vf ${DEST_FILE} || exit 1
	else
		exit 1
	fi
fi

echo "Preparing to compress files ..."
tar -zcvf ${TMP_PKG_FILE}.tgz $*
if [ "$?" != "0" ] ;then
	if [ -f "${TMP_PKG_FILE}.tgz" ] ;then
		rm -vf ${TMP_PKG_FILE}.tgz
	fi
fi

function add_self_extract_header_comment(){
	cat > ${DEST_FILE} << EOF
#! /bin/bash
## self unpackage script header
## created by ${USER}
## $(date +%Y-%m-%d\ %H:%M:%S)

EOF
}

function add_self_extract_header_content(){
	## change $ --> \$
	cat >>${DEST_FILE} <<EOF
DST_DIR=\$HOME
read -p "Please type the dir you will extract to Or press <Enter> to use current dir: " DST_DIR
DST_DIR=\$(echo \${DST_DIR%/} | sed "/^~/ s#~#\${HOME}#")
CUR_DIR=\$(pwd)
TMP_PKG_FILE=.__XXXtmp_\$(date +%Y%m%d%H%M%S)

############################################################
############################################################
## restore package file
function restore_package_file(){
	local LINE_NUM=\$(cat \$0 | wc -l | awk '{print \$1}')
	local n=1
	local SCRIPT_LINE=0

	while [ \${n} -le \${LINE_NUM} ] ; do
		local RET_VAL=""
		RET_VAL=\$(sed -n "\${n} p" \$0 |sed -n '/^##_THIS_IS_SCRIPT_FILE_END_TAG_XXX\$/p')
		if [ "\${RET_VAL}" != "" ] ;then
			SCRIPT_LINE=\${n}
			# echo "Current script file has \${SCRIPT_LINE} lines."
			break
		fi
		let n+=1
	done

	if [ "\${SCRIPT_LINE}" != "0" ] ;then
		let SCRIPT_LINE+=1
		# tail -n +\${SCRIPT_LINE} \$0 > \${PKG_FILE}.tgz
		if [ "\${DST_DIR}" = "" ] ;then
			DST_DIR=.
		fi
		if ! [ -d "\${DST_DIR}" ] ;then
			mkdir -p \${DST_DIR}
			if [ "\$?" != "0" ] ;then
				echo "mkdir \${DST_DIR} ERROR."
				exit
			fi
		fi
		## restore package file
		echo "restoring files from line \${SCRIPT_LINE} ..."
		echo "tail -n +\${SCRIPT_LINE} \$0 |tar -zxv -C \${DST_DIR}"
		# tail -n +\${SCRIPT_LINE} \$0 > \${TMP_PKG_FILE}.tgz
		# tar -zxvf \${TMP_PKG_FILE}.tgz -C \${DST_DIR}
		tail -n +\${SCRIPT_LINE} \$0 |tar -zxv -C \${DST_DIR}
		if [ "\$?" = "0" ] ;then
			echo "Restore file finished."
			rm -f \${TMP_PKG_FILE}.tgz
		else
			echo "Restore file ERROR."
			exit
		fi
	else
		echo "No package file was found."
	fi
}
############################################################
############################################################

# restore_package_file

EOF
}

function insert_autorun_script_content_to_dest(){
	read -p "Please type the script to insert [${INSERT_SCRIPT_FILE}]: " select
	if [ "${select}" != "" ] ;then
		INSERT_SCRIPT_FILE=${select}
	fi
	if ! [ -f "${INSERT_SCRIPT_FILE}" && -r "${INSERT_SCRIPT_FILE}" ] ;then
		echo "${INSERT_SCRIPT_FILE} No such file or No permission to read."
		rm -f ${TMP_PKG_FILE}.tgz
		rm -f ${DEST_FILE}
		exit
		# return
	fi
#	local LINE_NUMBER=$(cat ${INSERT_SCRIPT_FILE} |wc -l |awk '{print $1}')
#	for (( cur_line=1; cur_line<=${LINE_NUMBER}; cur_line++ )) ;do
#		one_line=$(sed -n "${cur_line} p" ${INSERT_SCRIPT_FILE})
#		# echo ${cur_line}: $one_line
#		sed -i -e "\$ a ${one_line:=\ }" ${DEST_FILE}
#		sed -i -e '$ s/^[ ]*$//g' ${DEST_FILE}
#	done
#	echo "Insert ${INSERT_SCRIPT_FILE} (total ${LINE_NUMBER} lines) OK."
	cat ${INSERT_SCRIPT_FILE} >> ${DEST_FILE}
	echo "Insert ${INSERT_SCRIPT_FILE} OK."
	read -p "Press <Enter> to continue"
}

function create_self_extract_header_to_dest(){
	add_self_extract_header_comment
	add_self_extract_header_content
}


function append_self_extract_main_to_dest(){
	# add restore_package_file func
	# this is the self extract body
	read -p "Insert autorun SCRIPT to the dest file <y/n>? " select
	if [ "${select}" = "y" ] ;then
		echo -e "Insert the autorun script to dest file\c "
		read -p " before/after extract package operation <b/a>? " select
		if [ "${select}" = "a" ] ;then
			echo "restore_package_file" >> ${DEST_FILE}
			insert_autorun_script_content_to_dest
		elif [ "${select}" = "b" ] ;then
			insert_autorun_script_content_to_dest
			echo "restore_package_file" >> ${DEST_FILE}
		fi
	else
		echo "restore_package_file" >> ${DEST_FILE}
	fi
	echo "exit" >> ${DEST_FILE}
	echo "##_THIS_IS_SCRIPT_FILE_END_TAG_XXX" >> ${DEST_FILE}

	# append package file body
	cat ${TMP_PKG_FILE}.tgz >> ${DEST_FILE}
	rm -f ${TMP_PKG_FILE}.tgz
}

function change_self_extract_dest_file_mode(){
	chmod a+x ${DEST_FILE}
	echo "Done."
	ls -lh ${DEST_FILE}
}

create_self_extract_header_to_dest
append_self_extract_main_to_dest
change_self_extract_dest_file_mode
exit 0

##################################################
##################################################
