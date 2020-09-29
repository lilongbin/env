#! /bin/bash
# 
####################################################
# Author      : longbin
# Created date: 2020-08-11 15:36:49
####################################################

function _r7_env_setup() {
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

function __build_printbar() {
    local info=$*
    for ((i=0;i<60;i++)) do
        echo -e ".\c"
    done
    echo ""
    echo -e "\t${info}"
    for ((i=0;i<60;i++)) do
        echo -e ".\c"
    done
    echo ""
}

function _make_v3_r7() {
    __build_printbar "v3 begin"
    make clean
    make v3_r7 -j8
    __build_printbar "v3 end"
}

function _make_v3_r7_overseas() {
    __build_printbar "overseas begin"
    make clean
    make v3_r7 overseas=1 -j8
    __build_printbar "overseas end"
}

_build_first_params_="overseas v3"
_build_second_params_=""

function build_image_r7() {
    echo "$(pwd)"
    case "$*" in
        'env_setup')
            _r7_env_setup
            ;;
        'overseas')
            _make_v3_r7_overseas
            ;;
        'v3'|*)
            _make_v3_r7
            ;;
    esac
}

build_image_r7 $*

