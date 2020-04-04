#! /bin/bash
# 
####################################################
# Author      : longbin
# Created date: 2018-03-09 21:57:49
####################################################

OPTION_MATRIX_1=$(echo "
1) all         for all series.
2) arm         for arm series.
3) ppc         for ppc series.
4) clean       clean all.
" | tr ' ' '\1')

function helpinfo()
{
	local helpdocs="
Usage:
	bash  $0  [n]

	the parameter n could be one of 1 2 3 4
	1 build all
	2 build arm
	3 build ppc
	4 clean
"
	# here document
	cat << EOF
	${helpdocs}
EOF
}

function countdown_func()
{
	for ((countdown=${TMOUT}-1; countdown>=1; countdown--))
	do
		trap 'exit 0' HUP
		echo -e "\r${prompt_msg} in ${countdown}s: \c "
		sleep 1
	done
}

# input var: iALL_OPTION_MATRIX
# output var: g_FINAL_OPTION_VAL
# local env: g_PROMPT_MSG g_COUNTDOWN_ON g_DEFAULT_OPTION_ID g_TIMEOUT_OPTION_ID
# g_DEFAULT_OPTION_ID is the default choice's index
# g_TIMEOUT_OPTION_ID is the default choice's index if times out
# g_COUNTDOWN_ON is the switcher of countdown func
# iALL_OPTION_MATRIX need be:
# num) option  msgs... | tr ' ' '\1'; and num start with 1
# eg:
#g_DEFAULT_OPTION_ID=$*
#g_TIMEOUT_OPTION_ID=0
#g_PROMPT_MSG="device's CPU arch"
#g_COUNTDOWN_ON=n
#select_one_func ${OPTION_MATRIX_1}
#echo ${g_FINAL_OPTION_VAL}
function select_one_func()
{
	local prompt_msg="Please type\e[1;31m the No.\e[0m of ${g_PROMPT_MSG}"
	local countdown_on=${g_COUNTDOWN_ON}
	local user_choice=${g_DEFAULT_OPTION_ID}
	local iALL_OPTION_MATRIX=$*
	g_FINAL_OPTION_VAL=

	local func_usage="
		Usage:\n
		\t select_one_func iALL_OPTION_MATRIX
	"

	# only integer is validate
	if [ "$(echo ${user_choice}| grep '[^0-9]' )" != "" ]; then
		echo "ERROR: ${user_choice}: Invalid option, should be Non-negtive integer."
		return 1
	fi
	if [ "${iALL_OPTION_MATRIX}" = "" ]; then
		echo "ERROR: Invalid param, cannot be empty."
		echo -e "${func_usage}"
		return 1
	fi

	local is_1st_loop=y
	while true
	do
		local istart=1
		local iend=${istart}
		local iindex=0

		# show all option matrix
		for line in ${iALL_OPTION_MATRIX}
		do
			iindex=$((iindex + 1))
			line=$(echo ${line}| tr '\1' ' ')
			echo "${line}"
			option_arr[$iindex]=$(echo ${line}| cut -d' ' -f2)
			# echo "${option_arr[iindex]}"
		done
		end_str="0) quit"
		echo ${end_str}
		iend=${iindex}
		# echo $istart $iindex $iend

		# get user choice
		if [ "${is_1st_loop}" = "y" ]; then
			# user_choice=${g_DEFAULT_OPTION_ID}
			is_1st_loop=n
		else
			user_choice=
		fi
		if [ "${user_choice}" = "" ]; then
			if [ "${countdown_on}" = "y" ]; then
				local TMOUT=10
				if [ "${g_TIMEOUT_OPTION_ID}" != "" ]; then
					echo -e "if times out, the ${g_TIMEOUT_OPTION_ID} will be the default option."
				fi
				countdown_func &
				bg_pid=$!
				trap 'kill ${bg_pid}; echo; exit' INT
				read -p "" user_choice
				TMOUT=0
				if [ "${user_choice}" = "" -a "${g_TIMEOUT_OPTION_ID}" != "" ]; then
					# use g_TIMEOUT_OPTION_ID
					user_choice=${g_TIMEOUT_OPTION_ID}
				fi

				(kill -HUP ${bg_pid} &>/dev/null)
			else
				echo -e "${prompt_msg}: \c "
				read -p "" user_choice
			fi
		fi
		if [ "${user_choice}" = "" ]; then
			echo "ERROR: select None option, please try again."
			continue
		fi

		# only integer is validate
		if [ "$(echo ${user_choice}| grep '[^0-9]' )" != "" ]; then
			echo "ERROR: ${user_choice}: Invalid option, should be Non-negtive integer."
			continue
		fi
		# istart <= user_choice <= iend
		if [ ${istart} -le ${user_choice} -a ${user_choice} -le ${iend} ]; then
			echo "Your choice: ${user_choice}"
			# echo "${option_arr[${user_choice}]}"
			g_FINAL_OPTION_VAL="${option_arr[${user_choice}]}"
			echo -e "\e[1;31m${g_FINAL_OPTION_VAL}\e[0m selected."
			break
		elif [ "${user_choice}" = "0" ]; then
			echo "Bye."
			g_FINAL_OPTION_VAL="$(echo ${end_str}| cut -d' ' -f2)"
			exit
		else
			echo "ERROR: ${user_choice} out of range(${istart}, ${iend}), please try again."
			continue
		fi
	done
}


function select_xxx_func()
{
	if [ "$*" = "-h" -o "$*" = "--help" ];then
		helpinfo
		exit
	fi

	local g_DEFAULT_OPTION_ID=$*
	local g_TIMEOUT_OPTION_ID=0
	local g_PROMPT_MSG="device's CPU arch"
	local g_COUNTDOWN_ON=n
	select_one_func ${OPTION_MATRIX_1}
	local selected_target=${g_FINAL_OPTION_VAL}
	echo ${g_FINAL_OPTION_VAL}
}

select_xxx_func $*


