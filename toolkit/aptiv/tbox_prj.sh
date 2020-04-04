#! /bin/bash
# 
####################################################
# Author      : longbin
# Created date: 2018-07-24 11:17:21
####################################################

# TBOX_PROJ=d2ub
function usage()
{
	echo "This tool can build/clean project, strip binary files and package to a tgz file."
	echo "Usage:"
	echo -e "\tbash $(basename ${0}) sw_dir <d2ub|c1ux> <build|clean|strip>"
	echo -e "eg:bash $(basename ${0}) ~/projects/d2ub/cc0818aa/sw d2ub strip"
}

function _tboxservice_set_env_p358()
{
	# PROJECT_TOP_DIR=~/projects/p358/cc0818aa/sw
	OUT_DIR=${PROJECT_TOP_DIR}/out/${TBOX_PROJ}
	STRIP=/mnt/ngi2-integration-4.4/sdk/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-strip
}

function _tboxservice_set_env_gem()
{
	# PROJECT_TOP_DIR=~/projects/gem/cc0818aa/sw
	OUT_DIR=${PROJECT_TOP_DIR}/out/${TBOX_PROJ}
	STRIP=/mnt/ngi2-integration-4.4/sdk/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-strip
}

function _tboxservice_set_env_d2ub()
{
	# PROJECT_TOP_DIR=~/projects/gem/cc0818aa/sw
	OUT_DIR=${PROJECT_TOP_DIR}/out/${TBOX_PROJ}
	STRIP=/mnt/ngi2-integration-4.4/sdk/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-strip
}

function _tboxservice_set_env_c1ux()
{
	# PROJECT_TOP_DIR=~/projects/gem/cc0818aa/sw
	OUT_DIR=${PROJECT_TOP_DIR}/out/${TBOX_PROJ}
	STRIP=/mnt/ngi2-integration-4.4/sdk/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-strip
}

function _tboxservice_init()
{
	# echo "$#: $*"
	if [ "$*" = "-h" -o "$*" = "--help" ] ;then
		usage
		exit
	fi
	if [ "$#" != "3" ] ;then
		echo "ERROR: parameter error."
		usage
		exit
	fi
	PROJECT_TOP_DIR=$1
	TBOX_PROJ=$2
	FUNCTION=$3
	if [ "" = "${FUNCTION}" ] ;then
		usage
		exit
	fi
	if [ ! -d ${PROJECT_TOP_DIR} ] ; then
		echo "ERROR: ${PROJECT_TOP_DIR}: NO such directory."
		usage
		exit
	fi
	case "${TBOX_PROJ}" in
		gem)
			;;
		p358)
			;;
		d2ub)
			;;
		c1ux)
			;;
		*)
			echo "ERROR: project name invalid."
			usage
			exit
			;;
	esac

	echo "TBOX_PROJ=${TBOX_PROJ}"
	_tboxservice_set_env_${TBOX_PROJ}
	if [ "$?" != "0" ] ;then
		echo "ERROR: undefine _tboxservice_set_env_${TBOX_PROJ}"
		exit
	fi
}

function tbox_build()
{
	local build_flag=""
	if [ "${TBOX_PROJ}" = "d2ub" ] ;then
		build_flag=nomost
	elif [ "${TBOX_PROJ}" = "c1ux" ] ;then
		build_flag=nomost
	fi
	pushd ${PROJECT_TOP_DIR}
	echo "./build ${TBOX_PROJ} ${build_flag}"
	sleep 3
	./build ${TBOX_PROJ}
	popd
}

function tbox_clean()
{
	pushd ${PROJECT_TOP_DIR}
	make clean
	popd
}

function tbox_strip()
{
	if [ ! -d ${OUT_DIR} ] ;then
		echo "ERROR: ${OUT_DIR}: No such directory."
		usage
		return
	fi
	# copy source file
	local TBOX_IMG_PACKAGE_DIR=${PROJECT_TOP_DIR}/flash_tbox_img
	pushd ${PROJECT_TOP_DIR}
		if [ "" = ${TBOX_IMG_PACKAGE_DIR} ] ;then
			echo "ERROR: TBOX_IMG_PACKAGE_DIR: undefined."
			return
		fi
		[ -f ${TBOX_IMG_PACKAGE_DIR}/bin ] || mkdir -p ${TBOX_IMG_PACKAGE_DIR}/bin
		[ -f ${TBOX_IMG_PACKAGE_DIR}/lib ] || mkdir -p ${TBOX_IMG_PACKAGE_DIR}/lib
		if [ ! -d ${TBOX_IMG_PACKAGE_DIR} ] ;then
			echo "ERROR: ${TBOX_IMG_PACKAGE_DIR}: create directory failed."
			return
		fi
		echo "copy binary files from ${OUT_DIR} -> ${TBOX_IMG_PACKAGE_DIR} ..."
		cp -rfd ${OUT_DIR}/bin/*    ${TBOX_IMG_PACKAGE_DIR}/bin/
		cp -rfd ${OUT_DIR}/lib/*.so ${TBOX_IMG_PACKAGE_DIR}/lib/
		rm -f ${TBOX_IMG_PACKAGE_DIR}/lib/libgnsdk*
		rm -f ${TBOX_IMG_PACKAGE_DIR}/lib/libtinyrpc.so
		rm -f ${TBOX_IMG_PACKAGE_DIR}/lib/libshrdatapool.so
		sleep 1
	popd

	# strip
	local BIN_FILES="$(find ${TBOX_IMG_PACKAGE_DIR}/bin/ -type f)"
   	local LIB_FILES="$(find ${TBOX_IMG_PACKAGE_DIR}/lib/ -type f)"
	local FILES="${BIN_FILES} ${LIB_FILES}"
	echo "${FILES}"
	echo ""
	if [ "" = "${FILES}" ] ;then
		echo "ERROR: parameter is empty."
		echo -e "Usage:\n\ttbox_strip file"
		return
	fi
	echo "${STRIP}"
	echo ""
	for file in ${FILES}
	do
		if [ ! -f ${file} ] ;then
			echo "ERROR: ${file}: No such file."
			continue
		fi
		echo "strip ${file} ..."
		sudo ${STRIP} ${file}
	done
	echo "strip complete."

	# package
	echo "start to package ${TBOX_IMG_PACKAGE_DIR}"
	pushd ${PROJECT_TOP_DIR}
	tar -zcvf ${TBOX_IMG_PACKAGE_DIR##*/}.tgz ${TBOX_IMG_PACKAGE_DIR##*/}
	popd
	echo "package ${TBOX_IMG_PACKAGE_DIR} to ${TBOX_IMG_PACKAGE_DIR}.tgz complete."
}

function _tboxservice_main()
{
	_tboxservice_init $*
	case "${FUNCTION}" in
		build)
			tbox_build
			;;
		clean)
			tbox_clean
			;;
		strip)
			tbox_strip
			;;
		*)
			echo "ERROR: ${FUNCTION}: Not supported yet."
			usage
			;;
	esac
}

_tboxservice_main $*

