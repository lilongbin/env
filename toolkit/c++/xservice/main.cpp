/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* File name   : main.cpp
* Author      : longbin
*******************************************/

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "comm.h"
#include "msgqueue.h"
#include "service.h"
#include "eventcallback.h"

#include "log.h"

class CallBack:public EventCallback
{
public:
	CallBack(){}
	~CallBack(){}
	void onResponse(MsgQueueType msg)
	{
        std::string str(msg.vpayload.begin(), msg.vpayload.end());
        printf("%s %s seqId=%d, cmdId=%d, payload=%s\n",
                __FILE__,  __func__, msg.header.seqId, msg.header.cmdId, str.c_str());
		printf("%s %s put your callback func here\n", __FILE__,  __func__);
	}
};

Service * m_service = Service::getInstance();
CallBack * m_callback = new CallBack();
MsgSequenceID * msgSequence = MsgSequenceID::getInstance();

void service_init()
{
	m_service->start();
	m_service->registerEventCallback(m_callback);
}

void service_request(std::string &line)
{
	MsgQueueType msg {};
	std::vector<uint8_t> vc(line.c_str(), line.c_str()+line.length());
	msg.header.seqId = msgSequence->getSequenceID();
	msg.header.cmdId = msgSequence->getSequenceID();
	msg.vpayload = std::move(vc);
	msg.header.payloadLength = msg.vpayload.size();
	m_service->onRequest(msg);
}

void service_run_test()
{
	service_init();
	std::string line;
	while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
		std::cout << "please input your words: ";// << std::endl;
		std::getline(std::cin, line);
		service_request(line);
		if (line.length() == 0) {
			break;
		}
	}
}

int main() {
	service_run_test();
	m_service->stop();
}

