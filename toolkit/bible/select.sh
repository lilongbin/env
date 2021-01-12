#! /bin/bash

PS3="please select your dir: "
select option in $(ls)
do
	if [ -n "$option" ] ;then
		ls -l $option
		break
	fi
done

echo "please select one option in 5s "
TMOUT=5
option=ls
select option in \
	ls \
	"echo hello"
do
	case ${REPLY} in
	1)
		ls
		;;
	2)
		echo "hello"
		;;
	?)
		echo "invalid option ."
		;;
	esac
	
	echo your option is: ${option}
	break

done
echo "reply is: ${REPLY}"
if [ "${REPLY}" = "" ] ;then
	echo "use default option: $option"
fi

