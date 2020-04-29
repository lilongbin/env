#! /bin/bash

LOG_FILE=~/vmdiskshrink.log
DISK_LIST=$(sudo /usr/bin/vmware-toolbox-cmd disk list)
echo -e "will shrink disk: \n############\n${DISK_LIST}\n############" | tee ${LOG_FILE}
for mydisk in ${DISK_LIST}
do
    if [ ! -d ${mydisk} ] ;then
        continue
    fi
    read -t 10 -p "shrink ${mydisk} ?<Y/n>" myselect
    if [ "${myselect,,}" = "n" ] ;then
        continue
    fi
    echo "start wipe ${mydisk}" | tee -a ${LOG_FILE}
    sudo /usr/bin/vmware-toolbox-cmd disk wipe ${mydisk}
    echo "start shrink ${mydisk}" | tee -a ${LOG_FILE}
    sudo /usr/bin/vmware-toolbox-cmd disk shrink ${mydisk}
done

