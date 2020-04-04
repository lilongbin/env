/*****************************************
 * Copyright (C) 2019 * Ltd. All rights reserved.
 * 
 * File name   : testSequence.cpp
 * Author      : longbin
 * Created date: 2019-07-25 18:59:14
 * Description : 
 *
 *******************************************/

#include <iostream>

#include <string>
#include <map>
#include <vector>
#include "sequenceId.hpp"

using namespace std;
int main() {
	Header_SequenceID * m_sequence = Header_SequenceID::getInstance();
	unsigned int id = 0;
	int i = 0;
	for (i = 0; i <= 10; i++)
	{
		id = m_sequence->getSequenceId();
		cout << id << endl;
	}
	return 0;
}
/*
 * usage:
 * g++ testSequence.cpp sequenceId.cpp
 * */
