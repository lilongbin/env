/*****************************************
 * Copyright (C) 2019 * Ltd. All rights reserved.
 * 
 * File name   : everything.cpp
 * Author      : longbin
 * Created date: 2019-02-13 12:48:54
 * Description : 
 *
 *******************************************/

#include <iostream>

#include <stdlib.h>

#include <string>
#include <algorithm>

#include <map>
#include <vector>
// #include <regex>
#include <fstream>
#include <iterator>

#include "files.h"
#include "mstring.h"
#include "regexcpp.h"

#define le_Version "build@" __DATE__
using namespace std;

map<string, vector<string> > gFileMap;
map<string, vector<string> > globaldb;
string g_home(getenv("HOME"));
static const char *g_dbfile = "le.dat";

void version()
{
	cout << le_Version << endl;
}

void usage()
{
	string helpinfo = "Usage:\n\tle [--init | --help | -h | filename]\n\tle";
	cout << helpinfo << endl;
}

string getLine(string &prompt)
{
	std::string line = "";
	std::cout << prompt;
	std::getline(std::cin, line);
	return line;
}

bool reg_search(string str, string expr)
{
	// cout << __func__ << ": [" << str << "] [" << expr << "]" << endl;
	bool flag = false;
	string ret;
#if 0
	smatch result;
	regex pattern(expr, regex::icase);

	string::const_iterator iter = str.begin();
	string::const_iterator iterEnd = str.end();
	while (std::regex_search(iter, iterEnd, result, pattern))
	{
		cout << "hello" << endl;
		cout << result[0] << endl;
		iter = result[0].second;
		continue;
		for (auto x = result.begin(); x != result.end(); x++)
		{
			flag = true;
			cout << *x << endl;
		}
		str = result.suffix().str();
	}
#endif
	ret = research(expr, str, REG_EXTENDED|REG_ICASE);
	if (ret.length())
	{
		flag = true;
	}
	return flag;
}

void dbinit()
{
	// cout << __func__ << endl;
	string path;
	vector<string> dirs;
	vector<string>::iterator dit;
	string key;
	string value;
	vector<string> FileList;
	vector<string>::iterator sit;
	map<string, vector<string> >::iterator mit;
	int enm = 0;
	int idx = 0;

	/* dbinit */
	getFileList(".", FileList);

	/* add files */
	for (sit = FileList.begin(); sit != FileList.end(); sit++)
	{
		value = *sit;
		path = value;
		// cout << __func__ << ": " <<  path << endl;
		dirs = split(path, "/");
		enm = 0;
		/* add keys for file */
		for (dit = dirs.begin(); dit != dirs.end(); dit++, enm++)
		{
			key = *dit;
			// cout << __func__ << " key: " <<  key << endl;
			if (0 == key.compare("."))
			{
				continue;
			}
			if (dirs.end() - 1 != dit)
			{
				/* is dir, only save dir prefix path */
				value = "";
				for (idx = 0; idx <= enm; idx++)
				{
					value += dirs[idx] + "/";
				}
				// cout << __func__ << " value1: " <<  value << endl;
			}
			else
			{
				/* is file */
				value = *sit;
			}
			// cout << __func__ << " value: " <<  value << endl;
			mit = gFileMap.find(key);
			if (gFileMap.end() != mit)
			{
				/* dir name alread exists in map */
				vector<string>::iterator ist;
				for (ist = gFileMap[key].begin(); ist != gFileMap[key].end(); ist++)
				{
					if (0 == value.compare(*ist))
					{
						/* found */
						break;
					}
				}
				if (ist != gFileMap[key].end())
				{
					/* found */
					/* do nothing */
				}
				else
				{
					gFileMap[key].push_back(value);
				}
			}
			else
			{
				/* dir name not exists in map*/
				gFileMap[key].push_back(key);
				gFileMap[key].push_back(value);
			}
		}
	}
}

void dbsave()
{
	/* dbsave */
	vector<string>::iterator dit;
	map<string, vector<string> >::iterator mit;
	ofstream ofs(g_dbfile);
	std::ostream_iterator<std::string> of_it(ofs, ", ");
	for (mit = gFileMap.begin(); mit != gFileMap.end(); mit++)
	{
		// cout << mit->first << ": ";
		std::copy(mit->second.begin(), mit->second.end(), of_it);
		ofs << endl;
		for (dit = mit->second.begin()+1; dit != mit->second.end(); dit++)
		{
			break;
			cout << *dit << ", ";
		}
		// cout << endl;
	}
}

int dbload()
{
	// cout << __func__ << endl;
	/* dbload */
	ifstream ifs(g_dbfile);
	std::string line;
	std::string key;
	vector<string> value;
	globaldb.clear();
	int ret = 0;

	while (ifs)
	{
		std::getline(ifs, line);
		// cout << "read: " << line << ";"<< endl;
		if (!line.size())
		{
			break;
		}
		value = split(line, ", ");
		key = value[0];
		value.erase(value.begin());
		globaldb[key] = value;
	}
	if (globaldb.empty())
	{
		cout << "Warning: load db file failed: " << g_dbfile << endl;
		usage();
		ret = 1;
	}
	return ret;
}

void dbshow()
{
	/* dbshow */
	vector<string>::iterator dit;
	map<string, vector<string> >::iterator mit;

	for (mit = globaldb.begin(); mit != globaldb.end(); mit++)
	{
		break;
		cout << mit->first << ": ";
		for (dit = mit->second.begin(); dit != mit->second.end(); dit++)
		{
			cout << *dit << ", ";
		}
		cout << endl;
	}
}

vector<string> getflist(vector<string> &findkey)
{
	vector<string>::iterator kit;
	vector<string>::iterator it;
	vector<string> flist;

	for (kit = findkey.begin(); kit != findkey.end(); kit++)
	{
		//cout << "key: " << *kit<< endl;
		for (it = globaldb[*kit].begin(); it != globaldb[*kit].end(); it++)
		{
			//cout << "file: " << *it << endl;
			flist.push_back(*it);
		}
	}
	return flist;
}

vector<string> search(string &line)
{
	// cout << __func__ << endl;
	std::string key;
	map<string, vector<string> >::iterator mit;
	vector<string> findkey;
	vector<string>::iterator it;

	size_t fpos = 0;
	while (string::npos != (fpos = line.find(" ", fpos)))
	{
		line.replace(fpos, 1, ".*");
	}
	for (mit = globaldb.begin(); mit != globaldb.end(); mit++)
	{
		key = mit->first;
		// cout << "match key: " << key << endl;
		if (reg_search(key, line))
		{
			// cout << "find out keyword: " << key << endl;
			findkey.push_back(key);
			// prettyPrint(key.c_str());
			continue;
			for (it = globaldb[key].begin(); it != globaldb[key].end(); it++)
			{
				prettyPrint(*it);
			}
		}
	}
	return findkey;
}

int proc(string line)
{
	vector<string> findkey;
	findkey = search(line);
	if (!findkey.size())
	{
		cout << "no match: " << line << endl;
		return 1;
	}
	// cout << "match: " << line << endl;
	vector<string> flist = getflist(findkey);
	vector<string>::iterator fit;
	vector<string> attr;
	vector<string>::iterator itattr;
	string filename = "";
#if 1
	for (fit = flist.begin(); fit != flist.end(); fit++)
	{
		filename = *fit;
		prettyPrint(filename);
	}
#else
	string str = "";
	size_t max_filename_len = 0;
	int idx = 0;
	for (fit = flist.begin(); fit != flist.end(); fit++)
	{
		filename = *fit;
		filename = filename.substr(filename.find_last_of("/")+1);
		if (max_filename_len < filename.size())
		{
			max_filename_len = filename.size();
		}
	}
	if (max_filename_len > 40)
	{
		max_filename_len = 40;
	}	
	for (fit = flist.begin(); fit != flist.end(); fit++)
	{
		str = "";
		idx = 0;
		filename = *fit;
		attr = getattr(filename);
		filename = filename.substr(filename.find_last_of("/")+1);
		for (itattr = attr.begin(); itattr != attr.end(); itattr++, idx++)
		{
			str.append(*itattr);
			if ((1 == idx)
					&& (max_filename_len > filename.size()))
			{
				str.append(max_filename_len - filename.size(), ' ');
			}
			str.append(" ");
		}
		cout << str << endl;
	}
# endif
	cout << endl;
	return 0;
}

int console(string line="")
{
	int ret = 0;
	/*
	 * dbinit();
	 * dbsave();
	 */
	ret = dbload();
	if (0 != ret)
   	{
		exit(0);
	}
	dbshow();

	if ((0 != line.size())
			&& (0 != line.compare("q")))
	{
		proc(line);
		return 0;
	}

	std::string prompt = "\033[01mSearch: \033[0m";
	std::cout << "Please type the file name you want to search, or q to quit." << endl;
	// cout << line << endl;
	while (1)
	{
		line = getLine(prompt);
		if (0 == line.size())
		{
			// cout << endl;
			continue;
		}
		if (0 == line.compare("q"))
		{
			break;
		}
		proc(line);
	}

	return 0;
}

#if 1
int main (int argc, char* argv[])
{
	// cout << argc << endl;
	string line = "";
	if (argc >= 2)
	{
		string arg1 = argv[1];
		int cnt = 0;
		transform(arg1.begin(), arg1.end(), arg1.begin(), ::tolower);
		// cout << arg1 << endl;
		if ((0 == arg1.compare("-h"))
				|| (0 == arg1.compare("--help"))
				|| (0 == arg1.compare("?")))
		{
			usage();
			return 0;
		}
		else if ((0 == arg1.compare("--version"))
				|| (0 == arg1.compare("-v")))
		{
			version();
			return 0;
		}
		else if ((0 == arg1.compare("--init"))
				|| (0 == arg1.compare("init")))
		{
			dbinit();
			dbsave();
			cout << "init OK." << endl;
			return 0;
		}
		else
		{
			for (cnt = 1; cnt < argc; cnt++)
			{
				line.append(argv[cnt]);
				line.append(" ");
			}
		}
	}
	console(line);
	return 0;
}
#endif

