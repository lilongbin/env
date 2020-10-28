#! /bin/bash
# created by Longbin_Li <beangr@163.com>
clear
echo "This script used to generate cscope files and tags for android source code"
FILENAME=cscope.files

function gen_cscope_indexer()
{
    local manual=1

    if [ "${manual}" = "1" ] ;then
        echo "Searching *.h *.c *.cc *.hpp *.cpp ... from $PWD"
        # sed use % to replace if used var contains of "/"
        find . -name "out" -prune \
            -o -iname "*.[hc]" \
            -o -iname "*.[hc]pp" \
            -o -iname "*.cc" \
            > ${FILENAME}
            #| sed -n "s%^\.%$PWD%p" > ${FILENAME}
        if [ "$?" != "0" ] ;then
            echo "error occured, ${FILENAME} not be generated"
            exit 1
        fi
        [ -s ${FILENAME} ] && echo "generate files: " && ls -lh ${FILENAME} 

        echo "generating cscope index files ..."
        cscope -bkq -i ${FILENAME}
    else
        #cscope-indexer -r
        cscope -Rbq
    fi
    [ $? == 0 ] && echo "generated files: " && ls -lh cscope*
}

function create_cscope_files() {
    if [ -f cscope.out ] ;then
        read -p "        update cscope files ? <y/n> " select
        if [ "${select,,}" == "y" ] ;then
            rm cscope.out
            gen_cscope_indexer
        else
            echo "not updated cscope files"
        fi
    else
        gen_cscope_indexer
    fi
}


function gen_ctags_indexer()
{
    echo "generating tags file ..."
    #ctags -R *
    ctags -R \
        --langmap=c:+.h \
        --languages=c,c++
    #          --links=yes \
    #          -I __THROW,__wur,__nonnull+  \
    #          --c++-kinds=+p --c-kinds=+px-n \
    #          --fields=+ialtfS --extra=+q \
    #          --file-scope=yes \
    #          $PWD
    echo "generating tags file complete" && ls -lh tags
}

function create_ctags_files() {
    if [ -f tags ] ;then
        read -p "		update tags file ? <y/n> " select
        if [ "${select,,}" == "y" ] ;then
            rm tags
            gen_ctags_indexer
        else
            echo "not updated tags"
        fi
    else
        gen_ctags_indexer
    fi
}

create_cscope_files
create_ctags_files

