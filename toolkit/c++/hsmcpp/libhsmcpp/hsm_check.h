/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : hsm_check.h
* Author      : longbin
* Created date: 2020-08-31 17:06:00
* Description : 
*
*******************************************/

#ifndef __HSM_CHECK_H__
#define __HSM_CHECK_H__

#include "hsm_comm.h"
#include "hsm_state.h"

class HSM_Check
{
private:
    HSM_State_Definition_T mStateDfn;

public:
    HSM_Check(const HSM_State_Definition_T &stateDfn) {
        mStateDfn = stateDfn;
    }
    ~HSM_Check() {}
    bool check() {
        return true;
    }
};

#endif //__HSM_CHECK_H__

