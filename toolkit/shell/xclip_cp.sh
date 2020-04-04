#! /bin/bash

####################################################
# Author      : longbin
# Created date: 2018-01-13 13:27:22
####################################################

function xclip_copy()
{
	local filename="$*"
	if [ "-h" == "$1" -o "--help" == "${1}" ] ;then
		echo -e "Usage:\n\txclip_copy file"
		return
	fi
	if [ "" = "${filename}" ] ;then
		echo "ERROR: empty parameter"
		echo -e "Usage:\n\txclip_copy file"
		return
	fi
	if [ ! -f ${filename} ] ;then
		echo "${filename}: No such file"
		return
	fi
	# cat ${filename} | xclip -selection c
	xclip -selection c ${filename}
	echo "${filename} has copied to xclip board."
}

function xclip_paste()
{
	if [ "-h" == "$1" -o "--help" == "${1}" ] ;then
		echo -e "Usage:\n\txclip_paste"
		return
	fi
	# paste from xclip board
	xclip -selection c -o
}

# xclip_copy $*

