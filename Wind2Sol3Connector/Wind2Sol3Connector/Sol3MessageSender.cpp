#include "boost/thread.hpp"
#include "boost/chrono/duration.hpp"
#include "Sol3MessageSender.h"
#include "MarketData2LuaTable.h"
#include "MyLog.h"

namespace logging = boost::log;
namespace src = boost::log::sources;

namespace sol3_message_sender
{
	Sol3MessageSender::Sol3MessageSender()
	{
		_m_nPort = -1;
		_m_pmOutputMessages = NULL;
		_m_psktSoket = NULL;
		_m_ServerAddress[0] = 0;
	}

	void Sol3MessageSender::SetMemoryQueue(sol3_memory_queue::CMemoryQueue<sol3_message_generator::out_msg::CBaseOutMessage *> *pmqQueue)
	{
		_m_pmOutputMessages = pmqQueue;
	}

	void Sol3MessageSender::SetServerInfo(char * szIP, int Port)
	{
		if (szIP == NULL || Port <= 0)
			return;
		strcpy(_m_ServerAddress, szIP);
		_m_nPort = Port;
	}

	void Sol3MessageSender::Start()
	{
		_m_thWorkingThread.reset(new boost::thread(&Sol3MessageSender::Processing, this));
		_m_thWorkingThread->detach();
		_m_thHeartBeatThread.reset(new boost::thread(&Sol3MessageSender::HeartBeat, this));
		_m_thHeartBeatThread->detach();
	}

	void Sol3MessageSender::Stop()
	{
		_m_thWorkingThread->interrupt();
		_m_thHeartBeatThread->interrupt();
	}

	void Sol3MessageSender::Processing()
	{
		char szOutBuffer[1024];
		char szBatchOutBuffer[4096];
		_m_psktSoket = new SocketObject();

		src::severity_logger_mt<logging::trivial::severity_level>&lg = my_logger::get();
		BOOST_LOG_SEV(lg, logging::trivial::severity_level::info) << "Message Sender Started!";

		if (_m_psktSoket == NULL || _m_ServerAddress == NULL || _m_nPort < 0)
		{
			BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "Message Sender Stopped!";
			return;
		}

		_m_SockectLocker.lock();
		while (_m_psktSoket->Connect(_m_ServerAddress, _m_nPort) != true)
		{
			boost::this_thread::interruption_point();
			boost::this_thread::sleep_for(boost::chrono::duration<int, boost::milli>(1000));
			BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "Connection to Sol3 Failed!" << _m_ServerAddress << ":" << _m_nPort;
		}
		_m_SockectLocker.unlock();
		BOOST_LOG_SEV(lg, logging::trivial::severity_level::info) << "Connection to Sol3 Succeed!" << _m_ServerAddress << ":" << _m_nPort;

		while (true)
		{
			try
			{
				boost::this_thread::interruption_point();

				out_msg::CBaseOutMessage *msg = NULL;

				int nret = _m_pmOutputMessages->nPop(&msg);
				size_t stMsgCount = _m_pmOutputMessages->tSize();

				int nbatchSize = stMsgCount > 10 ? 10 : stMsgCount;

				szBatchOutBuffer[0] = 0;
				int nMsgToSend = 0;

				if (nret == 0)
				{
					do {
						if (msg != NULL)
						{
							szOutBuffer[0] = 0;
							MarketData2Sol3::nGetSol3Message(msg, szOutBuffer, sizeof(szOutBuffer) - 1);
						}
						else
							continue;

						strcat(szBatchOutBuffer, szOutBuffer);
						nMsgToSend = 1;
						delete msg;
						msg = NULL;
						--nbatchSize;

						if (nbatchSize > 0)
							nret = _m_pmOutputMessages->nPop(&msg);
						else
							break;
					} while (nret == 0);
				}

				if (nMsgToSend == 1)
				{
					BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "Message to Send out:" << szBatchOutBuffer;
					/*
					if (msg->cMsgType == in_msg::FutureOption)
					{

					}
					*/
//					if (msg != NULL)
					{
					//	szOutBuffer[0] = 0;
					//	MarketData2Sol3::nGetSol3Message(msg, szOutBuffer, sizeof(szOutBuffer) - 1);
					//	delete msg;
					//	msg = NULL;
						if (szBatchOutBuffer[0] != 0)
						{
							//
							_m_SockectLocker.lock();
							nret = _m_psktSoket->Send(szBatchOutBuffer, static_cast<int>(strlen(szBatchOutBuffer)) + 1, 0);
							_m_SockectLocker.unlock();
//							printf("sendout=[%s]\n", szBatchOutBuffer);
							_m_bHasMessageSent = 1;
							if (nret == SOCKET_ERROR)
							{
								if (_m_psktSoket != NULL)
								{
									_m_SockectLocker.lock();
									BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "Message to Send out Error!";
									_m_psktSoket->Disconnect();
									delete _m_psktSoket;
									_m_psktSoket = new SocketObject();

									while (_m_psktSoket->Connect(_m_ServerAddress, _m_nPort) != true)
									{
										BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "Reconnect to Sol3 Server Failed!";
										boost::this_thread::interruption_point();
										boost::this_thread::sleep_for(boost::chrono::duration<int, boost::milli>(1000));
									}
									_m_SockectLocker.unlock();
									BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "Reconnect to Sol3 Server Succeed!";
								}
							}

						}

					}
				}
			}
			catch (boost::thread_interrupted& /*e*/)
			{
			}
			catch (boost::exception & )
			{
				std::cout << "boost exception" << endl;
			}
		}
		BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "Message Sender Thread Exit!";
	}

	void Sol3MessageSender::HeartBeat()
	{
		src::severity_logger_mt<logging::trivial::severity_level>&lg = my_logger::get();
		while (true)
		{
			boost::this_thread::interruption_point();
			boost::this_thread::sleep_for(boost::chrono::duration<int, boost::milli>(30000));
			if (_m_bHasMessageSent > 0)
			{
				_m_bHasMessageSent = 0;
			}
			else
			{
				char * msg = "heartbeat {}\n";
				_m_SockectLocker.lock();

				try
				{
					_m_psktSoket->Send(msg, static_cast<int>(strlen(msg)) + 1, 0);
					BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "Heart Beat is sent out!";
					_m_bHasMessageSent = 0;
				}
				catch (...)
				{

				}
				_m_SockectLocker.unlock();
			}
		}
	}

}
