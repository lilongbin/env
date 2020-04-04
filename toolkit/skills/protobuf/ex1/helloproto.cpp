/*****************************************
 * Copyright (C) 2019 * Ltd. All rights reserved.
 * 
 * File name   : helloproto.cpp
 * Author      : longbin
 * Created date: 2019-07-22 15:16:35
 * Description : 
 *
 *******************************************/

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>
#include "test.pb.h"
using namespace std;

int main() {
	int length = 0;
	/* 首先设置各字段的value，然后序列化 */
	Info *pinfo = new Info();
	pinfo->set_name("testname");
	pinfo->set_age(655384);
	cout << "info.name=" << pinfo->name() << ", age=" << hex << pinfo->age() << endl;
	length = pinfo->ByteSize();
	uint8_t *buf = new uint8_t[length];
	pinfo->SerializeToArray(buf, length); /* 序列化 */

	for (int i = 0; i < length; i++)
	{
		printf("%02X(%c) ", buf[i], buf[i]);
		/* cout << hex << buf[i] << " "; */
	}
	cout << endl; 

	/* 首先解序列化，然后获取各字段的value; */
	Info *pinfo2 = new Info();
	pinfo2->ParseFromArray(buf, length);
	cout << pinfo2->name() << endl;
	cout << pinfo2->age()  << endl;

	delete buf;
	delete pinfo;
	delete pinfo2;
	return 0;
}
