/*****************************************
 * Copyright (C) 2019 * Ltd. All rights reserved.
 * 
 * File name   : everything.h
 * Author      : longbin
 * Created date: 2019-02-13 12:48:54
 * Description : 
 *
 *******************************************/

#include <iostream>

#include <string>
#include <map>
#include <vector>
// #include <regex>
#include <fstream>
#include <iterator>

#include "files.h"
#include "mstring.h"
#include "regexcpp.h"

using namespace std;

string getLine(string &prompt);
bool reg_search(string str, string expr);
void dbinit();
void dbsave();
int dbload();
void dbshow();
vector<string> getflist(vector<string> &findkey);
vector<string> search(string &line);
int proc(string line);

