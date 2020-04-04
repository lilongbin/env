/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : main.cpp
* Author      : longbin
* Created date: 2020-01-06 12:57:36
* Description : 
*
*******************************************/

#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <fstream>
#include <iterator>

#include "everything.h"

#include "xservice/comm.h"
#include "xservice/msgqueue.h"
#include "xservice/service.h"
#include "xservice/eventcallback.h"

bool is_waiting_ack = false;

class CallBack:public EventCallback
{
public:
	CallBack(){}
	~CallBack(){}
	void onResponse(MsgQueueType msg)
	{
		std::string line;
		// printf("%s %s msg.header.cmdId=%d\n", __FILE__,  __func__, msg.header.cmdId);
		line.assign(msg.vpayload.begin(), msg.vpayload.end());
		proc(line);
		is_waiting_ack = false;
	}
};

CallBack * m_callback = new CallBack();
Service * m_service = Service::getInstance();
MsgSequenceID * msgSequence = MsgSequenceID::getInstance();
bool is_service_inited_ok = false;

void service_init()
{
	m_service->start();
	m_service->registerEventCallback(m_callback);
	is_service_inited_ok = true;
}

void service_request(std::string &line)
{
	if (is_service_inited_ok != true) {
		cout << "service_init not completed." << endl;
		return;
	}
	MsgQueueType msg;
	std::vector<uint8_t> vstring(line.c_str(), line.c_str()+line.length());
	msg.header.cmdId = msgSequence->getSequenceID();
	msg.vpayload = std::move(vstring);
	msg.header.payloadLength = msg.vpayload.size();
	m_service->onRequest(msg);
	// cout << "service_request ok: " << line << endl;
}

void service_run_test()
{
	service_init();
	std::string line;
	while (1)
	{
		std::cout << "please input your words: ";
		std::getline(std::cin, line);
		service_request(line);
		usleep(200);
		if (line.length() == 0)
		{
			break;
		}
	}
}

int console(string line)
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
		// service_request(line);
		return 0;
	}

	std::string prompt = "\033[01mSearch: \033[0m";
	std::cout << "Please type the file name you want to search, or q to quit." << endl;
	// cout << line << endl;
	while (1)
	{
		if (is_waiting_ack == true) {
			usleep(100);
			continue;
		}
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
		//send msg to sender
		// proc(line);
		service_request(line);
		is_waiting_ack = true;
	}

	return 0;
}

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
	service_init();
	console(line);
	return 0;
}

