/*****************************************
 * Copyright (C) 2019 * Ltd. All rights reserved.
 * 
 * File name   : regexcpp.cpp
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
using namespace std;

// 提取子串
string getsubstr(string src, regmatch_t &match)
{
   size_t length = 0;
   length = match.rm_eo - match.rm_so;

   return src.substr(match.rm_so, length);
}

bool match(string re, string src)
{
   // 从字符串开始的位置开始匹配
   bool ret = false;
   vector<string> group;
   regmatch_t match;
   regex_t reg;
   string str = "";
   string p;

   // cout << "src: " << src << " re: " << re << endl;
   //编译正则表达式
   /* int regcomp(regex_t *preg, const char *pattern, int cflags) */
   regcomp(&reg, re.c_str(), REG_EXTENDED);
   int offset = 0;
   int status = 0;
   while (offset < src.length()) {
      /*  int regexec(regex_t *preg, char *str, size_t nmatch, regmatch_t pmatch[], int eflags) */
      status = regexec(&reg, src.c_str() + offset, 1, &match, 0);
      /* 匹配正则表达式，注意regexec()函数一次只能匹配一个，不能连续匹配 */
      if (status == REG_NOMATCH) {
         ret = false;
         break;
      } else if (match.rm_so != -1) {
         ret = true;
         break;
         str = src.c_str() + offset;
         p = getsubstr(str, match);
         // cout << "Match: [" << offset+match.rm_so << ", " << offset+match.rm_eo << "]: " << p << endl;
         group.push_back(p);
      }
      offset += match.rm_eo;
   }

   //释放正则表达式
   //void regfree(regex_t *preg);
   regfree(&reg);
   return ret;
}

string search(string re, string src, int cflags)
{
   // 只要找到第一个就返回，否则返回空
   string result;
   regmatch_t match;
   regex_t reg;
   string str = "";
   string p;

   // cout << "src: " << src << " re: " << re << endl;
   //编译正则表达式
   /* int regcomp(regex_t *preg, const char *pattern, int cflags) */
   regcomp(&reg, re.c_str(), REG_EXTENDED);
   int offset = 0;
   int status = 0;
   while (offset < src.length()) {
      /*  int regexec(regex_t *preg, char *str, size_t nmatch, regmatch_t pmatch[], int eflags) */
      status = regexec(&reg, src.c_str() + offset, 1, &match, 0);
      /* 匹配正则表达式，注意regexec()函数一次只能匹配一个，不能连续匹配 */
      if (status == REG_NOMATCH) {
         break;
      } else if (match.rm_so != -1) {
         str = src.c_str() + offset;
         p = getsubstr(str, match);
         // cout << "Match: [" << offset+match.rm_so << ", " << offset+match.rm_eo << "]: " << p << endl;
         result = p;
         break;
      }
      offset += match.rm_eo;
   }

   //释放正则表达式
   //void regfree(regex_t *preg);
   regfree(&reg);
   return result;
}

vector<string> findall(string re, string src)
{
   // 遍历匹配，返回一个列表
   vector<string> group;
   regmatch_t match;
   regex_t reg;
   string str = "";
   string p;

   // cout << "src: " << src << " re: " << re << endl;
   //编译正则表达式
   /* int regcomp(regex_t *preg, const char *pattern, int cflags) */
   regcomp(&reg, re.c_str(), REG_EXTENDED);
   int offset = 0;
   int status = 0;
   while (offset < src.length()) {
      /*  int regexec(regex_t *preg, char *str, size_t nmatch, regmatch_t pmatch[], int eflags) */
      status = regexec(&reg, src.c_str() + offset, 1, &match, 0);
      /* 匹配正则表达式，注意regexec()函数一次只能匹配一个，不能连续匹配 */
      if (status == REG_NOMATCH) {
         break;
      } else if (match.rm_so != -1) {
         str = src.c_str() + offset;
         p = getsubstr(str, match);
         // cout << "find: [" << offset+match.rm_so << ", " << offset+match.rm_eo << "]: " << p << endl;
         group.push_back(p);
      }
      offset += match.rm_eo;
   }

   //释放正则表达式
   //void regfree(regex_t *preg);
   regfree(&reg);
   return group;
}

string sub(string re, string replace, string src, size_t count = -1)
{
   // 替换匹配到的字符，返回替换后的字符串
   string result;
   regmatch_t match;
   regex_t reg;
   string str = "";
   string p;
   size_t cnt = 1;

   // cout << "src: " << src << " re: " << re << endl;
   //编译正则表达式
   /* int regcomp(regex_t *preg, const char *pattern, int cflags) */
   regcomp(&reg, re.c_str(), REG_EXTENDED);
   int offset = 0;
   int status = 0;
   while (offset < src.length()) {
      /*  int regexec(regex_t *preg, char *str, size_t nmatch, regmatch_t pmatch[], int eflags) */
      status = regexec(&reg, src.c_str() + offset, 1, &match, 0);
      /* 匹配正则表达式，注意regexec()函数一次只能匹配一个，不能连续匹配 */
      if (status == REG_NOMATCH) {
         result.assign(src);
         break;
      } else if (match.rm_so != -1) {
         str = src.c_str() + offset;
         p = getsubstr(str, match);
         // cout << "find: [" << offset+match.rm_so << ", " << offset+match.rm_eo << "]: " << p << endl;
      }
      if (cnt <= count)
      {
         result.append(src.c_str() + offset, 0, match.rm_so);
         result.append(replace);
      }
      else
      {
         result.append(src.c_str() + offset, 0, match.rm_eo);
      }
      cnt++;
      offset += match.rm_eo;
   }

   //释放正则表达式
   //void regfree(regex_t *preg);
   regfree(&reg);
   return result;
}

void test()
{
   vector<string> result;
   vector<string>::iterator it;
   // 正则表达式
   string pattern = "[a-z]+";
   // 待搜索的字符串
   string str = "HELLOsaiYear2012@gmail.com";
   cout << "match: " << match(pattern, str) << endl;
   result = findall(pattern, str);
   cout << "findall: ";
   for (it = result.begin(); it != result.end(); it++)
   {
      cout << *it << " ";
   }
   cout << endl;

   cout << "raw str: " << str << endl;
   cout << "sub str: " << sub(pattern, "***", str) << endl;
   cout << "sub str: " << sub(pattern, "###", str, 2) << endl;
   cout << "sub str: " << sub("#$%&@", "###", str, 2) << endl;
}

#if 0
int main(void)
{
   test();
   return 0;
}
#endif

