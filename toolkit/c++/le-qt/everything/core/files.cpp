/*****************************************
 * Copyright (C) 2019 * Ltd. All rights reserved.
 * 
 * File name   : files.cpp
 * Author      : longbin
 * Created date: 2019-02-13 15:58:19
 * Description : 
 *
 *******************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

#include "files.h"
using namespace std;

vector<string> gFileList;

enum {
	F_FIFO = 1,
	F_CHR, F_DIR, F_REG, F_BLK, F_LNK, F_SOCK
};

int is_file(const char *file, struct stat *st) {
	/* 判断文件类型 */
	switch (st->st_mode & S_IFMT) {
		case S_IFIFO:
			return F_FIFO;
		case S_IFCHR:
			return F_CHR;
		case S_IFDIR:
			return F_DIR;
		case S_IFBLK:
			return F_BLK;
		case S_IFREG:
			return F_REG;
		case S_IFLNK:
			return F_LNK;
		case S_IFSOCK:
			return F_SOCK;
		default:
			return 0;
	}
}
void print_finode(const char *file, struct stat *st) {
	/* 获取文件的inode节点 */
	printf("%7ld ", st->st_ino);
}

void print_ftype(const char *argv, struct stat *st) {
	switch (st->st_mode & S_IFMT) {
		case S_IFIFO:
			printf("p");
			break;
		case S_IFCHR:
			printf("c");
			break;
		case S_IFDIR:
			printf("d");
			break;
		case S_IFBLK:
			printf("b");
			break;
		case S_IFREG:
			printf("-");
			break;
		case S_IFLNK:
			printf("l");
			break;
		case S_IFSOCK:
			printf("s");
			break;
		default:
			break;
	}
}

string ftype(struct stat &st) {
	string type = "-";
	switch (st.st_mode & S_IFMT) {
		case S_IFIFO:
			type.assign("p");
			break;
		case S_IFCHR:
			type.assign("c");
			break;
		case S_IFDIR:
			type.assign("d");
			break;
		case S_IFBLK:
			type.assign("b");
			break;
		case S_IFREG:
			type.assign("-");
			break;
		case S_IFLNK:
			type.assign("l");
			break;
		case S_IFSOCK:
			type.assign("s");
			break;
		default:
			break;
	}
	return type;
}

void print_fmod(const char *argv, struct stat *st) {
	/* 权限是与07777做位与;第一个7是用来表示设置位的; */
	char permission[10] = "rwxrwxrwx";
	unsigned int mode = (unsigned int)st->st_mode;
	int bit = 0;
	for (bit = 0; bit < 9; bit++) {
		if (!(mode & (0x1 << bit))) {
			permission[8-bit] = '-';	
		}
	}
	if (st->st_mode & S_ISUID)
		permission[2] = 's';	
	if (st->st_mode & S_ISGID)
		permission[5] = 's';	
	if (st->st_mode & S_ISVTX)
		permission[8] = 't';
	printf("%s ", permission);
}

void print_fnlink(const char *file, struct stat *st) {
	/* 获取文件的硬链接数 */
	printf("%2ld ", (long)st->st_nlink);
}

void print_fusr(const char *file, struct stat *st) {
	/* 获取文件的属主和属组的ID */
	/* printf("%4d %4d  ", st->st_uid, st->st_gid); */
	/*
	 * struct passwd *passwd = getpwuid(st->st_uid);
	 * printf("%-8s ", passwd->pw_name);
	 * struct group *grp = getgrgid(st->st_gid);
	 * printf("%-8s ", grp->gr_name);
	 */
}

void print_fsize(const char *file, struct stat *st) {
	switch (is_file(file, st)) {
		case F_DIR:
		case F_REG:
		case F_LNK:
			/* 文件大小 */
			printf("%8lld ", (long long)st->st_size);
			break;
		case F_BLK:
		case F_CHR:
			/* 特殊设备的主次设备号 */
			printf("%3d, %3d ",
					major(st->st_rdev), minor(st->st_rdev));
			break;
		case F_SOCK: 
		case F_FIFO:
			/* 文件大小 */
			printf("%8lld ", (long long)st->st_size);
			break;
	}
}

string fsize(string file, struct stat &st) {
	string size = "0";
	stringstream ss;
	switch (is_file(file.c_str(), &st)) {
		case F_DIR:
		case F_REG:
		case F_LNK:
			/* 文件大小 */
			ss << ((long long)st.st_size);
			break;
		case F_BLK:
		case F_CHR:
			/* 特殊设备的主次设备号 */
			ss << (major(st.st_rdev));
			//major(st.st_rdev), minor(st.st_rdev));
			break;
		case F_SOCK:
		case F_FIFO:
			/* 文件大小 */
			ss << ((long long)st.st_size);
			break;
	}
	ss >> size;
	return size;
}

void print_ftime(const char *file, struct stat *st) {
	/* struct tm *mytime = localtime(&(st->st_atime)); */
	struct tm *mytime = localtime(&(st->st_ctime));
	/* struct tm *mytime = localtime(&(st->st_mtime)); */
	char time[32] = {};
	const char *mon[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dev"};
	sprintf(time, "%s %2d %02d:%02d ",
			mon[mytime->tm_mon], mytime->tm_mday,
			mytime->tm_hour, mytime->tm_min);
	printf("%s", time); //打印文件相关的时间
}


string ftime(struct stat &st) {
	string mtime;
	/* struct tm *mytime = localtime(&(st.st_atime)); */
	/* struct tm *mytime = localtime(&(st.st_ctime)); */
	struct tm *mytime = localtime(&(st.st_mtime));
	char time[32] = {};
	const char *mon[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dev"};
	sprintf(time, "%02d-%02d-%02d %02d:%02d ",
			1900+mytime->tm_year, 1+mytime->tm_mon, mytime->tm_mday,
			mytime->tm_hour, mytime->tm_min);
	// printf("%s", time); //打印文件相关的时间
	mtime = time;
	return mtime;
}

void print_fname(const char *file, struct stat *st) {
	printf("%s\n", file); //处理文件名
}

void prettyPrint(string filename)
{
	const char *file = filename.c_str();
	struct stat st;
	if (stat(file, &st) == -1) {
		// printf("stat: %m\n");
		return ;
	}
	if (!is_file(file, &st)) {
		printf("%s: No such file or directory\n", file);
		return ;
	}
	/* print_finode(file, &st); */
	print_ftype(file, &st);
	print_fmod(file, &st);
	print_fnlink(file, &st);
	// print_fusr(file, &st);
	print_fsize(file, &st);
	print_ftime(file, &st);
	print_fname(file, &st);
	//printf("\n");
}

vector<string> getattr(string filename)
{
	// cout << __func__ << " filename: " << filename << endl;
	if (filename.size() == filename.find_last_of("/")+1)
	{
		filename = filename.substr(0, filename.find_last_of("/"));
	}
	// cout << __func__ << " filename: " << filename << endl;
	vector<string> attr;
	struct stat st;
	const char *file = filename.c_str();
	if (-1 == stat(file, &st))
	{
		return attr;
	}
	if (!is_file(file, &st)) {
		cout << file << ": No such file or directory" << endl;
		return attr;
	}
	/* type name path size mtime */
	string type = ftype(st);
	// cout << "type: " << type << endl;
	string name = filename.substr(filename.find_last_of("/")+1);
	// cout << "name: " << name << endl;
	string path = filename.substr(0, filename.find_last_of("/"));
	// cout << "path: " << path << endl;
	string size = fsize(filename, st);
	if (size.size() < 10)
	{
		size.append(10 - size.size(), ' ');
	}
	string mtime = ftime(st);

	attr.push_back(type);
	attr.push_back(name);
	attr.push_back(path);
	attr.push_back(size);
	attr.push_back(mtime);
	return attr;
}

void myls_il(char *file) {
	string filename = file;
	// prettyPrint(file);
	gFileList.push_back(filename);
}

void myls_rec(char *path) {
	struct stat st;
	if (stat(path, &st) == -1) {
		printf("stat: %s %m\n", path);
		return ;
	}
	if (!S_ISDIR(st.st_mode)) return ;
	DIR *dir = opendir(path);
	if (dir == NULL) {
		printf("opendir(%s): %m\n", path);
		return;
	}
	struct dirent *ent;
	while (ent = readdir(dir)) {
		char buf[256] = {};
		snprintf(buf, sizeof(buf), "%s/%s", path, ent->d_name);
		if (strcmp(".", ent->d_name) == 0 ||
				strcmp("..", ent->d_name) == 0) {
			continue;
		}
		if (ent->d_type == 4) {
			/* 如果目标是目录,递归调用 */
			myls_rec(buf);
		} else {
			myls_il(buf);
		}
	}
	closedir(dir); //回溯
}

void myls_ial(char *file) {
	struct stat st;
	if (stat(file, &st) == -1) {
		printf("stat: %s %m\n", file);
		return ;
	}
	if (!is_file(file, &st)) {
		printf("%s: No such file or directory\n", file);
		return ;
	}
	if (!S_ISDIR(st.st_mode)) {
		myls_il(file);
	} else {
		/* 去除路径结尾的/ */
		while (strlen(file) > 1) {
			if (file[strlen(file) - 1] == '/') {
				file[strlen(file) - 1] = '\0';
			} else {
				break;
			}
		}
		/* 调用递归函数处理目录 */
		myls_rec(file);
	}
}

void getFileList(string path, vector<string>& files)
{
	myls_ial((char *)path.c_str());
	files = gFileList;
}

