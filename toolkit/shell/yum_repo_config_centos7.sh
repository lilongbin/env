#! /bin/bash
# created by longbin <beangr@163.com>
# 2017-10-15
## yum and yum plugins installation scripts

PLAT_ARCH=$(uname -i | tr 'A-Z' 'a-z')

## check whether system is centos or not
function check_system_distributor() {
## get system distributor ID: centos ?
LINUX_DISTRIBUTOR=$(cat /etc/issue |tr 'A-Z' 'a-z'|awk ' /release/ {print $1}' | sed -n "1p")
LINUX_DISTRIBUTOR=${LINUX_DISTRIBUTOR:=$(lsb_release -i |tr 'A-Z' 'a-z'|awk '/distributor/ {print $3}')}
LINUX_DISTRIBUTOR=${LINUX_DISTRIBUTOR:=$(cat /etc/*release |tr 'A-Z' 'a-z'|awk '/\<release\>/ {print $1}'|sed -n '1p')}
LINUX_DISTRIBUTOR=${LINUX_DISTRIBUTOR:=$(cat /etc/*release |tr 'A-Z' 'a-z'|awk '/distrib_id=/ {print $1}'|sed 's/distrib_id=//'|sed -n '1p')}

	echo "checking system distributor and release ID ..."
	if [[ "${LINUX_DISTRIBUTOR}" == "centos" ]] ;then
		echo -e "\tCurrent OS Distributor: ${LINUX_DISTRIBUTOR}"
	else
		echo -e "\tCurrent OS is not centos"
		echo -e "\tCurrent OS Distributor: ${LINUX_DISTRIBUTOR}"
		exit 1
	fi
}

## check whether system is centos 7.* or 7.4
function check_system_release_version() {
## get system release version: 7.*/7.4 ?
LINUX_RELEASE_VERSION=$(cat /etc/issue | awk '/release/ {print $3}'| sed -n '1p')
LINUX_RELEASE_VERSION=${LINUX_RELEASE_VERSION:=$(lsb_release -r | tr 'A-Z' 'a-z' | awk '/release/ {print $2}')}
LINUX_RELEASE_VERSION=${LINUX_RELEASE_VERSION:=$(cat /etc/*release |tr 'A-Z' 'a-z'|awk '/\<release\>/ {print $3}'|sed -n '1p')}
LINUX_RELEASE_VERSION=${LINUX_RELEASE_VERSION:=$(cat /etc/*release |tr 'A-Z' 'a-z'|awk '/distrib_release=/ {print $1}'|sed 's/distrib_release=//'|sed -n '1p')}
	case ${LINUX_RELEASE_VERSION:0:5} in
		6.*)
			echo -e "\tCurrent OS Version: ${LINUX_RELEASE_VERSION}"
			;;
		7.*)
			echo -e "\tCurrent OS Version: ${LINUX_RELEASE_VERSION}"
			;;
		*)
			echo "Only support centos version, eg: 6.6/7.4 ..."
			exit 1
			;;
	esac
	echo "checked OK, preparing to setup softwares ..."
}

function check_user_UID() {
	sudo ls > /dev/null
	if [ "x$?" == "x1" ] ;then
		echo -e "\tThere is a configuration/permission problem."
		echo -e "\tPlease ensure that you have the permission"
		exit 1
	fi
	if [ "x${UID}" == "x0" ] ;then
		SUDO=''
	else
		SUDO=sudo
	fi
}

function check_yum_install_env() {
	check_system_distributor
	check_system_release_version
	check_user_UID
	trap 'echo -e "\nAborted by user, exit";exit' INT
	if [ "x$(which yum 2>/dev/null)" == "x" ] ;then
		echo "ERROR: yum: command not exists, please install yum manually"
		exit 1
	fi
}

function yum_plugins_installation() {
	echo "Preparing to install yum plugins ..."
	# ${SUDO} yum install yum-*
	# ${SUDO} yum install axel
	${SUDO} yum install yum-presto
	${SUDO} yum install yum-fastestmirror
	${SUDO} yum install yum-metadata-parser
	${SUDO} yum install yum-downloadonly
	${SUDO} yum install yum-priorities
	${SUDO} yum install epel-release
}

function yum_axel_plugin_install() {
	${SUDO} yum install axel
#	if [ "x${PLAT_ARCH}" == "xx86_64" ] ;then
#		${SUDO} rpm -ivh http://pkgs.repoforge.org/axel/axel-2.4-1.el6.rf.x86_64.rpm
#	else
#		${SUDO} rpm -ivh http://pkgs.repoforge.org/axel/axel-2.4-1.el6.rf.i686.rpm
#	fi
	# pushd /etc/yum/pluginconf.d/
	# ${SUDO} wget http://cnfreesoft.googlecode.com/svn/trunk/axelget/axelget.conf
	${SUDO} wget http://www.ha97.com/code/axelget.conf -P /etc/yum/pluginconf.d/
	if [ "$?" != 0 ] ;then
		local tmp_file=$(mktemp)
		${SUDO} cat > ${tmp_file} << EOF
[main]
enabled=1
onlyhttp=1
enablesize=54000
cleanOnException=1
EOF
		${SUDO} mv ${tmp_file} /etc/yum/pluginconf.d/axelget.conf
	fi
	# popd
	# pushd /usr/lib/yum-plugins/
	# ${SUDO} wget http://cnfreesoft.googlecode.com/svn/trunk/axelget/axelget.py
	${SUDO} wget http://www.ha97.com/code/axelget.py -P /usr/lib/yum-plugins/
	if [ "$?" != 0 ] ;then
		local tmp_file=$(mktemp)
		${SUDO} cat > ${tmp_file} << EOF
from yum.plugins import PluginYumExit, TYPE_CORE, TYPE_INTERACTIVE
from urlparse import urljoin
import os,time

requires_api_version = '2.3'
plugin_type = (TYPE_CORE, TYPE_INTERACTIVE)

enablesize=300000
trymirrornum=-1
maxconn=10
httpdownloadonly=False
cleanOnException=0

def init_hook(conduit):
	global enablesize,trymirrornum,maxconn,cleanOnException,httpdownloadonly
	enablesize = conduit.confInt('main','enablesize',default=30000)
	trymirrornum = conduit.confInt('main','trymirrornum',default=-1)
	maxconn = conduit.confInt('main','maxconn',default=10)
	httpdownloadonly=conduit.confBool('main','onlyhttp',default=False)
	cleanOnException=conduit.confInt('main','cleanOnException',default=0)
	return

def predownload_hook(conduit):
	global enablesize,cleanOnException,httpdownloadonly
	preffermirror=""
	PkgIdx=0
	TotalPkg=len(conduit.getDownloadPackages())
	for po in (conduit.getDownloadPackages()):
		PkgIdx+=1
		if hasattr(po, 'pkgtype') and po.pkgtype == 'local':
			continue
		totsize = long(po.size)
		ret = False
		if totsize <= enablesize:
			conduit.info(2, "Package %s download size %d less than %d,Skip plugin!"  % (po.repo.id,totsize,enablesize))
			continue
		else:
			conduit.info(2, "[%d/%d]Ok,we will try to use axel to download this big file:%d" % (PkgIdx,TotalPkg,totsize))
		local = po.localPkg()
		if os.path.exists(local):
			if not os.path.exists(local+".st"):
				fstate=os.stat(local)
				if totsize == fstate.st_size:
					conduit.info(2,"Target already exists,skip to next file!")
					continue
		localall = "%s %s" % (local,local+".st")
		rmcmd = "rm -f %s" % (localall)
		curmirroridx = 0
		conduit.info(2,"Before we start,clean all the key files")
		os.system(rmcmd)
		connnum = totsize / enablesize
		if connnum*enablesize<totsize:
			connnum+=1
		if connnum > maxconn:
			connnum = maxconn
		mirrors=[]
		mirrors[:0]=po.repo.urls
		if preffermirror != "":
			mirrors[:0] = [preffermirror]
		for url in mirrors:
			if url.startswith("ftp://") and httpdownloadonly:
				print "Skip Ftp Site:",url
				continue
			if url.startswith("file://"):
				print "Skip Local Site:",url
				continue
			curmirroridx += 1
			if (curmirroridx > trymirrornum) and (trymirrornum != -1):
				conduit.info(2, "Package %s has tried %d mirrors,Skip plugin!" % (po.repo.id,trymirrornum))
				break
			remoteurl =  "%s/%s" % (url,po.remote_path)
			syscmd = "axel -a -n %s %s -o %s" % (connnum,remoteurl,local)
			conduit.info(2, "Execute axel cmd:\n%s"  % syscmd)
			os.system(syscmd)
			time.sleep(2)
			if os.path.exists(local+".st"):
				conduit.info(2,"axel exit by exception,let's try another mirror")
				if cleanOnException:
					conduit.info(2,"because cleanOnException is set to 1,we do remove key file first")
					os.system(rmcmd)
				continue
			elif not os.path.exists(local):#this mirror may not update yet
				continue
			else:
				ret = True
				preffermirror=url
				break
		if not ret:
			conduit.info (2,"try to run rm cmd:%s"  % rmcmd)
			os.system(rmcmd)
EOF
		${SUDO} mv ${tmp_file} /usr/lib/yum-plugins/axelget.py
	fi
	# popd
}

# modify varible value of file callback function
function modify_varible_value_of_file_cb() {
	if [ "$#" -lt 3 ] ;then
		echo "too less arguments"
		exit 1
	fi
	# echo $*
	local filename=$1
	local variable=$2
	local value=$3
	local res=$(cat ${filename}| sed -n "/^${variable}/p")
	if [ "x${res}" == "x" ] ;then
		#append variable and value to file
		${SUDO} sed -i "/^enable[d]\?[ ]\?=[ ]\?1/ a ${variable}=${value}" ${filename}
	else
		#modify variable and value to file
		${SUDO} sed -i "/^${variable}[ ]\?=/ s;${variable}.*$;${variable}=${value};" ${filename}
	fi
}

function check_yum_conf_enable_plugins() {
	local yum_conf=$(cat /etc/yum.conf| sed -n '/^plugins[ ]\?=[ ]\?1/p')
	if [ "x${yum_conf}" != "x" ] ;then
		return
	else
		# yum_conf_enable_plugins /etc/yum.conf
		modify_varible_value_of_file_cb /etc/yum.conf plugins 1
	fi
}

function backup_yum_repos_d_files() {
	local backup_dir=/etc/yum.repos.d/repos.old.$(date +%F_%T)
	${SUDO} mkdir -p ${backup_dir}
	if [ "x$?" != "x0" ] ;then
		return
	fi
	# pushd /etc/yum.repos.d
	for file in $(ls /etc/yum.repos.d/ | sed -n '/\.repo$/p') ;
	do
		if [ -f "/etc/yum.repos.d/${file}" ] ;then
			${SUDO} cp /etc/yum.repos.d/${file} ${backup_dir}
		fi
	done
	# popd
}

function create_local_repo_DVD() {
	local dvd1_iso="CentOS-7.4-x86_64-bin-DVD1.iso"
	local dvd2_iso="CentOS-7.4-x86_64-bin-DVD2.iso"
	if [ -f ${dvd1_iso} ] ;then
		${SUDO} mkdir /media/_local_repo_DVD1
		${SUDO} mount -o loop -t iso9660 ${dvd1_iso} /media/_local_repo_DVD1
	fi
	if [ -f ${dvd2_iso} ] ;then
		${SUDO} mkdir /media/_local_repo_DVD2
		${SUDO} mount -o loop -t iso9660 ${dvd2_iso} /media/_local_repo_DVD2
	fi
	local tmp_file=$(mktemp)
	${SUDO} cat > ${tmp_file} << EOF
[_local_repo_DVD]
name=CentOS-\$releasever - Media
baseurl=file:/var/lib/yum/plugins/local
		file:///media/CentOS/
		file:///mnt/cdrom/
		file:///media/cdrecorder/
		file:///media/_local_repo_DVD1
		file:///media/_local_repo_DVD2
enabled=0
priority=9
gpgcheck=1
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-CentOS-${LINUX_RELEASE_VERSION:0:1}
EOF
	${SUDO} mv ${tmp_file} /etc/yum.repos.d/_local_repo_DVD.repo
}

function create_maintain_script() {
	return #no need to create the script
	local tmp_file=$(mktemp)
	${SUDO} cat > ${tmp_file} << EOF
#! /bin/bash
read -p "backup or restore <bk/rs>? " select
if [[ "x\${select}" == "xbk" ]] ;then
	for file in \$(ls /etc/yum.repos.d/ | sed -n '/repo$/p')
	do
		sudo mv \${file} \${file}.backup
	done
elif [[ "x\${select}" == "xrs" ]] ;then
	for file in \$(ls /etc/yum.repos.d/ | sed -n '/backup$/p')
	do
		sudo mv \${file} \${file%.backup*}
	done
else
	echo "Invalid option "
fi
EOF
	${SUDO} mv ${tmp_file} /etc/yum.repos.d/_backup_restore.sh
}

# add adobe repo
function yum_add_adobe_repo() {
	if [ "x${PLAT_ARCH}" == "xx86_64" ] ;then
		#64bit#
		${SUDO} rpm -ivh http://linuxdownload.adobe.com/linux/x86_64/adobe-release-x86_64-1.0-1.noarch.rpm
	else
		#32bit#
		${SUDO} rpm -ivh http://linuxdownload.adobe.com/linux/i386/adobe-release-i386-1.0-1.noarch.rpm
	fi
	${SUDO} rpm --import /etc/pki/rpm-gpg/RPM-GPG-KEY-adobe-linux
	# set adobe-linux-x86_64.repo priority=10
	local repo_file=/etc/yum.repos.d/adobe-linux-${PLAT_ARCH}.repo
	modify_varible_value_of_file_cb ${repo_file} priority 10
	${SUDO} yum install flash-plugin
}

# add atomic repo
function yum_add_atomic_repo() {
  	#download
	${SUDO} wget http://www.atomicorp.com/installers/atomic
	${SUDO} chmod a+x ./atomic

 	#install
	yes " " | sudo sh ./atomic
	${SUDO} rm -f ./atomic
}

# add epel repo
function yum_add_epel_repo() {
#	if [ "${PLAT_ARCH}" == "xx86_64" ] ;then
#		#64bit:#
#		${SUDO} rpm -ivh http://dl.fedoraproject.org/pub/epel/6/x86_64/epel-release-6-8.noarch.rpm
#	else
#		#32bit#
#		${SUDO} rpm -ivh http://dl.fedoraproject.org/pub/epel/6/i386/epel-release-6-8.noarch.rpm
#	fi
	${SUDO} yum install epel-release
	${SUDO} rpm --import /etc/pki/rpm-gpg/RPM-GPG-KEY-EPEL-${LINUX_RELEASE_VERSION:0:1}
}

# add rpmforge repo
function yum_add_rpmforge_repo() {
	return
	# download from http://pkgs.repoforge.org/rpmforge-release/
	if [ "x${PLAT_ARCH}" == "xx86_64" ] ;then
		#64bit#
		${SUDO} wget http://pkgs.repoforge.org/rpmforge-release/rpmforge-release-0.5.3-1.el6.rf.x86_64.rpm
	else
		#32bit#
		${SUDO} wget http://pkgs.repoforge.org/rpmforge-release/rpmforge-release-0.5.3-1.el6.rf.i686.rpm
	fi
	${SUDO} rpm --import http://apt.sw.be/RPM-GPG-KEY.dag.txt
	${SUDO} rpm -ivh rpmforge-release-0.5.3-1.el6.rf.*.rpm
	${SUDO} rm -f rpmforge-release-0.5.3-1.el6.rf*.rpm
}

# add rpmfusion repo which includes many multimedia libs
function yum_add_rpmfusion_repo() {
	return
	if [ "x${PLAT_ARCH}" == "xx86_64" ] ;then
		#64bit
		${SUDO} rpm -ivh http://download1.rpmfusion.org/free/el/updates/6/x86_64/rpmfusion-free-release-6-1.noarch.rpm
		${SUDO} rpm -ivh http://download1.rpmfusion.org/nonfree/el/updates/6/x86_64/rpmfusion-nonfree-release-6-1.noarch.rpm
	else
		#32bit
		${SUDO} rpm -ivh http://download1.rpmfusion.org/free/el/updates/6/i386/rpmfusion-free-release-6-1.noarch.rpm
		${SUDO} rpm -ivh http://download1.rpmfusion.org/nonfree/el/updates/6/i386/rpmfusion-nonfree-release-6-1.noarch.rpm
	fi
}

function yum_add_nux_dextop_repo() {
	if [ "x${PLAT_ARCH}" == "xx86_64" ] ;then
		#64bit
		if [ "${LINUX_RELEASE_VERSION:0:1}" == "7" ] ;then
			${SUDO} rpm -Uvh http://li.nux.ro/download/nux/dextop/el7/x86_64/nux-dextop-release-0-5.el7.nux.noarch.rpm
		else
			${SUDO} rpm -Uvh http://li.nux.ro/download/nux/dextop/el6/x86_64/nux-dextop-release-0-2.el6.nux.noarch.rpm
		fi
	else
		#32bit
		${SUDO} rpm -Uvh http://li.nux.ro/download/nux/dextop/el6/i386/nux-dextop-release-0-2.el6.nux.noarch.rpm
	fi
	# ${SUDO} rm -f nux-dextop-release*.rpm
	modify_varible_value_of_file_cb nux-dextop.repo enable 1
}

function setup_yum_repos_priority() {
	# pushd /etc/yum.repos.d/
	REPO_FILE_LIST=" CentOS-Base.repo atomic.repo epel.repo epel-testing.repo nux-dextop.repo rpmforge.repo rpmfusion-free-updates.repo "
	repo_priority_num=1
	for file in ${REPO_FILE_LIST}
	do
		file=/etc/yum.repos.d/${file}
		if [ -f "${file}" ] ;then
			echo "updating ${file} ..."
			modify_varible_value_of_file_cb ${file} priority XXX
		else
			continue
		fi
		for ((i=0; i<5; i++)) ;
		do
			let prn=${repo_priority_num}+${i}
			# echo ${repo_priority_num} ${prn}
			${SUDO} sed "1, /^priority=XXX/ {/^priority=XXX/ s/priority=.*$/priority=${prn}/}" -i ${file}
		done
		let repo_priority_num+=5
	done
	# popd
}

function rebuild_yum_cache() {
	${SUDO} yum clean all
	${SUDO} yum makecache
}

function install_and_setup_yum_plugins() {
	yum_plugins_installation
	# yum_axel_plugin_install
}

function enable_and_backup_repos() {
	check_yum_conf_enable_plugins
	backup_yum_repos_d_files
}

function add_extra_repos() {
	create_local_repo_DVD
	# create_maintain_script
	yum_add_adobe_repo
	# yum_add_atomic_repo
	yum_add_epel_repo
	# yum_add_nux_dextop_repo
	yum_add_rpmforge_repo
	yum_add_rpmfusion_repo
	yum_axel_plugin_install
}

function setup_priority_and_rebuild_cache() {
	setup_yum_repos_priority
	rebuild_yum_cache
}

check_yum_install_env
yum_add_nux_dextop_repo
install_and_setup_yum_plugins
enable_and_backup_repos
add_extra_repos
setup_priority_and_rebuild_cache

