#pragma once
#include "boost/container/deque.hpp"
#include "boost/container/map.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread.hpp"
#include "boost/thread/condition_variable.hpp"
#include "boost/chrono/duration.hpp"
#include <stdarg.h>
#include "MemoryQueue.h"


namespace sol3_memory_queue
{
	void Debug(char *, ...);
	template <typename T>
	class CMemoryQueue
	{

	public:
		CMemoryQueue();
		int nPush(T );
		int nPop(T* );
		size_t tSize();
		void vSetName(char * szQueueName);
	private:
		boost::container::deque<T> _m_dqDataQueue;
		boost::mutex _m_mLock;
		char *	_m_szQueueName[10];
		size_t 		_m_tSize;
		boost::condition_variable _m_cvDataArrived;
	};

	struct ptrCmp
	{
		bool operator()(const char * s1, const char * s2) const
		{
//			return strcmp(s1, s2) < 0;
			if (!(s1 && s2))
				return s1 < s2;
			else
				return strcmp(s1, s2) < 0;
		}
	};

	class CMemoryQueueManager
	{
	public:
		static	void Init();
		static CMemoryQueueManager * mqGetQueueHandle();
		void * pvGetQueueByName(char * szQueueName);
		int		nAppendQueue(char * szQueueName, void * pvQueue);
	private:
		static boost::mutex _m_mLock;
		static CMemoryQueueManager * _m_pSelf;
		boost::container::map<char *, void *, ptrCmp> _m_QList;
		CMemoryQueueManager();
		
	};


	template <typename T>
	void sol3_memory_queue::CMemoryQueue<T>::vSetName(char * szQueueName)
	{
		if (szQueueName == NULL || strlen(szQueueName) >= 10 || strlen(szQueueName) == 0)
			return;
		strcpy(_m_szQueueName, szQueueName);
		return;
	}

	template <typename T>
	size_t sol3_memory_queue::CMemoryQueue<T>::tSize()
	{
		while (!_m_mLock.try_lock_for(boost::chrono::duration<int, boost::milli>(5000)))
		{
			Debug("Get Memory Queue Time Out while push Data\n");
		}
		size_t s = _m_dqDataQueue.size();
		_m_mLock.unlock();
		return s;
	}

	template <typename T>
	sol3_memory_queue::CMemoryQueue<T>::CMemoryQueue()
	{
		_m_tSize = 5000;
		memset(_m_szQueueName, 0, sizeof(_m_szQueueName));
	}

	/*
	Return: 0, succesfully pop one message from queue
	-1, wait for msg time out;
	*/

	template <typename T>
	int sol3_memory_queue::CMemoryQueue<T>::nPop(T *oMsg)
	{
		boost::unique_lock<boost::mutex> lck(_m_mLock, boost::defer_lock);

		while (!lck.try_lock_for(boost::chrono::duration<int, boost::milli>(5000)))
		{
			//Debug("Get Memory Queue Time Out while pop Data\n");
		}
		if (_m_dqDataQueue.size() == 0)
		{
			//	_m_mLock.unlock();
			if (_m_cvDataArrived.wait_for<int, boost::milli>(lck, boost::chrono::duration<int, boost::milli>(5000)) == boost::cv_status::timeout)
			{
				//timed out
				if (lck.owns_lock())
					lck.unlock();
			//	Debug("Wait For Message Time Out while pop Data\n");
				return -1;
			}
			lck.unlock();
		}
		else if (_m_dqDataQueue.size() > 0)
		{
			*oMsg = _m_dqDataQueue[0];
			_m_dqDataQueue.pop_front();
			lck.unlock();
			return 0;
		}
		else
		{
			//should not reached here
			lck.unlock();
			return 0;
		}
		return -2;
	}

	/*
	return: -1, means Queue is full and this data is not pushed into queue
	0, means Data is pushed into Queue and Notification are sent out.
	*/
	template <typename T>
	int sol3_memory_queue::CMemoryQueue<T>::nPush(T  Msg)
	{

		while (!_m_mLock.try_lock_for(boost::chrono::duration<int, boost::milli>(5000)))
		{
			Debug("Get Memory Queue Time Out while push Data\n");
		}
		if (_m_dqDataQueue.size() > _m_tSize)
		{
			Debug("Memory Queue is full wait for pricessing,[%d]", _m_dqDataQueue.size());
			_m_mLock.unlock();
			return -1;
		}
		_m_dqDataQueue.push_back(Msg);
		_m_mLock.unlock();
		_m_cvDataArrived.notify_all();

		return 0;
	}
}