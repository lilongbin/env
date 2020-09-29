#! /bin/bash
# 
####################################################
# Author      : longbin
# Created date: 2020-08-11 15:36:49
####################################################

export __BUILD_IMAGE_SCRIPTS_PATH=${HOME}/toolkit/aptiv/build_image

function __build_printbar() {
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

function build_image() {
    local args=$*
    local mscript=build_${1}.sh
    local others=${args#* }
    if [ "$#" = "0" ] ;then
        echo "empty parameters."
        return
    fi
    __build_printbar "### ${mscript} ${others} ###\nbegin"
    sleep 1
    bash "${__BUILD_IMAGE_SCRIPTS_PATH}/${mscript}" ${others}
    __build_printbar "### ${mscript} ${others} ###\nend"
}

function __build_image_complete_()
{
    local curw=${COMP_WORDS[COMP_CWORD]}
    local wordlist=$(find ${__BUILD_IMAGE_SCRIPTS_PATH} -type f -name build_\*.sh -printf "%f\n"|sed '/build_image.sh/d' |sed 's/build_//g' |sed 's/\.sh//g')
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
            param1=${COMP_WORDS[1]}
            fscript=${__BUILD_IMAGE_SCRIPTS_PATH}/build_${param1}.sh
            option1=$(sed -n "/_build_first_params_=/p" ${fscript})
            #echo "option1: ${option1}"
            wordlist=$(eval export ${option1} && echo "${_build_first_params_}")
            #echo "wordlist: ${wordlist}"
            COMPREPLY=( $(compgen -W '${wordlist[@]}' -- "${curw}" ) )
            ;;
        3)
            param1=${COMP_WORDS[1]}
            fscript=${__BUILD_IMAGE_SCRIPTS_PATH}/build_${param1}.sh
            param2=${COMP_WORDS[2]}
            option2=$(sed -n "/_build_second_params_=/p" ${__BUILD_IMAGE_SCRIPTS_PATH})
            #echo "option2: ${option2}"
            wordlist=$(eval export ${option2} && echo "${_build_second_params_}")
            #echo "wordlist: ${wordlist}"
            COMPREPLY=( $(compgen -W '${wordlist[@]}' -- "${curw}" ) )
            ;;
        *);;
    esac
}

complete -F __build_image_complete_ build_image

