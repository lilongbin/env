#! /bin/bash
## this scripts is used to make un-sparse image files,
## then could copy the generated files to Windows and
## to make DataIO images
## created by longbin <beangr@163.com>
## 2015-01-24
## 2015-01-27
## 2015-03-06

#######################################################################
#### Global definition segment ########################################

## please define the "EMMC_IMAGES" and "ANDROID_DIR" directory name correctly
## in the following lines.

## define the route of project and image file directories
EMMC_IMAGES=emmc_images
PROJECT_DIR=tc75-factory
PREBURN_DIR=preburn_image-$(date +%Y%m%d)

ANDROID_DIR=${PROJECT_DIR}/LINUX/android
## define the route of un-sparse image source directory
RAW_IMG_OUT_DIR=${ANDROID_DIR}/out/target/product/TC75
VMLINUX_FILE=${RAW_IMG_OUT_DIR}/obj/KERNEL_OBJ/vmlinux
ESSENTIAL_FILES="emmc_appsboot.mbn MPRG8960.hex NON-HLOS.bin rpm.mbn sbl1.mbn sbl2.mbn sbl3.mbn tz.mbn"
LOGO_AND_ENV_BIN="logo1.bin logo2.bin env.bin"

## define the whole route of rawprogram0.xml and path0.xml file in emmc_images
RAW_XML=${EMMC_IMAGES}/rawprogram0.xml
PATCH0_XML=${EMMC_IMAGES}/patch0.xml
## define the whole route of target file in preburn iamge directory
RAW_TEST_XML=${PREBURN_DIR}/rawprogram0.xml

## define un-sparse image type
UN_SPARSE_IMGS="system userdata persist cache"
UN_SPARSE_IMG_TAIL="_test"

## define the size of un-sparse images
## system 1024M
let SYSTEM_SIZE=1024*1024*1024
## userdata 4928M - 80K -512M
let USERDATA_SIZE=1024*1024*4928-1024*80-1024*1024*512
## persist 8M
let PERSIST_SIZE=1024*1024*8
## cache 256M
let CACHE_SIZE=1024*1024*256
#######################################################################

## backup the main directory
MAIN_DIR=$(pwd)
OLD_EMMC_IMAGES=${EMMC_IMAGES}
OLD_PROJECT_DIR=${PROJECT_DIR}
######################

## get_current_script_file_name function can get current
## shell script's filename and return back
function get_current_script_file_name(){
	## get current script PID
	CUR_PID=$$
	## echo "current PID is ${CUR_PID}"
	## get current script file name
	local cs_file_name=`ps -ef | grep ${CUR_PID} \
			  | sed -n "/${USER}[ ]*${CUR_PID}/p" \
			  | awk '{ match($0, /[^ ]*$/)
				  print substr($0, RSTART, RLENGTH)
			  }'`
	## check current script file's mode
	if [[ -f ${cs_file_name} ]] && [[ -w ${cs_file_name} ]] ;then
		echo "${cs_file_name}"
	fi
}

## update_script_variable_to_value function can update a definition in a file
## this function has 3 options/arguments
## -f filename   :filename is in what you want to update definition
## -t definition :definition is the target you want to change
## -v value      :value is the value of the target will update to
function update_script_variable_to_value(){
	## get options and variables
	local OPTIND
	while getopts "f:t:v:" func_opt
	do
		case "${func_opt}" in
		f)
			target_file=${OPTARG}
			# echo "target filename is: ${target_file}"
			;;
		t)
			target_variable=${OPTARG}
			# echo "target variable is: ${target_variable}"
			;;
		v)
			target_value=${OPTARG}
			# echo "target value is: ${target_value}"
			;;
		?)
			echo "unknown option and argument."
			exit 1
			;;
		esac
	done

#	echo "there are $# arguments"
#	echo "All arguments are: $*"

	## check options and values
	if [[ "${target_file}" == "" ]] ;then
		echo "target filename is: ${target_file}"
		echo "get invalid argument: target_file."
		exit 1
	fi
	if [[ "${target_variable}" == "" ]] ;then
		echo "target variable is: ${target_variable}"
		echo "get invalid argument: target_variable."
		exit 1
	fi
	if [[ "${target_value}" == "" ]] ;then
		echo "target value is: ${target_value}"
		echo "get invalid argument: target_value."
		exit 1
	fi
	if ! [[ -f ${target_file} ]] || ! [[ -w ${target_file} ]] ;then
		echo "No such file, or have no permission"
		exit 1
	fi

	## replace target_variable -> target_value in target_file
	## the variable can't include any blank
	## and the variable should be defined in one line.
	sed -i "/^[\t ]*${target_variable}=[^ ]*$/ s%${target_variable}=[^ ]*$%${target_variable}=${target_value}%g" ${target_file}
	echo "${target_variable} updated to ${target_value} in \"${target_file}\"."
	return
}
######################

## check file naming availability
## to avoid invalid data
function check_file_naming_availability(){
	## availability file name regexp: /^[a-zA-Z0-9()_-]*$/
	local res_val=`echo $1 | sed -n "/^[a-zA-Z0-9./_-]*$/p"`
	# echo ${res_val}
	if [[ "${res_val}" == "" ]] ;then
		echo "Invalid parameter."
		exit 1
	fi
}

## check the "EMMC_IMAGES" and "ANDROID_DIR" directory
## which defined above whether exist.
function check_project_android_dir(){
	## check PROJECT_DIR directory
	if [[ -d ${PROJECT_DIR} ]] ;then
		echo "[project directory] is: ${PROJECT_DIR} "

		## check android directory
		if [[ -d "${ANDROID_DIR}" ]] ;then
			echo "[android directory] is: ${ANDROID_DIR} "
		else
			echo "android directory \"${ANDROID_DIR}\" no exists."
			exit 1
		fi
	else
		echo "project directory \"${PROJECT_DIR}\" error, No such directory"
		let TRIED_AGAIN_NUM+=1
		if [[ ${TRIED_AGAIN_NUM} -gt 3 ]] ;then
			let TRIED_AGAIN_NUM=0
			echo "you tried more than 3 times, exit"
			exit 1
		fi
		read -p "type your project directory: " PROJECT_DIR
		check_file_naming_availability ${PROJECT_DIR}
		if [[ ${PROJECT_DIR} == "" ]] ;then
			echo "type error, exit"
			exit 1
		else
			## update tmp variable
			ANDROID_DIR=${PROJECT_DIR}/LINUX/android
			RAW_IMG_OUT_DIR=${ANDROID_DIR}/out/target/product/TC75
			VMLINUX_FILE=${RAW_IMG_OUT_DIR}/obj/KERNEL_OBJ/vmlinux
			check_project_android_dir
		fi
	fi
}

## check emmc_images directory
function check_emmc_images_dir_exits(){
	if ! [[ -d ${EMMC_IMAGES} ]] ;then
		echo "emmc_images directory \"${EMMC_IMAGES}\" error, No such directory "
		let TRIED_AGAIN_NUM+=1
		if [[ ${TRIED_AGAIN_NUM} -gt 3 ]] ;then
			let TRIED_AGAIN_NUM=0
			echo "you tried more than 3 times, exit"
			exit 1
		fi
		read -p "type your project directory: " EMMC_IMAGES
		check_file_naming_availability ${EMMC_IMAGES}
		if [[ "${EMMC_IMAGES}" == "" ]] ;then
			echo "type error, exit"
			exit 1
		else
			## update tmp variable
			RAW_XML=${EMMC_IMAGES}/rawprogram0.xml
			PATCH0_XML=${EMMC_IMAGES}/patch0.xml
			## check emmc images directory again
			check_emmc_images_dir_exits
		fi
	else
		echo "[emmc_images directory] is: ${EMMC_IMAGES}"
	fi

}

## check out directory and emmc_images directory
function check_defined_dir_exists(){
	## get current script file name
	local cur_file_name=`get_current_script_file_name`
	echo "Current script file name is: ${cur_file_name}"

	echo -e "\nCurrent directory is:\n\t ${PWD}\n"
	## check project and android directory
	check_project_android_dir

	## check out directory
	if ! [[ -d ${RAW_IMG_OUT_DIR} ]] ;then
		echo "image directory ${RAW_IMG_OUT_DIR} not exists"
		exit 1
	else
		echo "[built image directory] is: ${RAW_IMG_OUT_DIR}"
	fi

	## update PROJECT_DIR to new one in current file
	if [[ "${OLD_PROJECT_DIR}" != "${PROJECT_DIR}" ]] ;then
		echo "old project directory is: ${OLD_PROJECT_DIR}"
		update_script_variable_to_value -f ${cur_file_name} -t PROJECT_DIR -v ${PROJECT_DIR}
		if [[ "$?" == "0" ]] ;then
			echo "updated project directory is: ${PROJECT_DIR}"
		else
			echo "updated project directory error"
			return
		fi
		## the PROJECT_DIR will automatically update by user's type
	fi

	## check emmc_images directory
	check_emmc_images_dir_exits
	if [[ "${OLD_EMMC_IMAGES}" != "${EMMC_IMAGES}" ]] ;then
		echo "old emmc_images directory is: ${OLD_EMMC_IMAGES}"
		update_script_variable_to_value -f ${cur_file_name} -t EMMC_IMAGES -v ${EMMC_IMAGES}
		if [[ "$?" == "0" ]] ;then
			echo "updated project directory is: ${EMMC_IMAGES}"
		else
			echo "updated project directory error"
			return
		fi
		## the PROJECT_DIR will automatically update by user's type
	fi
}

## verify preburn image directory
## if the directory not exists, create and use it;
## if the directory has already exists, notice user whether overwrite or not;
## if user's directory also exists, exit
function check_preburn_image_dir(){
	if [[ -d ${PREBURN_DIR} ]] ;then
		read -p "directory \"${PREBURN_DIR}\" has exists, overwrite <y/n>? " select
		if [[ "$select" != "y" ]] ;then
			read -p "please type a new directory name to collect image files: " PREBURN_DIR
			check_file_naming_availability ${PREBURN_DIR}
			if [[ "${PREBURN_DIR}" == "" ]] ;then
				echo "directory name can not be empty !"
				exit 1
			elif [[ -d "${PREBURN_DIR}" ]] ;then
				echo "directory \"${PREBURN_DIR}\" has also exists ."
				exit 1
			else
				mkdir -p ${PREBURN_DIR} && echo "mkdir ${PREBURN_DIR} ..."
			fi
		fi
	else
		echo "mkdir ${PREBURN_DIR} ..."
		mkdir -p ${PREBURN_DIR}
	fi
	
	## test whether target directory exists
	if [[ -d ${PREBURN_DIR} ]] && [[ -w ${PREBURN_DIR} ]] ;then
		echo "directory \"${PREBURN_DIR}\" checked OK."
	else
		echo "directory \"${PREBURN_DIR}\" not exists or have no permission to write."
		exit 1
	fi
}

function copy_logo_and_env_bin_to_preburn_dir(){
	## find and copy logo1.bin logo2.bin env.bin
	## find files depth==2
	echo "finding and copying files: ${LOGO_AND_ENV_BIN}"
	for file in ${LOGO_AND_ENV_BIN}
	do
		file_name=`find ${PREBURN_DIR} -maxdepth 2 -name ${file} |\
			sed -n "/${file}/p" | sed -n '1p' `
		if [[ "${file_name}" == "" ]] ;then
			file_name=`find ./ ${EMMC_IMAGES} -maxdepth 3 -name ${PREBURN_DIR} -prune -o -name ${file} |\
					  sed -n "/${file}/p" | sed -n '1p' `
		fi
		if [[ "${file_name##*/}" == "${file}" ]] ;then
			echo -e "\tfind out \"${file_name}\" "
			if ! [[ -f ${PREBURN_DIR}/${file} ]] ;then
				echo -e "\t${file_name}  ->  ${PREBURN_DIR}/${file_name##*/} ..."
				cp -f ${file_name} ${PREBURN_DIR}
			else
				echo -e "\t\"${file}\" exists in ${PREBURN_DIR}/, no need to copy."
			fi
		fi
	done

	lack_file=0
	for file in ${LOGO_AND_ENV_BIN}
	do
		if ! [[ -f ${PREBURN_DIR}/${file} ]] ; then
			echo -e "\tCouldn't find file \"${file}\"; \c "
			echo -e "Please copy \"${file}\" to dir \"${PREBURN_DIR}\"."
			lack_file=1
		fi
	done
	if [[ "${lack_file}" == "1" ]] ;then
		echo "ERR occurred, exit."
		exit 1
	fi
}

## test whether user's directory exists and copy image files
function copy_images_to_preburn_dir(){
	## define copy flag -u for update and -v for verbose
	# CP_FLAG="-uv"
	CP_FLAG="-v"
	echo "copying files ..."

	## copy file under emmc_images to preburn_image file
	if [[ -d ${EMMC_IMAGES} ]] ;then
		cp ${CP_FLAG} ${EMMC_IMAGES}/* ${PREBURN_DIR}
	fi
	
	## copy built images under out dir to preburn images directory
	## exclude system_test.img/userdata_test.img ...
	for img_file in `ls ${RAW_IMG_OUT_DIR}/*.img | sed 's/[ ]/\n/p' | sed "/${UN_SPARSE_IMG_TAIL}/d"`
	do
		[[ -d ${RAW_IMG_OUT_DIR} ]] && cp ${CP_FLAG} ${img_file} ${PREBURN_DIR}
	done

	## copy emmc_appsboot.mbn
	[[ -d ${RAW_IMG_OUT_DIR} ]] && cp ${CP_FLAG} ${RAW_IMG_OUT_DIR}/emmc*.mbn ${PREBURN_DIR}

	## copy vmlinux file
	[[ -f ${VMLINUX_FILE} ]] && cp ${CP_FLAG} ${VMLINUX_FILE} ${PREBURN_DIR}

	## test the completeness of files in ESSENTIAL_FILES list
	for file in ${ESSENTIAL_FILES}
	do
		if ! [[ -f ${PREBURN_DIR}/${file} ]] ;then
			let err_flag=1
			echo "ERR: \"${PREBURN_DIR}/${file}\" not exists"
		fi
	done

	if [[ "${err_flag}" == "1" ]] ;then
		echo "some essential files not exists, please check"
		exit 1
	else
		echo "copy files done."
	fi
}

function prepare_images_to_preburn_dir(){
	#######################################################################
	## copy images to preburn image directory
	check_defined_dir_exists
	check_preburn_image_dir
	read -p "Copy/Update image files to \"${PREBURN_DIR}\" <y/n>? " select
	if [[ ${select} == "y" ]] ;then
		copy_logo_and_env_bin_to_preburn_dir
		copy_images_to_preburn_dir
	else
		echo "refused by user. Exit"
		exit 1
	fi
	#######################################################################
}

## copy and edit rawprogram0.xml which defined the partition, label and image file
function copy_xml_file_to_preburn_and_modify(){
	## copy XML files
	if [[ -f ${RAW_XML} ]] ;then
		cp -v ${RAW_XML} ${RAW_TEST_XML}
	else
		echo "copy error, No such file: ${RAW_XML}"
		exit 1
	fi
	if [[ -f ${PATCH0_XML} ]] ;then
		cp -v ${PATCH0_XML} ${PREBURN_DIR}
	else
		echo "copy error, No such file: ${PATCH0_XML}"
		exit 1
	fi

	## edit rawprogram0.xml and path0.xml
	echo "editing ${RAW_TEST_XML} ..."
	echo -e "\treplace sparse=\"true\" -> sparse=\"false\" "
	sed -i "s%sparse=\"true\"%sparse=\"false\"%g" ${RAW_TEST_XML}

	## replace system.img to system_test.img etc.
	## define the source directory of un-sparse image

	if [[ "${UN_SPARSE_IMG_TAIL}" == "" ]] ;then
		echo "UN_SPARSE_IMG_TAIL can't be defined as empty."
		exit 1
	fi
	for us_img_name in ${UN_SPARSE_IMGS}
	do
		## rawprogram0.xml: system.img -> system_test.img
		sed -i "s%\"${us_img_name}.img\"%\"${us_img_name}${UN_SPARSE_IMG_TAIL}.img\"%g" ${RAW_TEST_XML}
		echo -e "\treplace ${us_img_name}.img -> ${us_img_name}${UN_SPARSE_IMG_TAIL}.img"
	done

	## add factory.img filename in rawprogram0.xml 
	## rawprogram0.xml: enterprise -> enterprise.img
	if [[ -f ${PREBURN_DIR}/enterprise.img ]] ;then
		sed -i "/label=\"enterprise\"/ s%filename=\"[^ ]*\"%filename=\"enterprise.img\"%g" ${RAW_TEST_XML}
		echo -e "\tset filename=\"enterprise.img\" "
	fi
	## rawprogram0.xml: factory -> factory.img
	if [[ -f ${PREBURN_DIR}/factory.img ]] ;then
		sed -i "/label=\"factory\"/ s%filename=\"[^ ]*\"%filename=\"factory.img\"%g" ${RAW_TEST_XML}
		echo -e "\tset filename=\"factory.img\" "
	fi
	## rawprogram0.xml: environment -> env.bin
	if [[ -f ${PREBURN_DIR}/env.bin ]] ;then
		sed -i "/label=\"environment\"/ s%filename=\"[^ ]*\"%filename=\"env.bin\"%g" ${RAW_TEST_XML}
		echo -e "\tset filename=\"env.bin\" "
	fi
	## rawprogram0.xml: logo1 -> logo1.bin
	if [[ -f ${PREBURN_DIR}/logo1.bin ]] ;then
		sed -i "/label=\"logo1\"/ s%filename=\"[^ ]*\"%filename=\"logo1.bin\"%g" ${RAW_TEST_XML}
		echo -e "\tset filename=\"logo1.bin\" "
	fi
	## rawprogram0.xml: logo2 -> logo2.bin
	if [[ -f ${PREBURN_DIR}/logo2.bin ]] ;then
		sed -i "/label=\"logo2\"/ s%filename=\"[^ ]*\"%filename=\"logo2.bin\"%g" ${RAW_TEST_XML}
		echo -e "\tset filename=\"logo2.bin\" "
	fi

	echo "copy and modify \"${RAW_TEST_XML}\" done. "
}

function deal_with_xml_file(){
	#######################################################################
	## deal with xml file
	read -p "copy ${RAW_XML} to ${RAW_TEST_XML} and modify it <y/n>? " select
	if [[ ${select} == "y" ]] ;then
		## copy xml file to preburn image directory and
		## check whether the image exists in preburn image directory
		## whose filename defined in xml 
		copy_xml_file_to_preburn_and_modify
	else
		echo "canceled by user, exit."
		exit 1
	fi
	#######################################################################
}

## make un-sparse image file
function check_and_setup_make_ext4fs_command(){
	## test whether make_ext4fs command exists
	make_ext4fs 2>/dev/null
	if [[ "$?" == "127" ]] ;then
		if [[ -f ${ANDROID_DIR}/build/envsetup.sh ]] ;then
			## setup environment so that we could use "make_ext4fs" command
			echo "setup make_ext4fs command ..."
			cd ${ANDROID_DIR}
			echo ${PWD}
			source build/envsetup.sh
			sleep 1
			choosecombo
			cd ${MAIN_DIR}
		else
			echo -e "\tmake_ext4fs: command not found"
			echo -e "\tplease execute following commands:"
			echo -e "\t +---------------------------------+"
			echo -e "\t |   cd ${ANDROID_DIR} "
			echo -e "\t |   source build/envsetup.sh      |"
			echo -e "\t |   choosecombo                   |"
			echo -e "\t +---------------------------------+"
			exit 1
		fi
	elif [[ "$?" != 0 ]] ;then
		echo "make_ext4fs command exists, please use it correctly."
	fi
}

function check_un_sparse_image_vs_xml(){
	## check whether rawprogram0.xml includes the records about 
	## un-sparse image file which are made by above steps
	echo "checking whether xml defined all un-sparse images ..."
	#sleep 1
	err_flag=0
	for file in `ls ${PREBURN_DIR}/*${UN_SPARSE_IMG_TAIL}.img`
	do
		if [[ $(sed -n "/${file##*/}/p" ${RAW_TEST_XML}) == "" ]] ;then
			echo "WARNING: ${RAW_TEST_XML} not defined ${file}"
			err_flag=1
		fi
	done

	if [[ "${err_flag}" == "1" ]] ;then
		echo -e "\n---------------Warning-------------------"
		echo "please check if ${RAW_TEST_XML} defined all un-sparse image filename"
		exit 1
	else
		echo "all un-sparse images have defined in \"${RAW_TEST_XML}\"."
	fi
}
	
function check_xml_vs_image_files(){
	## check whether image exists which named in rawprogram0.xml
	## gain the filename list in the file rawprogram0.xml
	echo "Verifying whether \"${RAW_TEST_XML}\" matches \"${PREBURN_DIR}/*.img\" ... "
	#sleep 1
	# CONTENT_FILENAMES=$(sed 's/[ ]/\n/g' ${RAW_TEST_XML} | \
			# sed -n '/filename=\"[^ ]\{1,\}\"/p' | \
			# cut -d "\"" -f 2 )
	CONTENT_FILENAMES=$(awk '{match($0, /filename=\"[^ ]*\"/)
			split(substr($0, RSTART, RLENGTH), arr, /\"/)
			if(arr[2]!="")
			print arr[2]
			}' ${RAW_TEST_XML})
	test_res_flag=0
	## test whether the image filename which define in rawprogram0.xml
	## exists in the preburn_image directory
	## echo "files:"
	## echo "${CONTENT_FILENAMES}"
	for test_file in ${CONTENT_FILENAMES}
	do
		# echo -e "\tVerifying \"${test_file}\" ...\n\c "
		if ! [[ -f ${PREBURN_DIR}/${test_file} ]] ;then 
			echo "ERR: \"${RAW_TEST_XML}\" defined \"${test_file}\", but No such file: ${PREBURN_DIR}/${test_file} "
			let test_res_flag=1
		else
			echo -e "\tVerifying OK. Exists \"${PREBURN_DIR}/${test_file}\" "
		fi
	done

	if [[ "${test_res_flag}" == "1" ]] ;then
		echo "Not all image files does match \"${RAW_TEST_XML}\" !"
		exit 1
	else
		echo -e "\tVerified over, no error."
	fi
}

function make_ext4fs_and_copy_to_preburn_image(){
	echo "make ext4 fs ..."

	check_and_setup_make_ext4fs_command

	## make ext4 fs for system/ data/ persist/ cache
	echo -e "\nmake_ext4fs ...\n"
	echo -e "\nmake_ext4fs: system${UN_SPARSE_IMG_TAIL}.img\n"
	make_ext4fs -l ${SYSTEM_SIZE} -a system ${RAW_IMG_OUT_DIR}/system${UN_SPARSE_IMG_TAIL}.img ${RAW_IMG_OUT_DIR}/system
	echo -e "\nmake_ext4fs: userdata${UN_SPARSE_IMG_TAIL}.img\n"
	make_ext4fs -l ${USERDATA_SIZE} -a userdata ${RAW_IMG_OUT_DIR}/userdata${UN_SPARSE_IMG_TAIL}.img ${RAW_IMG_OUT_DIR}/data
	echo -e "\nmake_ext4fs: persist${UN_SPARSE_IMG_TAIL}.img\n"
	make_ext4fs -l ${PERSIST_SIZE} -a persist ${RAW_IMG_OUT_DIR}/persist${UN_SPARSE_IMG_TAIL}.img ${RAW_IMG_OUT_DIR}/persist
	echo -e "\nmake_ext4fs: cache${UN_SPARSE_IMG_TAIL}.img\n"
	make_ext4fs -l ${CACHE_SIZE} -a cache ${RAW_IMG_OUT_DIR}/cache${UN_SPARSE_IMG_TAIL}.img ${RAW_IMG_OUT_DIR}/cache

	## copy un-sparse image files to preburn image directory
	if [[ "$?" == "0" ]] ;then
		echo -e "make_ext4fs finished .\ncopying image files ..."
		cp -v ${RAW_IMG_OUT_DIR}/*${UN_SPARSE_IMG_TAIL}.img ${PREBURN_DIR}
		echo "-------------------------------------"
		ls -lh ${PREBURN_DIR}/*${UN_SPARSE_IMG_TAIL}.img
		echo "-------------------------------------"
	fi

	check_un_sparse_image_vs_xml
	check_xml_vs_image_files
}

function make_un_sparse_image(){
	#######################################################################
	## make un-sparse images
	read -p "make ext4fs from \"${RAW_IMG_OUT_DIR}\" <y/n>? " select
	if [[ "${select}" == "y" ]] ;then
		echo "	system size:   $SYSTEM_SIZE"
		echo "	userdata size: $USERDATA_SIZE"
		echo "	persist size:  $PERSIST_SIZE"
		echo "	cache size:    $CACHE_SIZE"
	
		read -p "Are all the image sizes correct <y/n>? " select
		if [[ "${select}" == "y" ]] ;then
			make_ext4fs_and_copy_to_preburn_image
		else
			echo "Image size error, exit ." && exit 
		fi
	else
		echo "Refused by user, make ext4fs exit."
		exit 1
	fi
	#######################################################################
}

function generate_flash_img_shell_script(){
	flash_name=${PREBURN_DIR}/flash_img-by-${USER}.sh
	echo -e "generating flash image shell script for Linux ..."
	if ! [[ -f ${flash_name} ]] ;then
		## add script head
		echo -e "#! /bin/bash\n# This script is automatically generated in Ubuntu" > ${flash_name}
		echo -e "# Bourne-Again shell script, ASCII text executable" >> ${flash_name}
		echo -e "# $(date +%Y-%m-%d\ %H:%M:%S)" >> ${flash_name}
		## apend fastboot oem unlock
		sed -i "$ a \\\\nadb devices" ${flash_name}
		sed -i "$ a adb reboot bootloader" ${flash_name}
		sed -i "$ a fastboot oem unlock moto_pollux\n" ${flash_name}

		## generate shell script
		if [[ -f ${RAW_TEST_XML} ]]	;then
			awk '{match($0, /filename.*label=\"[^ ]*\"/)
				split(substr($0, RSTART, RLENGTH), arr, "\"")
					if (arr[2]!="")
						print ("fastboot flash "arr[4], arr[2])
			}'  ${RAW_TEST_XML} >> ${flash_name}
		fi

		## replace system_test.img -> system.img ...
		for file in ${UN_SPARSE_IMGS}
		do
			sed -i "/${file}${UN_SPARSE_IMG_TAIL}\.img/ s/${file}${UN_SPARSE_IMG_TAIL}\.img/${file}\.img/g" ${flash_name}
		done
		## modify shell script
		if [[ -f ${flash_name} ]] ;then
			## comment command include gpt partition
			sed -i "/^.*gpt.*\.bin.*$/ s/^/#/g" ${flash_name}
			## apend "fastboot reboot"
			sed -i "$ a \\\\nfastboot reboot\n" ${flash_name}
		fi

		## show message
		echo -e "\ngenerate shell script \"${flash_name}\" done.\n"
	else
		echo -e "WARNING: \"${flash_name}\" has already exist."
		read -p "Overwrite \"${flash_name}\" <y/n>? " select
		if [[ "${select}" == "y" ]] ;then
			rm -vf ${flash_name} && echo "remove old file \"${flash_name}\" done."
			generate_flash_img_shell_script
		else
			echo -e "\nNO operate update \"${flash_name}\" file.\n"
		fi
	fi
}

function pack_and_compress_preburn_images(){
	## to pack and compress the image files to zip file
	read -p "press <ENTER> to compress directory \"${PREBURN_DIR}\" to \"${PREBURN_DIR##*/}.zip\" file. "
	zip -r ${PREBURN_DIR##*/}.zip ${PREBURN_DIR} && echo "generate \"${PREBURN_DIR##*/}.zip\" finished." 
	if [[ "$?" == "0" ]] ;then
		echo "Now you can copy \"${PREBURN_DIR}(.zip)\" to windows system to Make DataIO format images."
	else
		[[ -f ${PREBURN_DIR##*/}.zip ]] && rm -v ${PREBURN_DIR##*/}.zip
		echo "generate \"${PREBURN_DIR##*/}.zip\" file failed."
		exit 1
	fi
}

## execution tracing
#set -x
#set +x
clear
prepare_images_to_preburn_dir
deal_with_xml_file
make_un_sparse_image
generate_flash_img_shell_script
pack_and_compress_preburn_images

echo "Make Preburn images Done, goodbye !"

