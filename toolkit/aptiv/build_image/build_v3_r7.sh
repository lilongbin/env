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

function _helpinfo() {
    echo "Usage:"
    echo -e "\tcurrentcmd [overseas | v3 | env_setup ]"
    echo ""
    exit 0
}

_build_first_params_="overseas v3"
_build_second_params_=""

function build_image_r7() {
    echo "$(pwd)"
    case "$*" in
        '?'|'--help'|'-h')
            _helpinfo
            ;;
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

function __build_image_r7_complete_()
{
    local curw=${COMP_WORDS[COMP_CWORD]}
    local wordlist=$(echo "env_setup overseas v3")
    # 定义双击TAB后显示的结果;compgen -W基于curw提供可能的补全
    #COMPREPLY=( $(compgen -W '${wordlist[@]}' -- "${curw}" ) )

    # COMP_CWORD是系统自动变量,表示当前命令单词索引
    # echo -e "\n\t cmd is ${COMP_WORDS[0]}"
    # echo -e "\t argv1 is ${COMP_WORDS[1]}"
    # echo -e "\t argv2 is ${COMP_WORDS[2]}"
    # echo -e "${COMP_WORDS[0]} ${COMP_WORDS[1]}\c"
    case ${COMP_CWORD} in
        0)  #仍在完成根命令,这里不需要处理
            #${COMP_WORDS[0]}是根命令param0
            ;;
        1)  #根命令已经完成,这里开始补充option0
            #${COMP_WORDS[0]}是根命令
            #${COMP_WORDS[1]}是option0
            #option0=${COMP_WORDS[1]}
            COMPREPLY=( $(compgen -W '${wordlist[@]}' -- "${curw}" ) )
            ;;
        2)#一级主命令已经完成,这里开始补充一级主命令的选项
            #${COMP_WORDS[1]}是一级主命令
            ;;
        *);;
    esac
}

complete -F __build_image_r7_complete_ build_image_r7

build_image_r7 $*

