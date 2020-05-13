#! /bin/bash
#
####################################################
# Filename    : dtags.sh
# Author      : longbin
# Created date: 2017-04-01 14:16:19
####################################################

export __DIR_TAGS_PATH=${HOME}/.dtags

function __print_specify_length_str__()
{
    local target_length=${COLUMN1_WIDTH_MAX:=8}
    local str_length=$(echo $*|wc -c)

    echo -n "$*"
    for (( ;str_length<=target_length;str_length++ ))
    do
        # 如果字符长度不够就用空格补齐
        echo -n ' '
    done
}

function __show_dtags_func__()
{
    local tag_name=$1
    local tag_len=${#tag_name}
    local tag_valid=0
    local COLUMN_SEP='----> '
    COLUMN1_WIDTH_MAX=8

    # 获取COLUMN1_WIDTH_MAX
    DIR_TAGS_LIST=$(ls ${__DIR_TAGS_PATH})
    for line in ${DIR_TAGS_LIST}
    do
        if [ "$(echo ${line}|wc -c)" -gt "${COLUMN1_WIDTH_MAX}" ] ;then
            let COLUMN1_WIDTH_MAX=$(echo ${line}|wc -c)
        fi
        if [ "${line:0:${tag_len}}" = "${tag_name}" ] ;then
            tag_valid=1
        fi
    done
    if [ "${tag_valid}" = "0" ] ;then
        tag_name=''
    fi

    # 显示每条dtag的具体信息
    DIR_TAGS_DETAILS=$(ls -l ${__DIR_TAGS_PATH} | awk '{ if ($(NF-1)=="->") { print $(NF-2)"->"$(NF) } }')
    for line in ${DIR_TAGS_DETAILS}
    do
        link_file=$(echo ${line}|awk -F'->' '{print $1}')
        link_path=$(echo ${line}|awk -F'->' '{print $2}')
        if [ "${link_file}" = "" ] ;then
            continue
        fi
        if [ "${tag_name}" != "" ] ;then
            if [ "${tag_name}" = "${link_file:0:${tag_len}}" ] ;then
                __print_specify_length_str__ "${link_file}"
                echo "${COLUMN_SEP}${link_path}"
                #break
            else
                continue
            fi
        else
            __print_specify_length_str__ "${link_file}"
            echo "${COLUMN_SEP}${link_path}"
        fi
    done
}

function dtag()
{
    local args=$*
    local param1=$1
    local tag_name=$2
    local helpinfo="Usage:\n\tdtag [{--list [tag_name]}|{--create tag_name}|{--delete tag_name}]\n"\
"\n\tdtag                     --- to list all tags created by dtag"\
"\n\tdtag --list              --- to list all tags created by dtag"\
"\n\tdtag --list tag_name     --- to list tag named as tag_name"\
"\n\tdtag --create tag_name   --- to create tag named as tag_name"\
"\n\tdtag --delete tag_name   --- to delete tag named as tag_name"

    if [ "${args,,}" = "--help" -o "${args,,}" = "-h" -o "${args,,}" = "?" ] ;then
        echo -e "${helpinfo}"
        return 0
    fi
    if [ "${param1,,}" = "?" -o "${tag_name,,}" = "?" ] ;then
        echo -e "${helpinfo}"
        return 0
    fi

    [ -d ${__DIR_TAGS_PATH} ] || mkdir -p ${__DIR_TAGS_PATH}
    if [ "$?" != "0" ] ;then
        echo "ERROR: permission denied."
        return 1
    fi

    # 如果参数为空则显示所有已创建的dtags
    # 如果param1 is not in ["--create", "--delete", "--list"]
    # 否则报错
    if [ "$#" = "0" ] ;then
        # show all created dtags
        if [ "$(ls ${__DIR_TAGS_PATH})" = "" ] ;then
            echo -e "${helpinfo}"
        else
            # 显示所有已创建dtags的具体信息
            __show_dtags_func__
        fi
        return 0
    else
        case "${param1,,}" in
            "--create")
                ;;
            "--delete")
                ;;
            "--list")
                ;;
            *)
                echo "${param1}: Invalid option."
                echo -e "${helpinfo}"
                return 1
                ;;
        esac
    fi

    case "${param1,,}" in
        "--create")
            # create dtag
            if [ -L ${__DIR_TAGS_PATH}/${tag_name} ] ;then
                echo "${tag_name}: already exists."
                read -p "update the dtag ${tag_name} to $(pwd) ?[y/N] " user_select
                if [ "${user_select/Y/y}" = "y" ] ;then
                    rm -f ${__DIR_TAGS_PATH}/${tag_name}
                else
                    return
                fi
            fi
            ln -s $(pwd) ${__DIR_TAGS_PATH}/${tag_name} 2>/dev/null
            if [ "$?" != "0" ] ;then
                echo "ERROR: permission denied."
                return 1
            fi
            return 0
            ;;
        "--delete")
            # 如果只有一个参数则删除相应的tag_name
            if [ "${tag_name}" = "" ] ;then
                # show all created tag_names
                echo -e "Usage:\n\tduntag tag_name"
                if [ "$(ls ${__DIR_TAGS_PATH})" != "" ] ;then
                    echo -e "\nThe tag_name could be:"
                    for line in $(ls ${__DIR_TAGS_PATH})
                    do
                        echo -e "\t${line}"
                    done
                fi
                return 1
            fi
            if [ -L ${__DIR_TAGS_PATH}/${tag_name} ] ;then
                read -p "remove dtag: \`${tag_name}'? " user_select
                if [ "${user_select/Y/y}" = "y" ] ;then
                    rm -f ${__DIR_TAGS_PATH}/${tag_name} 2>/dev/null
                    if [ "$?" != "0" ] ;then
                        echo "ERROR: permission denied."
                        return 1
                    fi
                fi
            else
                echo "ERROR: ${tag_name}: No such dtag."
                return 1
            fi
            ;;
        "--list")
            # 显示已创建dtags的具体信息
            __show_dtags_func__ ${tag_name}
            ;;
        *)
            return 1
            ;;
    esac
}

function djump()
{
    local args=$*
    local tag_name=$1
    local helpinfo="Usage:\n\tdjump [tag_name]\n"\
"\n\tdjump              --- change directory to the last one"\
"\n\tdjump tag_name     --- change directory to the tag_name represent, could use dtag to see details."

    if [ "${args,,}" = "--help" -o "${args,,}" = "-h" -o "${args,,}" = "?" ] ;then
        echo -e "${helpinfo}"
        return 0
    fi

    [ -d ${__DIR_TAGS_PATH} ] || mkdir -p ${__DIR_TAGS_PATH}
    if [ "$?" != "0" ] ;then
        echo "ERROR: permission denied."
        return 1
    fi

    # 如果0个参数则cd -
    # 如果只有1个参数则直接跳转
    # 否则给出帮助信息和可以选择的参数
    if [ "$#" -eq "0" ] ;then
        # change derectory to the last one
        cd -
        return 0
    elif [ "$#" -gt "1" ] ;then
        # show all created dtags
        echo -e "${helpinfo}"
        if [ "$(ls ${__DIR_TAGS_PATH})" != "" ] ;then
            echo -e "\nThe tag_name could be:"
            for line in $(ls ${__DIR_TAGS_PATH})
            do
                echo -e "\t${line}"
            done
        fi
        return 1
    fi

    if [ -L ${__DIR_TAGS_PATH}/${tag_name} ] ;then
        if [ ! -d ${__DIR_TAGS_PATH}/${tag_name} ] ;then
            # ${tag_name} 是个link文件却不是一个有效目录说明其是死连接
            echo "ERROR: ${tag_name}: Invlid dtag, please check the destination directory."
            return 1
        fi
        # 直接cd到指定的目标(目录)
        cd -P ${__DIR_TAGS_PATH}/${tag_name} 2>/dev/null
        if [ "$?" != "0" ] ;then
            echo "ERROR: ${tag_name}: Invalid dtag."
            return 1
        fi
    else
        echo "ERROR: ${tag_name}: No such dtag."
        return 1
    fi
}

# 为djump命令提供TAB键自动补全功能
function __complete_djump()
{
    local curw=${COMP_WORDS[COMP_CWORD]}
    local wordlist=$(find ${__DIR_TAGS_PATH} -type l -printf "%f\n" )
    # 定义双击TAB后显示的结果;compgen -W基于curw提供可能的补全
    COMPREPLY=( $(compgen -W '${wordlist[@]}' -- "${curw}" ) )
}

# 为dtag命令提供TAB键自动补全功能
function __complete_dtags()
{
    local curw=${COMP_WORDS[COMP_CWORD]}
    local wordlist=$(find ${__DIR_TAGS_PATH} -type l -printf "%f\n" )
    # 定义双击TAB后显示的结果;compgen -W基于curw提供可能的补全
    #COMPREPLY=( $(compgen -W '${wordlist[@]}' -- "${curw}" ) )

    # COMP_CWORD是系统自动变量,表示当前命令单词索引
    # echo -e "\n\tcmd is ${COMP_WORDS[0]}"
    # echo -e "\tpara1 is ${COMP_WORDS[1]}"
    # echo -e "\tpara2 is ${COMP_WORDS[2]}"
    # echo -e "${COMP_WORDS[0]} ${COMP_WORDS[1]}\c"
    case ${COMP_CWORD} in
        0)  #仍在完成根命令,这里不需要处理
            #${COMP_WORDS[0]}是根命令param0
            ;;
        1)  #根命令已经完成,这里开始补充param1
            #${COMP_WORDS[0]}是根命令
            #${COMP_WORDS[1]}是param1
            if [ ${COMP_WORDS[0]} = 'dtag' ] ;then
                COMPREPLY=( $(compgen -W '--create --delete --list' -- "${curw}" ) )
            fi
            ;;
        2|*)#一级主命令已经完成,这里开始补充一级主命令的选项
            #${COMP_WORDS[1]}是一级主命令
            case ${COMP_WORDS[1]} in
                '--delete')
                    COMPREPLY=( $(compgen -W '${wordlist[@]}' -- "${curw}" ) )
                    ;;
                '--list')
                    COMPREPLY=( $(compgen -W '${wordlist[@]}' -- "${curw}" ) )
                    ;;
                *)
                    ;;
            esac
            ;;
    esac
}

complete -F __complete_djump djump
complete -F __complete_dtags dtag

