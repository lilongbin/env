#! /bin/bash
#
####################################################
# Filename    : djump.sh
# Author      : longbin
# Created date: 2017-04-01 14:16:19
####################################################
#
# change history
# v1.1 2020-05-04 merge dtag/duntag/djump to one command djump
#

export __DJUMP_VERSION='v1.2'
export __DJUMP_DATA_PATH=${HOME}/.djump

[ -d ${__DJUMP_DATA_PATH} ] || mkdir -p ${__DJUMP_DATA_PATH}

function __djump_print_specify_length_str__()
{
    local target_length=${DJUMP_PRINT_COLUMN1_WIDTH_MAX:=8}
    local str_length=$(echo $*|wc -c)

    echo -n "$*"
    for (( ;str_length<=target_length;str_length++ ))
    do
        # 如果字符长度不够就用空格补齐
        echo -n ' '
    done
}

function __djump_list_tag_func__()
{
    local tag_name=$1
    local tag_len=${#tag_name}
    local tag_valid=0
    local COLUMN_SEP='----> '
    DJUMP_PRINT_COLUMN1_WIDTH_MAX=8

    # 获取DJUMP_PRINT_COLUMN1_WIDTH_MAX
    DJUMP_TAGS_LIST=$(ls ${__DJUMP_DATA_PATH})
    for line in ${DJUMP_TAGS_LIST}
    do
        if [ "$(echo ${line}|wc -c)" -gt "${DJUMP_PRINT_COLUMN1_WIDTH_MAX}" ] ;then
            let DJUMP_PRINT_COLUMN1_WIDTH_MAX=$(echo ${line}|wc -c)
        fi
        if [ "${line:0:${tag_len}}" = "${tag_name}" ] ;then
            tag_valid=1
        fi
    done
    if [ "${tag_valid}" = "0" ] ;then
        tag_name=''
    fi

    # 显示每条tag的具体信息
    DJUMP_TAGS_DETAILS=$(ls -l ${__DJUMP_DATA_PATH} | awk '{ if ($(NF-1)=="->") { print $(NF-2)"->"$(NF) } }')
    for line in ${DJUMP_TAGS_DETAILS}
    do
        link_file=$(echo ${line}|awk -F'->' '{print $1}')
        link_path=$(echo ${line}|awk -F'->' '{print $2}')
        if [ "${link_file}" = "" ] ;then
            continue
        fi
        if [ "${tag_name}" != "" ] ;then
            if [ "${tag_name}" = "${link_file:0:${tag_len}}" ] ;then
                __djump_print_specify_length_str__ "${link_file}"
                echo "${COLUMN_SEP}${link_path}"
                #break
            else
                continue
            fi
        else
            __djump_print_specify_length_str__ "${link_file}"
            echo "${COLUMN_SEP}${link_path}"
        fi
    done
}

function djump()
{
    local argv1=$1
    local argv2=$2
    local argv3=$3
    local helpinfo="djump ${__DJUMP_VERSION}"\
'\n\tIt is a convenient tool to help you change work directory quickly.'\
'\nUsage:'\
'\n\tdjump [[--jump] [tag_name]]|[--list [tag_name]]|[--create|--delete tag_name]|[tag_name]'\
'\n'\
'\n\tdjump                     change directory to the last one'\
'\n\tdjump tag_name            change directory to the tag_name represent.'\
'\n\tdjump --jump              change directory to the last one'\
'\n\tdjump --jump tag_name     change directory to the tag_name represent.'\
'\n\tdjump --list              to list all tags created'\
'\n\tdjump --list tag_name     to list tag named like tag_name'\
'\n\tdjump --create tag_name   to create tag named as tag_name, tag_name shouldnot start with (-)'\
'\n\tdjump --delete tag_name   to delete tag named as tag_name'

    for argvx in "$@"
    do
        # echo ${argvx}
        case "${argvx,,}" in
            '?')
                echo -e "${helpinfo}"
                return 0
                ;;
            '--help'|'-h')
                echo -e "${helpinfo}"
                return 0
                ;;
        esac
    done

    [ -d ${__DJUMP_DATA_PATH} ] || mkdir -p ${__DJUMP_DATA_PATH}
    if [ "$?" != "0" ] ;then
        echo "error: permission denied."
        return 1
    fi

    # 如果0个参数则cd -
    # 如果只有1个参数则直接跳转
    # 如果option1 in ["--create", "--delete", "--list"], action
    # 否则给出帮助信息和可以选择的参数
    if [ "${argv1:0:1}" = "-" ] ;then
        option1=${argv1}
        tag_name=${argv2}
        extra_arg=${argv3}
    else
        option1='--jump'
        tag_name=${argv1}
        extra_arg=${argv2}
    fi
    if [ "${tag_name:0:1}" = "-" ] ;then
        echo "error: invalid tag_name, tag_name shouldnot start with (-)"
        echo -e "${helpinfo}"
        return 1
    fi
    if [ "${extra_arg}" != "" ] ;then
        echo "error: invalid parameters."
        # show all created tag
        echo -e "${helpinfo}"
        return 1
    fi

    case "${option1,,}" in
        '-j'|'--jump')
            # jump
            if [ "${tag_name}" = '' ] ;then
                cd -
                return 0
            fi
            if [ -L ${__DJUMP_DATA_PATH}/${tag_name} ] ;then
                if [ ! -d ${__DJUMP_DATA_PATH}/${tag_name} ] ;then
                    # ${tag_name} 是个link文件却不是一个有效目录说明其是死连接
                    echo "error: ${tag_name}: Invlid soft-link source directory."
                    return 1
                fi
                # 直接cd到指定的目标(目录)
                cd -P ${__DJUMP_DATA_PATH}/${tag_name} 2>/dev/null
                if [ "$?" != "0" ] ;then
                    echo "error: ${tag_name}: Invalid tag."
                    return 1
                fi
            else
                echo "error: ${tag_name}: No such tag."
                if [ "$(ls ${__DJUMP_DATA_PATH})" != "" ] ;then
                    echo -e "\nThe tag_name could be:"
                    for line in $(ls ${__DJUMP_DATA_PATH})
                    do
                        echo -e "\t${line}"
                    done
                fi
                return 1
            fi
            ;;
        '-c'|'--create')
            # create tag
            if [ "${tag_name}" = "" ] ;then
                echo "error: Invalid tag_name, tag_name shouldnot be empty."
                echo -e "${helpinfo}"
                return 1
            fi
            if [ -L ${__DJUMP_DATA_PATH}/${tag_name} ] ;then
                echo "${tag_name}: already exists."
                read -p "update the tag ${tag_name} to $(pwd) ?[y/N] " user_select
                if [ "${user_select/Y/y}" = "y" ] ;then
                    rm -f ${__DJUMP_DATA_PATH}/${tag_name}
                else
                    return
                fi
            fi
            ln -s $(pwd) ${__DJUMP_DATA_PATH}/${tag_name} 2>/dev/null
            if [ "$?" != "0" ] ;then
                echo "error: permission denied."
                return 1
            fi
            ;;
        '-d'|'--delete')
            # 如果只有一个参数则删除相应的tag_name
            if [ "${tag_name}" = "" ] ;then
                echo "error: Invalid tag_name, tag_name shouldnot be empty."
                # show all created tag_name
                echo -e "${helpinfo}"
                if [ "$(ls ${__DJUMP_DATA_PATH})" != "" ] ;then
                    echo -e "\nThe tag_name could be:"
                    for line in $(ls ${__DJUMP_DATA_PATH})
                    do
                        echo -e "\t${line}"
                    done
                fi
                return 1
            fi
            if [ -L ${__DJUMP_DATA_PATH}/${tag_name} ] ;then
                read -p "remove tag: \`${tag_name}'? " user_select
                if [ "${user_select/Y/y}" = "y" ] ;then
                    rm -f ${__DJUMP_DATA_PATH}/${tag_name} 2>/dev/null
                    if [ "$?" != "0" ] ;then
                        echo "error: permission denied."
                        return 1
                    fi
                fi
            else
                echo "error: ${tag_name}: No such tag."
                return 1
            fi
            ;;
        '-l'|'--list')
            # 显示已创建tag的具体信息
            __djump_list_tag_func__ ${tag_name}
            ;;
        *)
            echo "${option1}: Invalid option."
            echo -e "${helpinfo}"
            return 1
            ;;
    esac
    return 0
}

# 为dtag命令提供TAB键自动补全功能
function __djump_complete_dtag()
{
    local curw=${COMP_WORDS[COMP_CWORD]}
    local wordlist=$(find ${__DJUMP_DATA_PATH} -maxdepth 1 -type l -printf "%f\n" )
    # 定义双击TAB后显示的结果;compgen -W基于curw提供可能的补全
    COMPREPLY=( $(compgen -W '${wordlist[@]}' -- "${curw}" ) )
}

# 为djump命令提供TAB键自动补全功能
function __djump_complete_djump()
{
    local curw=${COMP_WORDS[COMP_CWORD]}
    local wordlist=$(find ${__DJUMP_DATA_PATH} -maxdepth 1 -type l -printf "%f\n" )
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
        1)  #根命令已经完成,这里开始补充option1
            #${COMP_WORDS[0]}是根命令
            #${COMP_WORDS[1]}是option1
            option1=${COMP_WORDS[1]}
            if [ "${option1:0:1}" = '-' ] ;then
                COMPREPLY=( $(compgen -W '-j --jump -c --create -d --delete -l --list' -- "${curw}" ) )
            else
                COMPREPLY=( $(compgen -W '${wordlist[@]}' -- "${curw}" ) )
            fi
            ;;
        2|*)#一级主命令已经完成,这里开始补充一级主命令的选项
            #${COMP_WORDS[1]}是一级主命令
            case ${COMP_WORDS[1]} in
                '-j'|'--jump')
                    COMPREPLY=( $(compgen -W '${wordlist[@]}' -- "${curw}" ) )
                    ;;
                '-d'|'--delete')
                    COMPREPLY=( $(compgen -W '${wordlist[@]}' -- "${curw}" ) )
                    ;;
                '-l'|'--list')
                    COMPREPLY=( $(compgen -W '${wordlist[@]}' -- "${curw}" ) )
                    ;;
                *)
                    ;;
            esac
            ;;
    esac
}

# complete -F __djump_complete_dtag dtag
complete -F __djump_complete_djump djump

