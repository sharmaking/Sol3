// MemoryQueue.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "boost/container/deque.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread.hpp"
#include "boost/thread/condition_variable.hpp"
#include "boost/chrono/duration.hpp"
#include <stdarg.h>
#include "MemoryQueue.h"
namespace sol3_memory_queue
{
	

	void Debug(char *pszFormat, ...)
	{
		va_list   pArgList;

		va_start(pArgList, pszFormat);
		int nByteWrite = vfprintf(stdout, pszFormat, pArgList);
		va_end(pArgList);

		return ;
	}


	CMemoryQueueManager * CMemoryQueueManager::mqGetQueueHandle()
	{
		_m_mLock.lock();
		if (_m_pSelf == NULL)
		{
			_m_pSelf = new CMemoryQueueManager;
		}
		_m_mLock.unlock();
		return _m_pSelf;
	}

	void * CMemoryQueueManager::pvGetQueueByName(char * szQueueName)
	{
		if (szQueueName == NULL)
			return NULL;
		boost::container::map<char *,void *, ptrCmp>::iterator it = _m_QList.begin();

		_m_mLock.lock();
		it = _m_QList.find(szQueueName);
		_m_mLock.unlock();

		if (it != _m_QList.end())
		{
			return it->second;
		}
		return NULL;
	}

	CMemoryQueueManager::CMemoryQueueManager()
	{
	}

	int CMemoryQueueManager::nAppendQueue(char * szQueueName, void * pvQueue)
	{
		if (szQueueName == NULL)
			return -1;
		_m_mLock.lock();
		if (_m_QList.size() == 0)
		{
			_m_QList.insert(std::pair <char *, void *>(szQueueName, pvQueue));
		}
		else
		{
			boost::container::map<char *, void *, ptrCmp>::iterator it;
			it = _m_QList.find(szQueueName);
			if (it == _m_QList.end())
			{
				_m_QList.insert(std::pair <char *, void *>(szQueueName, pvQueue));
			}
			else
			{
				_m_QList[szQueueName] = pvQueue;
			}
		}
		_m_mLock.unlock();
		return 0;
	}

	void CMemoryQueueManager::Init()
	{
		CMemoryQueueManager::mqGetQueueHandle();
	}

	boost::mutex CMemoryQueueManager::_m_mLock;

	CMemoryQueueManager * CMemoryQueueManager::_m_pSelf;

}

class CMsgType
{
public:
	int nId;
};

/*
sol3_memory_queue::CMemoryQueue<CMsgType> m;
void vInsertMsg()
{

	CMsgType msg;
	for (int i = 1; i < 10000; i++)
	{
		msg.nId = i;
		while (m.nPush(msg) != 0)
			;
		//boost::this_thread::sleep_for(boost::chrono::duration<int, boost::milli>(1));
		
	}
}

void vPopMsg()
{
	CMsgType msg;
	while (true)
	{
		while(m.nPop(msg) != 0)
			;
		if (msg.nId == 9999)
			std::cout << msg.nId << std::endl;
	}
}
void test()
{
	boost::thread t2(vPopMsg);
	boost::thread t1(vInsertMsg);
	t1.join();
	t2.join();

}
int _tmain(int argc, _TCHAR* argv[])
{
	test();
	char str1[10];
	std::cin >> str1;
	return 0;
}
*/

