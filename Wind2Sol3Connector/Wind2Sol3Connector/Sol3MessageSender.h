#pragma once

#include "SocketObject.h"
#include "MemoryQueue.h"
#include "Sol3MessageGenerator.h"
#include "boost/thread.hpp"
#include <boost/atomic.hpp>
#include "boost/thread/mutex.hpp"

namespace sol3_message_sender
{
	class Sol3MessageSender
	{
	public:
		Sol3MessageSender();
		void SetMemoryQueue(sol3_memory_queue::CMemoryQueue<sol3_message_generator::out_msg::CBaseOutMessage *>*);
		void SetServerInfo(char * szIP, int Port);
		void Start();
		void Stop();
		void Processing();
		void HeartBeat();
	private:
		sol3_memory_queue::CMemoryQueue<sol3_message_generator::out_msg::CBaseOutMessage *> * _m_pmOutputMessages;
		SocketObject * _m_psktSoket;
		shared_ptr<boost::thread> _m_thWorkingThread;
		shared_ptr<boost::thread> _m_thHeartBeatThread;
		char _m_ServerAddress[256];
		mutable boost::atomic<int> _m_bHasMessageSent;
		int	 _m_nPort;
		boost::mutex  _m_SockectLocker;
	};
}
