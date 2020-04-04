/*****************************************
 * Copyright (C) 2019 * Ltd. All rights reserved.
 * 
 * File name   : tboxservice.cpp
 * Author      : longbin
 * Created date: 2019-07-23 13:28:35
 * Description : 
 *
 *******************************************/

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>
#include "itbox.pb.h"
#include "singleton.hpp"
#include "sequenceId.hpp"

using namespace std;

typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;

typedef struct TBox_Msg_Header_Tag
{
	uint16_t Flag;
	uint32_t SequenceID;
	uint32_t CommandID;
	uint16_t Length;
} TBox_Msg_Header_T;

class TBOX:public Singleton<TBOX>
{
	friend class Singleton<TBOX>;
	public:
		~TBOX();
		void handleCommunicationSyncDebug(TBox_Msg_Header_T header, uint8_t * payloadbuf);
		void handleCommunicationSyncNotify();
	private:
		TBOX();
		Header_SequenceID* m_sequence;
};

TBOX::~TBOX()
{
}
TBOX::TBOX()
{
	m_sequence = Header_SequenceID::getInstance();
}

void TBOX::handleCommunicationSyncDebug(TBox_Msg_Header_T header, uint8_t * payloadbuf)
{
	assert(NULL != payloadbuf);
	bool sync = 0;
	string radioInformation;
	uint32_t syncSerialNumber = 0;
	RadioInterfaceTBox::RadioTxCommunicationSyncNotify communicationsync;
	RadioInterfaceTBox::RadioInfo radioInfo;

	cout << "handleCommunicationSyncDebug begin" << endl;
	cout << "flag: " << header.Flag << ", "
		 << "sequenceId: " << header.SequenceID << ", "
		 << "commandId: " << header.CommandID << ", "
		 << "length: " << header.Length << endl;

	/* ParseFromArray */
	if (false == communicationsync.ParseFromArray(payloadbuf, header.Length))
	{
		cout << "handleCommunicationSyncDebug error: " << header.Length << endl;
		return;
	}

	cout << "protobuf parsed data: " << endl;
	sync = communicationsync.sync();
	radioInformation = communicationsync.radioinformation();
	syncSerialNumber = communicationsync.syncserialnumber();
	cout << "sync: " << sync << ", "
		 << "radioInformation: " << radioInformation <<", "
		 << "syncSerialNumber: " << syncSerialNumber << endl;
	radioInfo = communicationsync.radiobasicinfo();
	cout << "radioInfo.radiohmiversion: " << radioInfo.radiohmiversion() << ", "
		 << "radioInfo.radiofortboxserviceversion: " << radioInfo.radiofortboxserviceversion() << ", "
		 << "radioInfo.communicationprotocolversion: " << radioInfo.communicationprotocolversion() << endl;
}

void TBOX::handleCommunicationSyncNotify()
{
	uint32_t m_syncNo = 0;
	uint8_t * payloadbuf = NULL;
	RadioInterfaceTBox::RadioTxCommunicationSyncNotify communicationsync;
	RadioInterfaceTBox::RadioInfo * pRadioinfo = communicationsync.mutable_radiobasicinfo();

	/* set radioinfo */
	pRadioinfo->set_radiohmiversion("2019-07-23");
	pRadioinfo->set_radiofortboxserviceversion("2.1.1");
	pRadioinfo->set_communicationprotocolversion("2.17");

	/* set communicationsync */
	communicationsync.set_sync(false);
	communicationsync.set_radioinformation("Local NGI 2.0 System");
	communicationsync.set_syncserialnumber(m_syncNo++);

	TBox_Msg_Header_T header;
	header.Flag       = RadioInterfaceTBox::MHF_MsgTypeNotification;
	header.SequenceID = m_sequence->getSequenceId();
	header.CommandID  = RadioInterfaceTBox::eCmd_RadioTxCommunicationSyncNotify;
	header.Length     = communicationsync.ByteSize();

	/* SerializeToArray */
	payloadbuf = new uint8_t[header.Length];
	if (NULL == payloadbuf)
	{
		// warning;
		cout << "allocate payloadbuf failed: " << header.Length << endl;
		return;
	}
	if (false == communicationsync.SerializeToArray(payloadbuf, header.Length))
	{
		cout << "SerializeToArray failed: " << header.Length << endl;
		// warning;
	}

	handleCommunicationSyncDebug(header, payloadbuf);
}

int main() {
	TBOX * tbox = TBOX::getInstance();
	tbox->handleCommunicationSyncNotify();
	tbox->handleCommunicationSyncNotify();
	tbox->handleCommunicationSyncNotify();
	tbox->handleCommunicationSyncNotify();
	tbox->handleCommunicationSyncNotify();
	tbox->handleCommunicationSyncNotify();
	return 0;
}


