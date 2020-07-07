/*****************************************
 * Copyright (C) 2019 * Ltd. All rights reserved.
 * File name   : singleton.hpp
 * Created date: 2019-07-25 18:45:05
 *******************************************/

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <cstdio>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <condition_variable>
#include <algorithm>

template <class T>
class Singleton
{
private:
    static std::mutex ms_MuxLock_Singleton;
    using MuxGuard = std::lock_guard<std::mutex>;
    static T* ms_pInstance;
    Singleton(const Singleton& src){(void)src;}
    Singleton &operator=(const Singleton& src){(void)src;};

    class Garbo
    {
    public:
        ~Garbo()
        {
            // std::cout<<"Singleton<"<<typeid(T).name()<<">::Garbo::~Garbo()" << std::endl;
            if (Singleton::ms_pInstance)
            {
                delete Singleton::ms_pInstance;
                Singleton::ms_pInstance = NULL;
            }
        }
        void touch() { return; }
    };
    static Garbo ms_garbo;

protected:
    Singleton() {
        ms_garbo.touch(); //prevent optimised and no garbo instance to trigger deconstruct
    }
    ~Singleton() {}

public:
    static T* getInstance()
    {
        if (ms_pInstance == NULL)
        {
            MuxGuard mlk(ms_MuxLock_Singleton);
            if (ms_pInstance == NULL)
            {
                ms_pInstance = new T();
            }
        }
        return ms_pInstance;
    }
};

template <class T> std::mutex Singleton<T>::ms_MuxLock_Singleton;
template <class T> typename Singleton<T>::Garbo Singleton<T>::ms_garbo;
template <class T> T* Singleton<T>::ms_pInstance = NULL;

#endif //__SINGLETON_H__

