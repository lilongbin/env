#! /bin/bash
# 
####################################################
# Author      : longbin
# Created date: 2019-08-02 10:20:14
####################################################

busybox_folder=.
rootfs=rootfs

function _create_rootfs_dir()
{
	local install_dir=${busybox_folder}/_install
	if [ ! -d ${install_dir} ] ;then
		echo "ERROR: ${install_dir}: No such directory."
		echo "Please execute 'make install' in busybox source directory first."
		exit 1
	fi
	if [ -d ${rootfs} ]; then
		sudo rm -rf ${rootfs}
	fi
	mkdir -p ${rootfs}

	cp -rfd ${install_dir}/*  ${rootfs}/
	pushd ${rootfs}
	for dir in proc sys dev etc tmp mnt lib lib64 etc/init.d etc/sysconfig home root var var/log
	do
		if [ ! -d ${dir} ] ;then
			echo "mkdir ${dir}"
			mkdir -p ${dir}
		fi
	done
	popd
}

function _create_etc_fstab()
{
	local fstab_file=${rootfs}/etc/fstab
	sudo touch /etc/fstab
	echo "#device		mount-point	type	options		dump	fsck order" >> ${fstab_file}
	# echo "proc		/proc		proc	defaults		0	0" >> ${fstab_file}
	echo "tmpfs		/tmp		tmpfs	defaults		0	0" >> ${fstab_file}
	# echo "sysfs		/sys		sysfs	defaults		0	0" >> ${fstab_file}
	echo "tmpfs		/dev		tmpfs	defaults		0	0" >> ${fstab_file}
	echo "var		/dev		tmpfs	defaults		0	0" >> ${fstab_file}
	echo "ramfs		/dev		ramfs	defaults		0	0" >> ${fstab_file}
	echo "debugfs		/sys/kernel/debug	debugfs		defaults	0	0" >> ${fstab_file}
}
 
function _create_etc_inittab()
{
	local inittab_file=${rootfs}/etc/inittab
	if [ -f ${inittab_file} ] ;then
		rm ${inittab_file}
	fi
	echo "# /etc/inittab" > ${inittab_file}
	echo "::sysinit:/etc/init.d/rcS" >> ${inittab_file}
	echo "console::askfirst:-/bin/sh" >> ${inittab_file}
	echo "::ctrlaltdel:/sbin/reboot" >> ${inittab_file}
	echo "::shutdown:/sbin/swapoff -a" >> ${inittab_file}
	echo "::shutdown:/bin/umount -a -r" >> ${inittab_file}
	echo "::restart:/sbin/init" >> ${inittab_file}
}

function _create_etc_initd_rcS()
{
	local rcS_file=${rootfs}/etc/init.d/rcS
	if [ -f ${rcS_file} ]; then
		rm ${rcS_file}
	fi
	echo "#! /bin/sh" > ${rcS_file}
	echo "PATH=/sbin:/bin:/usr/sbin:/usr/bin" >> ${rcS_file}
	echo "runlevel=S" >> ${rcS_file}
	echo "prevlevel=N" >> ${rcS_file}
	echo "umask 022" >> ${rcS_file}
	echo "export PATH runlevel prevlevel" >> ${rcS_file}
	echo "mount -a" >> ${rcS_file}
	echo "mkdir -p /dev/pts" >> ${rcS_file}
	echo "mount -t devpts devpts /dev/pts" >> ${rcS_file}
	echo "#mount -n -t usbfs none /proc/bus/usb" >> ${rcS_file}
	echo "echo /sbin/mdev > /proc/sys/kernel/hotplug" >> ${rcS_file}
	echo "mdev -s" >> ${rcS_file}
	echo "mkdir -p /var/lock" >> ${rcS_file}
	echo "ifconfig lo 127.0.0.1" >> ${rcS_file}
	echo "ifconfig eth0 192.168.1.20" >> ${rcS_file}
	echo "/bin/hostname -F /etc/sysconfig/HOSTNAME" >> ${rcS_file}
	echo "mount -t proc none /proc" >> ${rcS_file}
	echo "mount -t sysfs none /sys" >> ${rcS_file}
	echo "/sbin/mdev -s" >> ${rcS_file}
	echo "/sbin/syslogd &" >> ${rcS_file}
	echo "sleep 1"         >> ${rcS_file}
	echo "logger '##############init rcS ok##################'" >> ${rcS_file}
	chmod +x ${rcS_file}
}

function _create_etc_profile()
{
	local profile=${rootfs}/etc/profile
	echo 'USER="root"' >> ${profile}
	echo 'LOGNAME=$USER' >> ${profile}
	echo 'export HOSTNAME=`/bin/hostname`' >> ${profile}
	echo 'export USER=root' >> ${profile}
	echo 'export HOME=/root' >> ${profile}
	# echo 'export PS1="[$USER@$HOSTNAME \W]\# "' >> ${profile}
	echo 'export PS1="[$USER:\w]\# "' >> ${profile}
	echo 'PATH=/bin:/sbin:/usr/bin:/usr/sbin' >> ${profile}
	echo 'LD_LIBRARY_PATH=/lib64:/usr/lib64:$LD_LIBRARY_PATH' >> ${profile}
	echo 'export PATH LD_LIBRARY_PATH' >> ${profile}
	echo "logger '##############load profile ok###################'" >> ${profile}
}

function _create_etc_sysconfig_hostname()
{
	local hostname=${rootfs}/etc/sysconfig/HOSTNAME
	echo "x86_64" > ${hostname}
}

function _create_lib_dir()
{
	sudo cp -arf /lib/x86_64-linux-gnu/* ${rootfs}/lib64/
	sudo rm ${rootfs}/lib/*.a
	sudo strip ${rootfs}/lib/*
}

function _create_dev_node()
{
	mkdir -p ${rootfs}/dev/
	sudo mknod ${rootfs}/dev/tty1 c 4 1
	sudo mknod ${rootfs}/dev/tty2 c 4 2
	sudo mknod ${rootfs}/dev/tty3 c 4 3
	sudo mknod ${rootfs}/dev/tty4 c 4 4
	sudo mknod ${rootfs}/dev/console c 5 1
	sudo mknod ${rootfs}/dev/null c 1 3
}

function _create_rootfs_img()
{
	local rootfs_img=${busybox_folder}/rootfs.img
	if [ -f ${rootfs_img} ]; then
		rm ${rootfs_img}
	fi
	
	find . | cpio -o --format=newc > ${rootfs_img}
	echo "rootfs cpio img: ${rootfs_img}"
}

function _create_rootfs_ramdisk()
{
	local ramfs=${busybox_folder}/rootfs_ramdisk.gz
	echo "create ramdisk ..."
	sudo dd if=/dev/zero of=ramdisk bs=1M count=32
	sudo mkfs.ext4 ramdisk
	sudo mkdir -p tmpfs
	sudo mount -t ext4 -o loop ramdisk ./tmpfs/
	sudo cp -raf ${rootfs}/*  tmpfs/
	sudo umount tmpfs
	sudo gzip --best -c ramdisk > ${ramfs}
	sudo rm -f ramdisk
	sudo rm -r tmpfs
	echo "ramdisk: ${ramfs}"
}

function _usage()
{
	local info="
	Usage:\n
		please execute below comand in busybox directory.\n\t
		bash $0
	"
	echo -e ${info}
	exit 0
}

if [ "${1,,}" = "-h" -o "${1,,}" = "--help" ] ;then
	_usage
fi
_create_rootfs_dir
_create_etc_fstab
_create_etc_inittab
_create_etc_initd_rcS
_create_etc_profile
_create_etc_sysconfig_hostname
_create_lib_dir
_create_dev_node
_create_rootfs_img
_create_rootfs_ramdisk

