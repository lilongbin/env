#! /bin/bash
# created by Longbin_Li <beangr@163.com>
clear
echo "use this script to generate cscope files and tags for linux kernel"
FILENAME=cscope.files

function gencscopef()
{
	echo "Searching *.h *.c ... from $PWD"
	
	# find . -iname "*.[hc]" \
			 # | sed -n "s%^\.%$PWD%p" \
			 # > ${FILENAME}
	
	# if [[ $? == 0 ]] ;then
		# [[ -s ${FILENAME} ]] && echo "generate files: " && ls -lh ${FILENAME} 
	# else
		# echo "error occured, ${FILENAME} not be generated" && exit 1
	# fi
	# sed -n "s/old/new/p"
	# sed -n "s%$old%$new%p" use % to replace if used var contains of "/"
	
	echo "generating cscope index files ..."
	# cscope -bkq -i ${FILENAME}
	cscope-indexer -r
	
	# [[ $? == 0  ]] && echo "generated files: " && ls -lh cscope*

}

if [[ -f cscope.out  ]] ;then
	read -p "		update cscope files ? <y/n> " select
	if [[ "$select" == "y" ]] ;then
		rm cscope.out
		gencscopef
	else
		echo "not updated cscope files"
	fi
else
	gencscopef
fi


function genctags()
{
	echo "generating tags file ..."
	ctags -R *
	# --langmap=c:+.h \
			  # --fields=+lS \
			  # $PWD
	echo "preparations over" && ls -lh tags
}

if [[ -f tags ]] ;then
	read -p "		update tags file ? <y/n> " select
	if [[ "$select" == "y" ]] ;then
		rm tags
		genctags
	else
		echo "not updated tags"
	fi
else
	genctags
fi

