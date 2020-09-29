#! /bin/bash
# 
####################################################
# Author      : longbin
# Created date: 2020-08-11 15:36:49
####################################################

function preinit() {
    tools_parent_path=/mnt/tmp/gwm_v3/RENESAS_RCH3M3_Android_O-MR1_ReleaseNote_2018_03E/mydroid/prebuilts/gcc/linux-x86/arm
    tools_dir=arm-none-eabi-6
    real_tools_dir=/opt/gcc-arm-none-eabi-6-2017-q2-update
    echo "sudo mkdir -p /mnt/tmp/gwm_v3"
    sudo mkdir -p /mnt/tmp/gwm_v3
    sudo chmod 777 /mnt/tmp/gwm_v3
    mkdir -p ${tools_parent_path}
    pushd ${tools_parent_path} && ln -sf ${real_tools_dir} ${tools_dir} && echo "set environment ok"
    popd
}
#preinit ; exit 0

printbar() {
    local info=$*
    for ((i=0;i<60;i++)) do
        echo -e "#\c"
    done
    echo ""
    echo -e "build r7 for:\n\t${info}"
    for ((i=0;i<60;i++)) do
        echo -e "#\c"
    done
    echo ""
}

build() {
    if [ "$#" = "0" ] ;then
        printbar "*** v3 ***\nbegin"
        sleep 1
        make clean
        make v3_r7 -j8
        printbar "*** v3 ***\nend"
    else
        printbar "*** overseas ***\nbegin"
        sleep 1
        make clean
        make v3_r7 overseas=1 -j8
        printbar "*** overseas ***\nend"
    fi
}

build $*

