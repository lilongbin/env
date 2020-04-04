#! /bin/bash
# 2019-03-21 11:16:47

function plantuml()
{
	local script_abspath=/home/user/toolkit/design/plantuml/plantuml
	if [ "${script_abspath}" = "" ] ;then
		return
	fi
	if [ -f ${script_abspath} ] ;then
		bash ${script_abspath} $*
	fi
}

