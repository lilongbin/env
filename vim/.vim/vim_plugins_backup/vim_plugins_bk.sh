#! /bin/bash

FILE_NAME=plugins_bk_${USER}
if [[ -f "${FILE_NAME}.zip" ]] ;then
	rm -v ${FILE_NAME}.zip
fi
sleep 1

pushd ../
zip -r vim_plugins_backup/${FILE_NAME}.zip colors/ doc/ ftplugin/ nerdtree_plugin/ plugin/ syntax/ tools/ indent/
popd

