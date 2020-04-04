/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : singleton.h
* Author      : longbin
* Created date: 2020-01-03 13:36:48
* Description : 
*
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
   static T* m_pInstance;
   Singleton(const Singleton& src){(void)src;}
   Singleton &operator=(const Singleton& src){(void)src;};

   class Garbo
   {
   public:
      ~Garbo()
      {
         if (Singleton::m_pInstance)
         {
            delete Singleton::m_pInstance;
            Singleton::m_pInstance = NULL;
         }
      }
   };
   static Garbo garbo;

protected:
   Singleton(){}
   ~Singleton(){}

public:
   static T* getInstance()
   {
      if (m_pInstance == NULL)
      {
         m_pInstance = new T();
      }
      return m_pInstance;
   }
};

template <class T>
T* Singleton<T>::m_pInstance = NULL;

#endif
