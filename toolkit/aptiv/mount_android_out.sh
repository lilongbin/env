#! /bin/bash
# 
####################################################
# Author      : longbin
# Created date: 2019-10-08 05:07:17
####################################################

outdisk=/mnt/android_build_out
outdir=${outdisk}/out
aospdir=/home/user/android_code/android_v3/GWM_V3/AOSP
outlink=out

function mount_out_dir()
{
	if [ ! -d ${outdisk} ] ;then
		echo "Error: ${outdisk}: invalid."
		return
	fi
	if [ ! -d ${outdir} ] ;then
		mkdir -p ${outdir}
	fi
	if [ ! -d ${aospdir} ] ;then
		echo "Error: ${aospdir}: aosp invalid."
		return
	fi
    if [ -L ${outlink} ] ;then
        echo "Warning: ${outlink} already exits, please remove it first."
        return
    fi
    local cmdstr="ln -s ${outdir} ${outlink} "
	pushd ${aospdir}
    echo "${cmdstr}"
    ${cmdstr}
	popd
}

mount_out_dir

