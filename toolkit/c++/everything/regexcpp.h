/*****************************************
 * Copyright (C) 2019 * Ltd. All rights reserved.
 * 
 * File name   : regexcpp.h
 * Author      : longbin
 * Created date: 2019-02-14 14:55:35
 * Description : 
 *
 *******************************************/

#include <iostream>
#include <cstdio>
#include <cstring>
#include <regex.h>
#include <string>
#include <map>
#include <vector>
#include <regex.h>
using namespace std;

// 提取子串
string regetsubstr(string src, regmatch_t &match);
bool rematch(string re, string src);
string research(string re, string src, int cflags=0);
vector<string> refindall(string re, string src);
string resub(string re, string replace, string src, size_t count=-1);

