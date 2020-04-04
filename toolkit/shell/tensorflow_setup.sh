#! /bin/bash
#
#================================================================
#   Author      : longbin
#   Created date: 2017-10-22 13:31:20
#================================================================

CENTOS_BASIC="gcc gcc-c++ zlib zlib-devel openssl openssl-devel libXScrnSaver tkinter tk-devel readline readline-devel six numpy mack protobuf python python-setuptools python-pip python-scipy python-numpy "
INSTALL_CHECK_FLAG="-y"

PYTHON_ESSENTIAL="six numpy mock protobuf tensorflow-tensorboard "

setuptools_url="https://pypi.python.org/packages/45/29/8814bf414e7cd1031e1a3c8a4169218376e284ea2553cc0822a6ea1c2d78/setuptools-36.6.0.zip"
pip_url="https://pypi.python.org/packages/11/b6/abcb525026a4be042b486df43905d6893fb04f05aac21c32c638e939e447/pip-9.0.1.tar.gz"

function python_zlib()
{
	# Modules/Setup
	#zlib zlibmodule.c -I$(prefix)/include -L$(exec_prefix)/lib -lz
	echo "modify Modules/Setup for zlib in python source file"
}

function setup_pip()
{
	# install setuptools
	zip_tail=".zip"
	if [ "" = "$(ls setuptools-*${zip_tail} 2>/dev/null)" ] ;then
		dl=$(wget ${setuptools_url})
		sleep 1
	fi
	setuptools_zip=$(ls setuptools-*${zip_tail} 2>/dev/null)
	if [ -f ${setuptools_zip} ] ;then
		unzip ${setuptools_zip}
		pushd ${setuptools_zip%%${zip_tail}}
		echo "python setup.py install"
		sleep 2
		python setup.py install
		popd
	fi

	# install pip
	zip_tail=".tar.gz"
	if [ "" = "$(ls pip-*${zip_tail} 2>/dev/null)" ] ;then
		dl=$(wget ${pip_url})
		sleep 1
	fi
	pip_zip=$(ls pip-*${zip_tail} 2>/dev/null)
	if [ -f ${pip_zip} ] ;then
		tar -zxvf ${pip_zip}
		pushd ${pip_zip%%${zip_tail}}
		echo "python setup.py install"
		sleep 2
		python setup.py install
		popd
	fi
	pip install pip --upgrade
}

function install_soft_for_each() {
	FILELIST=$*
	read -p "Setup build environment for CentOS, press <Enter> to continue "
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
	if [ "${select}" = "n" ] ;then
		return
	fi

	# FILELIST=$(echo ${FILELIST} | sed 's/[\t ]/\n/g'| sort -u)
	for file in ${FILELIST}
	do
		let cur_num+=1
		let cur_percent=cur_num*100/soft_num
		# echo "${cur_percent}%"

		trap 'echo -e "\nAborted by user, exit";exit' INT
		echo "========================="
		echo " [${cur_percent}%] installing $file ..."
		echo "-------------------------"
		${SUDO} yum install ${file} ${INSTALL_CHECK_FLAG}
		# sleep 1
		echo "$file installed ."
	done
}


function centos_essential()
{
	install_soft_for_each ${CENTOS_BASIC}
}

function setup_python()
{
	setup_pip
	for tool in ${PYTHON_ESSENTIAL}
	do
		python -m pip install ${tool}
	done
}

centos_essential
setup_python

