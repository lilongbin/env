#! /bin/bash
# created by Longbin_Li <beangr@163.com>
clear
echo "This script used to generate cscope files and tags for android source code"
FILENAME=cscope.files

function gencscopef()
{
	echo "Searching *.h *.c *.cpp ... from $PWD"
	
	find . -name "out" -prune \
			 -o -iname "*.[hc]" \
			 -o -iname "*.[hc]pp" \
			 > ${FILENAME}
	#		 | sed -n "s%^\.%$PWD%p" \
	
	if [[ $? == 0 ]] ;then
		[[ -s ${FILENAME} ]] && echo "generate files: " && ls -lh ${FILENAME} 
	else
		echo "error occured, ${FILENAME} not be generated" && exit 1
	fi
	# sed -n "s/old/new/p"
	# sed -n "s%$old%$new%p" use % to replace if used var contains of "/"
	
	echo "generating cscope index files ..."
	cscope -bkq -i ${FILENAME}
	
	[[ $? == 0  ]] && echo "generated files: " && ls -lh cscope*
}

function gencscopefi()
{
	cscope-indexer -r -v
}

if [[ -f cscope.out  ]] ;then
	read -p "		update cscope files ? <y/n> " select
	if [[ "$select" == "y" ]] ;then
		rm cscope.out
		# gencscopef
		gencscopefi
	else
		echo "not updated cscope files"
	fi
else
	# gencscopef
	gencscopefi
fi


function genctags()
{
echo "generating ctags file ..."
ctags -R \
		  --langmap=c:+.h \
		  --languages=c,c++
#		  --links=yes \
#		  -I __THROW,__wur,__nonnull+  \
#		  --c++-kinds=+p --c-kinds=+px-n \
#		  --fields=+ialtfS --extra=+q \
#		  --file-scope=yes \
#		  $PWD
echo "preparations over" && ls -lh tags

}

if [[ -f tags ]] ;then
	read -p "		update tags file ? <y/n> " select
	if [[ "$select" == "y" ]] ;then
		rm tags
		genctags
	else
		echo "Do not update tags"
	fi
else
	genctags
fi

