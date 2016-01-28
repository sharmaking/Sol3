// Wind2Sol3Connector.cpp : Defines the entry point for the console application.
//

#pragma warning(disable:4503)

#include "stdafx.h"

#include <iostream>
#include <fstream>

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/detail/thread_id.hpp>
#include <boost/program_options.hpp>
#include <boost/move/utility.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/thread.hpp>
#include <boost/chrono/duration.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/atomic.hpp>

#include "../GetDataAPI/include/GetDataStruct.h"
#include "../GetDataAPI/include/GetDataApi.h"
//#include "boost/container/vector.hpp"
#include "Sol3MessageGenerator.h"
#include "Sol3MessageSender.h"
#include "MemoryQueue.h"

#include "MyLog.h"

using namespace sol3_message_generator;
using namespace sol3_memory_queue;

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace po = boost::program_options;

using boost::shared_ptr;

std::vector<char*> g_subOptionCode;

//[ example_tutorial_formatters_stream_date_time
void init(logging::trivial::severity_level sLevel)
{
	try
	{
		// Create a text file sink
		typedef sinks::synchronous_sink< sinks::text_file_backend > file_sink;

		boost::shared_ptr< file_sink > sink(new file_sink(
			keywords::file_name = "%Y%m%d_%H%M%S_%5N.log",      // file name pattern
			keywords::rotation_size = 16384000                     // rotation size, in characters
			));

		// Set up where the rotated files will be stored
		sink->locked_backend()->set_file_collector(sinks::file::make_collector(
			keywords::target = "logs",                          // where to store rotated files
			keywords::max_size = 16 * 1024 * 1024 * 100,              // maximum total size of the stored files, in bytes
			keywords::min_free_space = 100 * 1024 * 1024        // minimum free space on the drive, in bytes
			));

		// Upon restart, scan the target directory for files matching the file_name pattern
		sink->locked_backend()->scan_for_files();

		sink->set_formatter
			(
			expr::format("%1%: [%2%] [%5%] -<%4%> %3%")
			% expr::attr< unsigned int >("RecordID")
			% expr::attr< boost::posix_time::ptime >("TimeStamp")
			% expr::smessage
			% logging::trivial::severity
			% expr::attr< boost::log::aux::thread::id >("ThreadID")
			);
		//% expr::attr< boost::log::aux::thread::id >("ThreadID")
		sink->set_filter(logging::trivial::severity >= sLevel);

		sink->locked_backend()->auto_flush(true);

		// Add it to the core
		logging::core::get()->add_sink(sink);

		// Add some attributes too
		logging::add_common_attributes();
		logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
		logging::core::get()->add_global_attribute("RecordID", attrs::counter< unsigned int >());
		logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());

		// Do some logging
		return;
	}
	catch (std::exception& e)
	{
		std::cout << "FAILURE: " << e.what() << std::endl;
	}
}



template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
	copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " "));
	return os;
};

class MySpi : public GetDataSpi{

public:
	mutable boost::atomic<int> m_isDisconnect;

	virtual void OnRecvDayBegin(GD_ISODateTimeType pDate){
		src::severity_logger_mt<logging::trivial::severity_level>&lg = my_logger::get();
		BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "Day Begin:" << pDate;
	};			//接收当前交易日
	virtual void OnRecvDayEnd(GD_ISODateTimeType pDate){
		src::severity_logger_mt<logging::trivial::severity_level>&lg = my_logger::get();
		BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "Day End:" << pDate;
	};			//接收到闭市消息

	virtual void OnRecvCode(TDF_CODE* pCode, int codeNum, TDF_OPTION_CODE* pOptionCode, int optionCodeNum){
		char sztmpBuf[32];
		sztmpBuf[0] = 0;

		std::ofstream out_file("codelist.txt");

		for (int i = 0; i < optionCodeNum; i++)
		{
			char* newCode = new char[1024];
			strcpy(newCode, (pOptionCode + i)->basicCode.szCode);
			g_subOptionCode.push_back(newCode);
			int a = ((pOptionCode)->nExerciseDate);
			sprintf(sztmpBuf, "%d;%d;%d", (pOptionCode + i)->nExerciseDate % 10000 / 100, (pOptionCode + i)->nExerciseDate / 10000, (pOptionCode + i)->nExerciseDate);
			//printf( "%d;%d;%d", 20151217 % 10000 / 100, (pOptionCode + i)->nExerciseDate / 10000, (pOptionCode + i)->nExerciseDate);

			out_file << "O;XSHG;" << newCode << ";50ETF;" << "OPtions on 50 ETF;" << "CNY;" << "1;" << "CN;" << "1;" << sztmpBuf << ";11:30:00;" << (pOptionCode + i)->chCallOrPut << ";" << (pOptionCode + i)->nExercisePrice / 10000.0 << ";510050" << ";XSHG" << ";P" << std::endl;
		}

		out_file.close();

	};		//代码表

	virtual void OnRecvFuture(TDF_FUTURE_DATA* pFuture){
		//m_date.push_back(pFuture);
		std::cout << pFuture->szCode << ", " << pFuture->nTradingDay << ", " << pFuture->nTime << ", " << pFuture->nClose << std::endl;
	};

	virtual void OnRecvFutureOption(TDF_FUTURE_DATA* pFuture){
		//m_date.push_back(pFuture);
		std::cout << pFuture->szCode << ", " << pFuture->nTradingDay << ", " << pFuture->nTime << ", " << pFuture->nClose << std::endl;
		in_msg::CFutureOptionInputMessage *msg = new in_msg::CFutureOptionInputMessage(*pFuture);

		src::severity_logger_mt<logging::trivial::severity_level>&lg = my_logger::get();
		BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "TICK_DATA:" << pFuture->nTradingDay << "," << pFuture->nTime << "," << pFuture->szCode << ", " << pFuture->nAskPrice[0] << ", " << pFuture->nAskVol[0] << "," << pFuture->nBidPrice[0] << "," << pFuture->nBidVol[0] << ", " <<pFuture->nPreClose<<"," <<pFuture->nPreSettlePrice<<","<<pFuture->nClose<<","<<pFuture->nSettlePrice<<"," <<pFuture->nStatus;
		if (!isValidPrice(pFuture))
		{
			BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "TICK_ERROR:" << pFuture->nTradingDay << "," << pFuture->nTime << "," << pFuture->szCode << ", " << pFuture->nAskPrice[0] << ", " << pFuture->nAskVol[0] << "," << pFuture->nBidPrice[0] << "," << pFuture->nBidVol[0] << ", " <<pFuture->nAskPrice[1]<<","<<pFuture->nBidPrice[1]<< pFuture->nStatus;

		}
		if (msg == NULL)
		{
			BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "msg is null";
		}
		while (_m_pmqWindDataQueue->nPush(msg) != 0)
		{
			//boost::thread::sleep(boost::chrono::duration(200));
			boost::this_thread::sleep_for(boost::chrono::duration<int, boost::milli>(20));
		}
	};

	virtual void OnRecvRspOver(){
		std::cout << "rspOver" << std::endl;
		m_isDisconnect = -1;
	};

	bool isValidPrice(TDF_FUTURE_DATA * sPrice)
	{
		if (sPrice->nAskPrice[0] > sPrice->nBidPrice[0])
		{
			if (sPrice->nAskPrice[1] > sPrice->nBidPrice[1])
				return true;
			else
				return false;
		}
		else
			return false;
	}
	MySpi() :m_isDisconnect(1)
	{
		//CMemoryQueue<in_msg::CBaseInputMessage*> *q;
		CMemoryQueueManager * qmHandle = CMemoryQueueManager::mqGetQueueHandle();
		_m_pmqWindDataQueue = (CMemoryQueue<in_msg::CBaseInputMessage*> *)(qmHandle->pvGetQueueByName("WindDataQueue"));
	}
	~MySpi(){};

	virtual void OnRecvMarket(TDF_MARKET_DATA* pMarket)
	{
		in_msg::CStockInputMessage *msg = new in_msg::CStockInputMessage(*pMarket);

		src::severity_logger_mt<logging::trivial::severity_level>&lg = my_logger::get();
		BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "STOCK_DATA_MARKET:" << pMarket->nTradingDay << "," << pMarket->nTime << "," << pMarket->szCode << ", " << pMarket->nAskPrice[0] << ", " << pMarket->nAskVol[0] << "," << pMarket->nBidPrice[0] << "," << pMarket->nBidVol[0] << ", " <<pMarket->nPreClose << "," <<pMarket->iVolume << "," << pMarket->nMatch << "," <<  pMarket->nStatus;

		if (msg == NULL)
		{
			BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "msg is null";
		}
		while (_m_pmqWindDataQueue->nPush(msg) != 0)
		{
			//boost::thread::sleep(boost::chrono::duration(200));
			boost::this_thread::sleep_for(boost::chrono::duration<int, boost::milli>(20));
		}
	}

	virtual void OnDisconnect()
	{
		src::severity_logger_mt<logging::trivial::severity_level>&lg = my_logger::get();
		BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "Received Discconect signal!";
		m_isDisconnect = 1;
	}

	virtual void OnError(string error)
	{
		src::severity_logger_mt<logging::trivial::severity_level>&lg = my_logger::get();
		BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "Received OnError signal!" << error;
	}

	virtual void OnRecvIndex(TDF_INDEX_DATA* pIndex)
	{
		//throw std::exception("The method or operation is not implemented.");
		int i = 0;
		return;
	}

	virtual void OnRecvTransaction(TDF_TRANSACTION* pTransaction)
	{
		//throw std::exception("The method or operation is not implemented.");
		in_msg::CStockInputMessage *msg = new in_msg::CStockInputMessage(*pTransaction);

		src::severity_logger_mt<logging::trivial::severity_level>&lg = my_logger::get();
		BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "STOCK_DATA_TRANS:" << pTransaction->nTime << "," << pTransaction->nPrice << "," << pTransaction->nVolume << ", " << pTransaction->nBSFlag << ", " << pTransaction->szCode;

		if (msg == NULL)
		{
			BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "msg is null";
		}
		while (_m_pmqWindDataQueue->nPush(msg) != 0)
		{
			//boost::thread::sleep(boost::chrono::duration(200));
			boost::this_thread::sleep_for(boost::chrono::duration<int, boost::milli>(20));
		}
	}

private:
	sol3_memory_queue::CMemoryQueue<in_msg::CBaseInputMessage *> *_m_pmqWindDataQueue;
	//src::severity_logger_mt<logging::trivial::severity_level>&tick_lg;
};




int _tmain(int ac, char * av[])
{

	/*
	Parsing command Line
	*/

//	int opt;
//	int portnum;
	int nDataHostPort;
	int nSol3HostPort;
	char szSol3Host[255];
	char szMarketDataHost[255];
	char szLogLevel[32];
	int	 nIsSimulation;
	std::string sDuration[2];
	logging::trivial::severity_level sLevel;

	try {
		po::options_description desc("Allowed options");
		desc.add_options()
			("help", "produce help message")
			("verbose,v", po::value<int>()->implicit_value(1), "enable verbosity (optionally specify level)")
			("log-level", po::value<std::string>()->default_value("debug"), "log level, debug/info/trace")
			("sol3-host", po::value< std::string >()->default_value("116.228.204.38"), "Sol3 Host Address ")
			("sol3-port", po::value<int>(&nSol3HostPort)->implicit_value(12343)->default_value(12343, "no"), "listen on a port.")
			("input-file", po::value< std::vector<std::string> >(), "input file")
			("marketdata-host", po::value<std::string>()->default_value("116.228.204.37"), "Exchange Market Data Host")
			("marketdata-port", po::value<int>(&nDataHostPort)->implicit_value(18500)->default_value(18500, "no"), "exchange market data listen on a port.")
			("simulation", po::value<int>(&nIsSimulation)->implicit_value(0)->default_value(0, "no"), "replay history data!")
			("duration", po::value<std::string>()->default_value("2016-01-19 14:55:00;2016-01-19 15:10:00"), "the simulation duration.")
			("stocks", po::value<std::string>()->default_value("510050"), "the underlying ETFs,510050 ")
			;

		po::positional_options_description p;
		//		p.add("input-file", -1);

		po::variables_map vm;
		po::store(po::command_line_parser(ac, av).
			options(desc).positional(p).run(), vm);
		po::notify(vm);

		if (vm.count("help")) {
			std::cout << "Usage: options_description [options]\n";
			std::cout << desc;
			return 0;
		}

		if (vm.count("sol3-host"))
		{
			std::cout << "sol3 host: "
				<< vm["sol3-host"].as< std::string >() << "\n";
		}
		else
		{
			std::cout << desc;
			return 0;
		}

		if (vm.count("duration"))
		{
			std::string tmpDuratoin = vm["duration"].as<std::string>();
			std::string::size_type d = tmpDuratoin.find_first_of(";");
			if (d < tmpDuratoin.length())
			{
				sDuration[0] = tmpDuratoin.substr(0, d);
				sDuration[1] = tmpDuratoin.substr(d+1, tmpDuratoin.length() - d);
			}
		}
		strcpy(szSol3Host, vm["sol3-host"].as< std::string >().c_str());

		if (vm.count("marketdata-host"))
		{
			std::cout << "Market Data host: " << vm["marketdata-host"].as< std::string >() << "\n";
		}
		else
		{
			std::cout << desc;
			return 0;
		}
		strcpy(szMarketDataHost, vm["marketdata-host"].as< std::string >().c_str());

		if (vm.count("marketdata-port"))
		{
			std::cout << "Market Data Port:" << nDataHostPort << std::endl;
		}
		else
		{
			std::cout << desc;
			return 0;
		}
		if (vm.count("sol3-port"))
		{
			std::cout << "Sol3 Port: " << nSol3HostPort << "\n";
		}
		else
		{
			std::cout << desc;
			return 0;
		}
		if (vm.count("log-level")) {
			std::cout << "Log Level is " << vm["log-level"].as<std::string>() << "\n";
		}
		if (vm.count("stocks")) {
			std::string tmpStocks = vm["stocks"].as< std::string >();
			while (tmpStocks.length() > 0)
			{
				std::string::size_type d = tmpStocks.find_first_of(";");
				if (d < tmpStocks.length())
				{
					char *tmpStockCode = new char[32];
					tmpStockCode[0] = 0;
					strcpy(tmpStockCode, tmpStocks.substr(0, d).c_str());
					g_subOptionCode.push_back(tmpStockCode);
				}
				else
				{
					char *tmpStockCode = new char[32];
					tmpStockCode[0] = 0;
					strcpy(tmpStockCode, tmpStocks.c_str());
					g_subOptionCode.push_back(tmpStockCode);
					break;
				}

				tmpStocks = tmpStocks.substr(d + 1, tmpStocks.length() - d - 1);
			}
		}
		strcpy(szLogLevel, vm["log-level"].as< std::string >().c_str());
		/*
		enum severity_level
		{
		trace,
		debug,
		info,
		warning,
		error,
		fatal
		};*/
		if (strcmp(szLogLevel, "trace") == 0)
		{
			sLevel = logging::trivial::severity_level::trace;
		}
		else if (strcmp(szLogLevel, "debug") == 0)
		{
			sLevel = logging::trivial::severity_level::debug;
		}
		else if (strcmp(szLogLevel, "info") == 0)
		{
			sLevel = logging::trivial::severity_level::info;
		}
		else if (strcmp(szLogLevel, "warning") == 0)
		{
			sLevel = logging::trivial::severity_level::warning;
		}
		else if (strcmp(szLogLevel, "error") == 0)
		{
			sLevel = logging::trivial::severity_level::error;
		}
		else if (strcmp(szLogLevel, "fatal") == 0)
		{
			sLevel = logging::trivial::severity_level::fatal;
		}


	}
	catch (std::exception& e)
	{
		std::cout << e.what() << "\n";
		return 1;
	}

	//Init Logs
	init(sLevel);

	src::severity_logger_mt<logging::trivial::severity_level>&lg = my_logger::get();
	//src::severity_logger_mt<logging::trivial::severity_level>& lg = my_logger::get();
	BOOST_LOG_SEV(lg, logging::trivial::severity_level::info) << "Program Started!";
	BOOST_LOG_SEV(lg, logging::trivial::severity_level::info) << "Sol3 Host:" << szSol3Host;
	BOOST_LOG_SEV(lg, logging::trivial::severity_level::info) << "Sol3 Port:" << nSol3HostPort;
	BOOST_LOG_SEV(lg, logging::trivial::severity_level::info) << "MarketData Host:" << szMarketDataHost;
	BOOST_LOG_SEV(lg, logging::trivial::severity_level::info) << "MarketData Port:" << nDataHostPort;
	BOOST_LOG_SEV(lg, logging::trivial::severity_level::info) << "Log Level :" << sLevel;

	// init Queue and QueueManagers
	CMemoryQueueManager::Init();
	CMemoryQueueManager * mqh = CMemoryQueueManager::mqGetQueueHandle();

	CMemoryQueue<in_msg::CBaseInputMessage *> *foqueue = new CMemoryQueue<in_msg::CBaseInputMessage *>();
	mqh->nAppendQueue("WindDataQueue", (void *)foqueue);

	CMemoryQueue<out_msg::CBaseOutMessage *> *oqueue = new CMemoryQueue<out_msg::CBaseOutMessage *>();
	mqh->nAppendQueue("Sol3DataQueue", (void *)oqueue);


	// Init Message Generator
	sol3_message_generator::CMessageGenerator MsgGenerator;
	MsgGenerator.vSetInputQueue(foqueue);
	MsgGenerator.vSetOutputQueue(oqueue);
	MsgGenerator.Start();


	//Init Message Sender Queue
	sol3_message_sender::Sol3MessageSender MsgSender;
	MsgSender.SetMemoryQueue(oqueue);
	//MsgSender.SetServerInfo("116.228.204.38", 12343);
	MsgSender.SetServerInfo(szSol3Host, nSol3HostPort);
	MsgSender.Start();


	GetDataSpi * spi = new MySpi();
	GetDataApi * api = CreateGetDataApi(spi,true,3000);

	//int suc = api->connect("116.228.204.37", 18500);

	int suc = 0;
	
	suc = api->connect(szMarketDataHost, nDataHostPort);

	while (suc <= 0)
	{
		boost::this_thread::sleep_for(boost::chrono::duration<int, boost::milli>(1000));
		BOOST_LOG_SEV(lg, logging::trivial::severity_level::info) << "Connection to MarketData host Failed";
		suc = api->connect(szMarketDataHost, nDataHostPort);
	}

	GD_ErrorType err;

	if (suc)	// 连接成功
	{
		while (api->Login("LB", "LB", err) == false)
		{
			boost::this_thread::sleep_for(boost::chrono::duration<int, boost::milli>(1000));
			BOOST_LOG_SEV(lg, logging::trivial::severity_level::info) << "MarketData API login status :" << err;
		}
	}
	else
	{
		BOOST_LOG_SEV(lg, logging::trivial::severity_level::error) << "MarketData API login Failed! :" << err;
		return 1;
	}


	if (err == 0)
	{
		//请求实时行情
		boost::this_thread::sleep_for(boost::chrono::duration<int, boost::milli>(1000));

		if (nIsSimulation == 0)
			api->ReqRealtimeData(g_subOptionCode, false, err);
		else
		{
			char tmpszStart[32];
			char tmpszEnd[32];
			strcpy(tmpszStart, sDuration[0].c_str());
			strcpy(tmpszEnd, sDuration[1].c_str());

			api->ReqSimulationData(tmpszStart, tmpszEnd, GD_SimuModelType::normal_GD_SimuModelType, g_subOptionCode, false, err);
		}

		BOOST_LOG_SEV(lg, logging::trivial::severity_level::info) << "Number of Instruments Subscribed :" << g_subOptionCode.size();
	}
	else
	{
		BOOST_LOG_SEV(lg, logging::trivial::severity_level::error) << "MarketData API login failed :" << err;
		return 1;
	}


	while (true)
	{
		int x = 0;
		boost::this_thread::sleep_for(boost::chrono::duration<int, boost::milli>(1000));
	}

	boost::this_thread::sleep_for(boost::chrono::duration<int, boost::milli>(1000*10));

	boost::this_thread::sleep_for(boost::chrono::duration<int, boost::milli>(1000*10));
//	getchar();
	//should I do api->disconnect first before retry?
	BOOST_LOG_SEV(lg, logging::trivial::severity_level::error) << "program exit!";

	MsgGenerator.Stop();
	MsgSender.Stop();
	return 0;
}
