#! /bin/bash
# 
####################################################
# Author      : longbin
# Created date: 2019-08-22 13:26:06
####################################################

# define tools
cmd_strip=/mnt/ngi2-integration-4.4/sdk/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-strip
cmd_mkimg=/mnt/ngi2-integration-4.4/images/mkimage
cmd_fastboot=~/fastboot

function update_project_env()
{
	# define project_name
	local name=$*
	if [ "${name}" = "" ] ;then
		project_name=c1ux
	else
		project_name=${name}
	fi
	echo "update_project_env to ${name}"
	sleep 2

	# define file name
	m4_img=${project_name}_m4_img.out
	firmware_img=firmware.img
	firmware_md5=firmware.md5
	target_dir=~/share/GM_refreshpkg_${project_name}-fw
}

function build_m4()
{
	echo -e "\nmake ${project_name}_m4"
	make ${project_name}_m4
	if [ "$?" != "0" ] ;then
		echo "build failed."
		exit
	fi
	sleep 2
}

function mk_m4_img()
{
	local outfile=${project_name}_m4.out
	build_m4
	cp ./out/${project_name}_m4/bin/${project_name}_m4.out ${outfile}

	echo -e "\n${cmd_strip} ${outfile}"
	${cmd_strip} ${outfile}
	if [ "$?" != "0" ] ;then
		echo "error occured."
		exit
	fi

	echo -e "\nmkimage ${outfile} ${m4_img} ... "
	${cmd_mkimg} -A arm -C none -T filesystem -a 0xa0000000 -e 0xa0000000 -n "IPU1FW" -d ${outfile} ${m4_img}
	if [ "$?" != "0" ] ;then
		echo "failed: error occured."
		exit
	fi
	echo "mkimage ${outfile} ${m4_img} done."
}

function flash_m4()
{
	echo -e "\n${cmd_fastboot} flash firmware ${m4_img}"
	sudo ${cmd_fastboot} flash firmware ${m4_img}
}

function update_md5()
{
	local img_file=$1
	local md5_file=${img_file%.*}.md5
	local md5_val=""
	echo "md5sum ${img_file} ... "

	if [ "" = ${img_file} ] ;then
		echo "ERROR: parameter invalid."
		echo -e "Usage:\n\tupdate_md5 img_file"
		exit
	fi
	if [ ! -f ${img_file} ] ;then
		echo "ERROR: ${img_file}: No such file."
		echo -e "Usage:\n\tupdate_md5 img_file"
		exit
	fi

	md5_val=$(md5sum ${img_file} | awk '{print $1}')
	echo ${md5_val}
	echo ${md5_val} > ${md5_file}
	echo "md5sum ${img_file} done"
}

function mk_firmware_img()
{
	local dspbin=dsp_J6_NGI2.bin
	local dspimg=dspcode.img

	echo -e "\n##############create ${firmware_img}##############"
	if [ ! -f ${m4_img} ] ;then
		echo "ERROR: ${m4_img}: No such file."
		exit
	fi
	[ -f ${firmware_img} ] && rm ${firmware_img}

	# add ${m4_img} to ${firmware_img} head
	echo "add ${m4_img} to ${firmware_img} head ..."
	dd if=${m4_img} of=${firmware_img}
	[ -f ${dspimg} ] && rm ${dspimg}
	if [ ! -f ${dspbin} ] ;then
		echo "${dspbin}: No such file."
		exit
	fi
	echo "add ${m4_img} to ${firmware_img} head done."

	echo -e "\nmkimage ${dspbin} ${dspimg} ..."
	${cmd_mkimg} -A arm -C none -T filesystem -a 0xa0000000 -e 0xa0000000 -n "DSPCODE" -d ${dspbin} ${dspimg}
	if [ "$?" != "0" ] ;then
		echo "error occured."
		exit
	fi
	echo "mkimage ${dspbin} ${dspimg} done"

	# append ${dspimg} to ${firmware_img}
	echo "append ${dspimg} to ${firmware_img} ..."
	dd if=${dspimg} of=${firmware_img} bs=1K seek=8192
	if [ ! -f ${firmware_img} ] ;then
		echo "ERROR: ${firmware_img}: No such file."
		exit
	fi
	rm ${dspimg}
	echo "append ${dspimg} to ${firmware_img} done."

	# update_md5 to generate firmware.md5
	echo -e "\n##############create ${firmware_md5}##############"
	update_md5 ${firmware_img}
}

function cp_firmware_files()
{
	echo -e "\n##############copy ${firmware_img} to ${target_dir}##############"
	if [ ! -d ${target_dir} ] ;then
		echo "mkdir -p ${target_dir}"
		mkdir -p ${target_dir}
	fi
	if [ ! -d ${target_dir} ] ;then
		echo "${target_dir}: No permission to access"
		exit
	fi
	if [ -f ${firmware_img} ] ;then
		cp -v ${firmware_img} ${target_dir}
	fi
	if [ -f ${firmware_md5} ] ;then
		cp -v ${firmware_md5} ${target_dir}
	fi
}

update_project_env $*
mk_m4_img
mk_firmware_img
cp_firmware_files

