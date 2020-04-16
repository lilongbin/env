#! /bin/bash
# Author:			longbin <beangr@163.com>
# Created Date:		2014-06-24
# Last Modified:	2015-08-05
# this script is available for centos to configure embedded environment

#list the software need to be installed to the variable FILELIST
TOOLS_COMM="figlet ghex gimp nautilus-open-terminal shutter tree alien markdown "
TOOLS_EDITOR="vim vim-doc vim-scripts ctags cscope gawk indent gedit gedit-plugins glogg lnav "
TOOLS_PACKET="rar unrar zip unzip p7zip p7zip-full p7zip-plugins "
TOOLS_NETWORK="axel curl ssh openssl openssl-devel libssl-dev uget tftp-hpa tftpd-hpa nfs-kernel-server filezilla "
TOOLS_TERM="ckermit minicom putty "
TOOLS_PYTHON="build-dep python python-pip python-setuptools tkintertk-devel tk tcl tk-dev tcl-dev python-tk python3-tk python-dev python-numpy python-scipy opencv-python matplotlib python-wand "
TOOLS_EMBED="uml-utilities squashfs-tools libelf-dev libelf-devel elfutils-libelf-devel qemu libmagickwand-dev liblz4-tool ninja-build meson "
TOOLS_CODING="meld dos2unix unix2dos tofrodos subversion git-core gitk "
TOOLS_BUILD="build-essential libtool cmake automake flex bison gperf graphviz gnupg gettext mingw32 readline readline-devel libc6-dev libc++-dev lib32stdc++6 lib32bz2-1.0 lib32bz2-dev gcc gcc-c++ g++ g++-multilib zlib zlib-devel tshark libncurses5-dev libncursesw5-dev libjsoncpp-dev ccache "
TOOLS_AUDIO="audacity audacious "
TOOLS_MEDIA="ubuntu-restricted-addons ubuntu-restricted-extras mpv smplayer"
TOOLS_SNAP="snapd snapcraft "
TOOLS_MONO="mono-complete mono-addins-utils mono-devel gtk-sharp2 gnome-sharp2 "
TOOLS_QT_DEV="qt4-dev-tools qt4-doc qt4-qtconfig qt4-demos qt4-designer qtcreator "

TOOLS_CENTOS_EXTRA="ntlmaps net-tools wireshark nfs4-acl-tools nfs-utils nfs-utils-lib ncurses-devel ncurses "
TOOLS_UBUNTU_EXTRA="ntlmaps net-tools wireshark emacs usb-creator-gtk libjsoncpp-dev "

# albert
# sudo add-apt-repository ppa:noobslab/macbuntu
# audo apt install albert
BASIC_TOOLS="${TOOLS_COMM} ${TOOLS_EDITOR} ${TOOLS_PACKET} ${TOOLS_NETWORK} ${TOOLS_TERM}"
CODE_TOOLS="${TOOLS_PYTHON} ${TOOLS_EMBED} ${TOOLS_CODING} ${TOOLS_BUILD} ${TOOLS_QT_DEV}"

# for android env
BUILD_ANDROID_U12="git gnupg flex bison gperf python-markdown build-essential zip curl ia32-libs libc6-dev libncurses5-dev:i386 xsltproc x11proto-core-dev libx11-dev:i386 libreadline6-dev:i386 libgl1-mesa-dev g++-multilib mingw32 tofrodos libxml2-utils zlib1g-dev:i386 "
#libgl1-mesa-dri:i386 libgl1-mesa-glx:i386
BUILD_ANDROID_U14="git gnupg flex bison gperf python-markdown build-essential zip curl ia32-libs libc6-dev lib32ncurses5-dev lib32z1 x11proto-core-dev libx11-dev libreadline-gplv2-dev lib32z-dev libgl1-mesa-dev g++-multilib mingw32 tofrodos libxml2-utils xsltproc libxml-simple-perl zlib1g-dev "
BUILD_ANDROID_U16="lib32ncurse5s5 lib32z1 "
BUILD_ANDROID_U18=" "
BUILD_ANDROID_U20=" "

DOCKER_DEP="apt-transport-https ca-certificates curl software-properties-common gnupg-agent "
# curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
# sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"
# sudo bash -c "echo deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable > /etc/apt/sources.list.d/docker.list"

## apt-cache search opencv
# OPEN_CV="$(apt-cache search opencv | awk '{print $1}')"
# OPEN_CV="libgtk2.0-dev pkg-config"
## g++ `pkg-config opencv --cflags --libs` my_example.cpp -o my_example

## bison and flex is the analyzer of programmer and spell
## textinfo is a tool to read manual like man
## automake is used to help create Makefile
## libtool helps to deal with the dependency of libraries
## cvs, cvsd and subversion are used to control version

CENTOS_6_FILELIST="${CENTOS_BASIC_TOOLS} ${CENTOS_CODE_TOOLS} ${CENTOS_EMBED_TOOLS} ${AI_TOOLS}"
CENTOS_7_FILELIST="${CENTOS_BASIC_TOOLS} ${CENTOS_CODE_TOOLS} ${CENTOS_EMBED_TOOLS} ${AI_TOOLS}"
INSTALL_CHECK_FLAG="-y"

## check_user_UID
function check_user_UID() {
#	if [ ${UID} -lt 500 ] ;then
#		echo "ERROR: Please don't use root to execute this script."
#		exit 1
#	fi
	# echo "Please input your password "
	sudo ls > /dev/null
	if [ "$?" = "1" ] ;then
		echo -e "\tThere is a configuration/permission problem."
		echo -e "\tPlease ensure that you have permission to use sudo"
		exit 1
	fi
	if [ "${UID}" = "0" ] ;then
		SUDO=''
	else
		SUDO=sudo
	fi
}

## check whether system is centos or not
function check_system_distributor() {
## get system distributor ID: centos ?
LINUX_DISTRIBUTOR=$(cat /etc/issue |tr 'A-Z' 'a-z'|awk ' /release/ {print $1}' | sed -n "1p")
LINUX_DISTRIBUTOR=${LINUX_DISTRIBUTOR:=$(lsb_release -i |tr 'A-Z' 'a-z'|awk '/distributor/ {print $3}')}
LINUX_DISTRIBUTOR=${LINUX_DISTRIBUTOR:=$(cat /etc/*release |tr 'A-Z' 'a-z'|awk '/\<release\>/ {print $1}'|sed -n '1p')}
LINUX_DISTRIBUTOR=${LINUX_DISTRIBUTOR:=$(cat /etc/*release |tr 'A-Z' 'a-z'|awk '/distrib_id=/ {print $1}'|sed 's/distrib_id=//'|sed -n '1p')}

	echo "checking system distributor and release ID ..."
	if [ "${LINUX_DISTRIBUTOR}" = "centos" ] ;then
		echo -e "\tCurrent OS Distributor: ${LINUX_DISTRIBUTOR}"
	else
		echo -e "\tCurrent OS is not centos"
		echo -e "\tCurrent OS Distributor: ${LINUX_DISTRIBUTOR}"
		exit 1
	fi
}

## check whether system is centos 6.5 or 7.x
function check_system_release_version() {
## get system release version: 6.5/7.x ?
LINUX_RELEASE_VERSION=$(cat /etc/issue | awk '/release/ {print $3}'| sed -n '1p')
LINUX_RELEASE_VERSION=${LINUX_RELEASE_VERSION:=$(lsb_release -r | tr 'A-Z' 'a-z' | awk '/release/ {print $2}')}
LINUX_RELEASE_VERSION=${LINUX_RELEASE_VERSION:=$(cat /etc/*release |tr 'A-Z' 'a-z'|awk '/\<release\>/ {print $3}'|sed -n '1p')}
LINUX_RELEASE_VERSION=${LINUX_RELEASE_VERSION:=$(cat /etc/*release |tr 'A-Z' 'a-z'|awk '/distrib_release=/ {print $1}'|sed 's/distrib_release=//'|sed -n '1p')}

	case ${LINUX_RELEASE_VERSION:0:5} in
		6.*)
			echo -e "\tCurrent OS Version: ${LINUX_RELEASE_VERSION}"
			FILELIST=${CENTOS_6_FILELIST}
			;;
		7.*)
			echo -e "\tCurrent OS Version: ${LINUX_RELEASE_VERSION}"
			FILELIST=${CENTOS_7_FILELIST}
			;;
		*)
			echo "Only support CentOS 6.X or 7.X Version, eg: 6.5/7.4 ..."
			exit 1
			;;
	esac
	echo "checked OK, preparing to setup softwares ..."
	# sleep 2
}

## update and upgrade system
function update_upgrade_centos() {
	read -p "	update yum cache <y/N>? " select
	if [ "${select}" = "y" ] ;then
		echo "sudo yum clean all"
		${SUDO} yum clean all
		#update the source.list
		echo "sudo yum makecache"
		${SUDO} yum makecache
	fi

	read -p "	update system <y/N>? " select
	if [ "${select}" = "y" ] ;then
		echo "sudo yum update"
		#upgrade the software have installed on the system
		${SUDO} yum update
	fi
}

## function yum groupinstall software
function yum_groupinstall_supports() {
	echo -e "\tsudo yum groupinstall \"Base\""
	${SUDO} yum groupinstall "Base"
	echo -e "\tInstalling Development tools ..."
	${SUDO} yum groupinstall "Development tools"
	echo -e "\tInstalling X Window System ..."
	${SUDO} yum groupinstall "X Window System"
	echo -e "\tInstalling Desktop ..."
	${SUDO} yum groupinstall "Desktop"
	echo -e "\tInstalling Chinese Support ..."
	${SUDO} yum groupinstall "Chinese Support"
}

## function initial vim
function vim_initialize_viminfo() {
	if [ "${UID}" -ge 500 ] ;then
		local VIMINFO_HOME=${HOME}
		echo "Initializing viminfo file ..."
		${SUDO} rm -f ${VIMINFO_HOME}/.viminfo
		# touch ${VIMINFO_HOME}/.viminfo
	fi
}

#install one software every cycle
function install_soft_for_each() {
	trap 'echo -e "\nAborted by user, exit";exit' INT
	echo "Will install below software for your system:"
	local soft_num=0
	local cur_num=0
	for file in ${FILELIST}
	do
		let soft_num+=1
		echo -e "${file} \c "
	done
	echo ""
	TMOUT=10
	read -p "    Install above softwares <Y/n>? " select
	if [ "${select,,}" = "n" ] ;then
		return
	fi

	# FILELIST=$(echo ${FILELIST} | sed 's/[\t ]/\n/g'| sort -u)
	for file in ${FILELIST}
	do
		let cur_num+=1
		let cur_percentage=cur_num*100/soft_num
		# echo "${cur_percentage}%"

		trap 'echo -e "\nAborted by user, exit";exit' INT
		echo "========================="
		echo " [${cur_percentage}%] installing $file ..."
		echo "-------------------------"
		${SUDO} yum install ${file} ${INSTALL_CHECK_FLAG}
		# sleep 1
		echo "$file installed."
	done
}

read -p "Setup build environment for CentOS, press <Enter> to continue "

check_system_distributor
check_system_release_version
update_upgrade_centos
yum_groupinstall_supports
install_soft_for_each
vim_initialize_viminfo

echo "Work done!"

