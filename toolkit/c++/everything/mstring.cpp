/*****************************************
* Copyright (C) 2019 * Ltd. All rights reserved.
* 
* File name   : mstring.cpp
* Author      : longbin
* Created date: 2019-02-13 17:16:14
* Description : 
*
*******************************************/

#include <iostream>

#include <string>
#include <map>
#include <vector>
#include "mstring.h"
using namespace std;

vector<string> split(string &src, const string & delm)
{
   vector<string> list;
   string::size_type pos1 = 0;
   string::size_type pos2 = 0;
   
   while (pos2 = src.find(delm, pos1))
   {
      if (string::npos == pos2) break;
      list.push_back(src.substr(pos1, pos2-pos1));
      pos1 = pos2 + delm.size();
   }
   if (src.length() != pos1)
   {
      list.push_back(src.substr(pos1));
   }
   return list;
}

