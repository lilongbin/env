#! /bin/bash
# 
####################################################
# Author      : longbin
# Created date: 2018-08-10 14:51:11
####################################################


function usage()
{
	echo "This tool can replace binary files with flash_tbox_img dir's for rootfs img."
	echo "Usage:"
	echo -e "\tbash $(basename ${0}) raw_rootfs_img_file"
	echo -e "eg:bash $(basename ${0}) ~/share/rootfs_dom0.img"
}

function update_rootfs()
{
	local img_file=$1
	local rootfs_img_mount_path=_tmp_rootfs_img_dir
	if [ "" = "${img_file}" ] ;then
		echo "ERROR: parameter invalid."
		usage
		exit
	fi
	if [ ! -f ${img_file} ] ;then
		echo "ERROR: ${img_file}: No such file."
		usage
		exit
	fi
	if [ ! -d ${rootfs_img_mount_path} ] ;then
		echo "mkdir -p ${rootfs_img_mount_path}"
		mkdir -p ${rootfs_img_mount_path}
		if [ "$?" != "0" ] ;then
			echo "ERROR: mkdir failed."
			exit
		fi
	fi

	local NEW_BINARY_DIR=flash_tbox_img

	echo "mount -o loop -t ext4 ${img_file} ${rootfs_img_mount_path}"
	sudo mount -o loop -t ext4 ${img_file} ${rootfs_img_mount_path}/
	if [ "$?" != "0" ] ;then
		echo "ERROR: mount failed."
		exit
	fi

	echo "sudo cp ${NEW_BINARY_DIR}/bin/* ${rootfs_img_mount_path}/usr/local/bin/"
	sudo cp ${NEW_BINARY_DIR}/bin/* ${rootfs_img_mount_path}/usr/local/bin/
	echo "sudo cp ${NEW_BINARY_DIR}/lib/* ${rootfs_img_mount_path}/usr/local/lib/"
	sudo cp ${NEW_BINARY_DIR}/lib/* ${rootfs_img_mount_path}/usr/local/lib/

	sleep 1
	echo "umount ${rootfs_img_mount_path}"
	sudo umount ${rootfs_img_mount_path}
	if [ "$?" != "0" ] ;then
		echo "ERROR: umount failed."
		exit
	fi

	if [ -d ${rootfs_img_mount_path} ] ;then
		echo "rmdir ${rootfs_img_mount_path}"
		rmdir ${rootfs_img_mount_path}
	fi
}

function update_md5()
{
	local img_file=$1
	local md5_file=${img_file%.*}.md5
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
	echo "md5sum ${img_file} ..."
	local md5_val=$(md5sum ${img_file} | awk '{print $1}')
	echo ${md5_val}
	echo ${md5_val}	> ${md5_file}
}

update_rootfs $*
update_md5 $1

