// Sol3MessageGenerator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "boost/algorithm/string.hpp"
#include "Sol3MessageGenerator.h"
#include "MyLog.h"
#include <string>
#include "Sol3PhaseGenerator.h"

using namespace std;
namespace logging = boost::log;
namespace src = boost::log::sources;
extern std::vector<char*> g_subOptionCode;

namespace sol3_message_generator
{
	EExchanges GetExchanges(char * szWindCode)
	{
		string s(szWindCode);
		if (boost::algorithm::ends_with(s, string(".SH")))
		{
			return EExchanges::XSHG;
		}

		return EExchanges::UNKNOWN;
	}
	
	void CMessageGenerator::vSetOutputQueue(sol3_memory_queue::CMemoryQueue<out_msg::CBaseOutMessage*> * q)
	{
		_m_pmqSol3DataQueue = q;
	}

	void CMessageGenerator::vSetInputQueue(sol3_memory_queue::CMemoryQueue<in_msg::CBaseInputMessage*> * q)
	{
		_m_pmqWindDataQueue = q;
	}

	void CMessageGenerator::Start()
	{
		_m_thWorkingThread.reset(new boost::thread(&CMessageGenerator::Processing, this));
		_m_thWorkingThread->detach();
	}

	void CMessageGenerator::Stop()
	{
		_m_thWorkingThread->interrupt();
	}

	/*
		Desc: Receive Data from G
	*/
	void CMessageGenerator::Processing()
	{
		src::severity_logger_mt<logging::trivial::severity_level>&lg = my_logger::get();
		BOOST_LOG_SEV(lg, logging::trivial::severity_level::info) << "Message Generator Thread Started!";
		while (true)
		{
			try
			{
				boost::this_thread::interruption_point();
				
				in_msg::CBaseInputMessage *msg = NULL;
				int nret = _m_pmqWindDataQueue->nPop(&msg);
				if (nret == 0)
				{
					if (msg->cMsgType == in_msg::FutureOption)
					{
						char tmpCode[32];
						tmpCode[0] = 0;
						strcpy(tmpCode, ((in_msg::CFutureOptionInputMessage*)msg)->m_stFutureOptionPrice.szCode);


						boost::container::map<char *, TickPrice, sol3_memory_queue::ptrCmp>::iterator it = _m_mpSym2Tick.begin();
						it = _m_mpSym2Tick.end();
						it = _m_mpSym2Tick.find(tmpCode);
						size_t ss = _m_mpSym2Tick.size();
						
						if (it == _m_mpSym2Tick.end())
						{
							TickPrice ttt;
							ttt.Cur = ((in_msg::CFutureOptionInputMessage*)msg)->m_stFutureOptionPrice;
							ttt.Pre.szCode[0] = 0;
							char * szKey = new char[32];
							szKey[0] = 0;
							strcpy(szKey, tmpCode);
							_m_mpSym2Tick.insert(pair<char*, TickPrice>(szKey, ttt));
						}
						else
						{
							it->second.Pre = it->second.Cur;
							it->second.Cur = ((in_msg::CFutureOptionInputMessage*)msg)->m_stFutureOptionPrice;
						}
						/*
						TDF_FUTURE_DATA p=_m_mpSym2Tick[((in_msg::CFutureOptionInputMessage*)msg)->m_stFutureOptionPrice.szCode].Pre;
						_m_mpSym2Tick[((in_msg::CFutureOptionInputMessage*)msg)->m_stFutureOptionPrice.szCode].Pre = _m_mpSym2Tick[((in_msg::CFutureOptionInputMessage*)msg)->m_stFutureOptionPrice.szCode].Cur;
						TDF_FUTURE_DATA c=_m_mpSym2Tick[((in_msg::CFutureOptionInputMessage*)msg)->m_stFutureOptionPrice.szCode].Cur;
						_m_mpSym2Tick[((in_msg::CFutureOptionInputMessage*)msg)->m_stFutureOptionPrice.szCode].Cur = ((in_msg::CFutureOptionInputMessage*)msg)->m_stFutureOptionPrice;
						TDF_FUTURE_DATA t = ((in_msg::CFutureOptionInputMessage*)msg)->m_stFutureOptionPrice;
						*/

						//skip first price
						long long  nVolDelta = 0;
						if (((in_msg::CFutureOptionInputMessage*)msg)->m_stFutureOptionPrice.nTime == 0)
						{
							nVolDelta = 0;
						}
						else
						{
							it = _m_mpSym2Tick.find(tmpCode);
							if (it == _m_mpSym2Tick.end())
								nVolDelta = 0;
							else if (it->second.Pre.szCode[0] == 0)
								nVolDelta = 0;
							else
								nVolDelta = it->second.Cur.iVolume - it->second.Pre.iVolume;

						}
						if (nVolDelta < 0)
						{
							TDF_FUTURE_DATA p = it->second.Pre;
							TDF_FUTURE_DATA c = it->second.Cur;
							BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "nVolDelta < 0";
						}
						sol3_phase_generator::eMarketPhase oePhase;

						if (_m_cPhaseGener->bGetPhase(((in_msg::CFutureOptionInputMessage*)msg)->m_stFutureOptionPrice, &oePhase))
						{
							out_msg::CBaseOutMessage * oMsg ;
							int n = 0;
							while (_m_cPhaseGener->nGeneratePhaseMessage(_m_vCodeList, &oMsg, n) > 0)
							{
								_m_pmqSol3DataQueue->nPush(oMsg);
							}
						}

						out_msg::CMarket * mkt = GetCMarket(msg);
						if (mkt != NULL && bIsNewMessage(tmpCode,mkt))
						{
							_m_pmqSol3DataQueue->nPush(mkt);
						}
						else if (mkt != NULL)
						{

							BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "Message drop MKT DUP:" << mkt->localid;
							delete mkt;
						}

						out_msg::CLast * lst = GetCLast(msg, nVolDelta);
						if (lst != NULL && bIsNewMessage(tmpCode, lst))
						{
							_m_pmqSol3DataQueue->nPush(lst);
						}
						else if (lst != NULL)
						{
							BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "Message drop lst DUP:" << mkt->localid;
							delete lst;
						}
		

						if ((((in_msg::CFutureOptionInputMessage*)msg)->m_stFutureOptionPrice.nTime <= 92600000 &&
							((in_msg::CFutureOptionInputMessage*)msg)->m_stFutureOptionPrice.nTime >= 91500000) || ((in_msg::CFutureOptionInputMessage*)msg)->m_stFutureOptionPrice.nTime == 0)
						{
							delete msg;
							continue;
						}
						out_msg::CPrice *prc = GetCPrice(msg);
						if (prc != NULL && bIsNewMessage(tmpCode,prc))
						{
							_m_pmqSol3DataQueue->nPush(prc);
						}
						else if (prc != NULL)
						{
							delete prc;
						}
						if (((in_msg::CFutureOptionInputMessage*)msg)->m_stFutureOptionPrice.nStatus == 86)
						{

						out_msg::CClosing *cls = GetCClosing(msg);
						if (cls != NULL && bIsNewMessage(tmpCode,cls))
						{
							_m_pmqSol3DataQueue->nPush(cls);
						}
						else if (cls != NULL)
						{
							delete cls;
						}
						}
						if (msg != NULL)
						{
							delete msg;
						}

					}
					else if (msg->cMsgType == in_msg::StockMarket)
					{
						char tmpCode[32];
						tmpCode[0] = 0;
						strcpy(tmpCode, ((in_msg::CStockInputMessage*)msg)->m_sMarket.szCode);
						out_msg::CMarket * mkt = GetCMarket(msg);
						if (mkt != NULL && bIsNewMessage(tmpCode, mkt))
						{
							_m_pmqSol3DataQueue->nPush(mkt);
						}
						else if (mkt != NULL)
						{

							BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "Message drop MKT DUP:" << mkt->localid;
							delete mkt;
						}
						if (msg != NULL)
						{
							delete msg;
						}
					}
					else if (msg->cMsgType == in_msg::StockTras)
					{
						char tmpCode[32];
						tmpCode[0] = 0;
						strcpy(tmpCode, ((in_msg::CStockInputMessage*)msg)->m_sTrans.szCode);
						out_msg::CLast * lst = GetCLast(msg, 0);
						if (lst != NULL )
						{
							_m_pmqSol3DataQueue->nPush(lst);
						}
						else if (lst != NULL)
						{
							BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "Message drop lst DUP:" << lst->localid;
							delete lst;
						}
						if (msg != NULL)
						{
							delete msg;
						}
					}
					else
					{
						BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "Unknown Message Received";
					}
				}
			}
			catch (boost::thread_interrupted& /*e*/)
			{
			}
		}
		BOOST_LOG_SEV(lg, logging::trivial::severity_level::debug) << "Message Generator Thread Exit!";
	}

	out_msg::CPrice * CMessageGenerator::GetCPrice(in_msg::CBaseInputMessage  *m)
	{
		out_msg::CPrice *prc = new out_msg::CPrice();

		if (m->cMsgType == in_msg::InMsgType::FutureOption)
		{
			in_msg::CFutureOptionInputMessage * msg = static_cast<in_msg::CFutureOptionInputMessage*>(m);
			if (msg == NULL)
				return NULL;

			prc->cMsgType = out_msg::Prices;
			prc->close = msg->m_stFutureOptionPrice.nClose;
			prc->open = msg->m_stFutureOptionPrice.nOpen;
			prc->high = msg->m_stFutureOptionPrice.nHigh;
			prc->low = msg->m_stFutureOptionPrice.nLow;
			EExchanges exch = GetExchanges(msg->m_stFutureOptionPrice.szWindCode);
			if (exch == EExchanges::XSHG)
			{
				//ShangHai Stock Exchange
				strcpy(prc->exchange, "XSHG");
			}
			prc->isin[0] = 0;
			strcpy(prc->localid, msg->m_stFutureOptionPrice.szCode);
			//strcpy(prc->isin, prc->localid);
			prc->isin[0] = 0;
		}
		else if (m->cMsgType == in_msg::InMsgType::StockMarket)
		{
			in_msg::CStockInputMessage * msg = static_cast<in_msg::CStockInputMessage*>(m);
			if (msg == NULL)
				return NULL;

			prc->cMsgType = out_msg::Prices;
			prc->close = msg->m_sMarket.nMatch;
			prc->open = msg->m_sMarket.nOpen;
			prc->high = msg->m_sMarket.nHigh;
			prc->low = msg->m_sMarket.nLow;
			EExchanges exch = GetExchanges(msg->m_sMarket.szWindCode);
			if (exch == EExchanges::XSHG)
			{
				//ShangHai Stock Exchange
				strcpy(prc->exchange, "XSHG");
			}
			prc->isin[0] = 0;
			/*
			strcpy(prc->localid, msg->m_sMarket.szCode);
			boost::container::map<char *, char *, sol3_memory_queue::ptrCmp>::iterator it = _m_local2isin.begin();
			it = _m_local2isin.find(prc->localid);
			if (it != _m_local2isin.end())
			{
				strcpy(prc->isin, it->second);
			}
			*/
		}

		return prc;
	}

	out_msg::CMarket * CMessageGenerator::GetCMarket(in_msg::CBaseInputMessage *m)
	{
		out_msg::CMarket *mkt =NULL;
		if (m->cMsgType == in_msg::InMsgType::FutureOption)
		{
			in_msg::CFutureOptionInputMessage * msg = static_cast<in_msg::CFutureOptionInputMessage*>(m);

			if (msg == NULL)
				return NULL;

			mkt = new out_msg::CMarket();

			if (mkt == NULL)
				return NULL;

			mkt->cMsgType = out_msg::Market;
			EExchanges exch = GetExchanges(msg->m_stFutureOptionPrice.szWindCode);

			if (exch == EExchanges::XSHG)
			{
				//ShangHai Stock Exchange
				strcpy(mkt->exchange, "XSHG");
			}

			mkt->isin[0] = 0;
			strcpy(mkt->localid, msg->m_stFutureOptionPrice.szCode);


			for (int i = 0; i < 5; i++)
			{
				mkt->nAskPrice[i] = msg->m_stFutureOptionPrice.nAskPrice[i];
				mkt->nAskVol[i] = msg->m_stFutureOptionPrice.nAskVol[i];
				mkt->nBidPrice[i] = msg->m_stFutureOptionPrice.nBidPrice[i];
				mkt->nBidVol[i] = msg->m_stFutureOptionPrice.nBidVol[i];
			}
			_m_cPhaseGener->CurrentPhase(mkt->phase);
		}
		else if (m->cMsgType == in_msg::InMsgType::StockMarket)
		{
			in_msg::CStockInputMessage * msg = static_cast<in_msg::CStockInputMessage*>(m);
			if (msg == NULL)
				return NULL;

			mkt = new out_msg::CMarket();
			if (mkt == NULL)
				return NULL;

			mkt->cMsgType = out_msg::Market;
			EExchanges exch = GetExchanges(msg->m_sMarket.szWindCode);

			if (exch == EExchanges::XSHG)
			{
				//ShangHai Stock Exchange
				strcpy(mkt->exchange, "XSHG");
			}

			mkt->isin[0] = 0;
			strcpy(mkt->localid, msg->m_sMarket.szCode);
			/*
			boost::container::map<char *, char *, sol3_memory_queue::ptrCmp>::iterator it = _m_local2isin.begin();
			it = _m_local2isin.find(mkt->localid);
			if (it != _m_local2isin.end())
			{
				strcpy(mkt->isin, it->second);
			}
			*/

			for (int i = 0; i < 5; i++)
			{
				mkt->nAskPrice[i] = msg->m_sMarket.nAskPrice[i];
				mkt->nAskVol[i] = msg->m_sMarket.nAskVol[i];
				mkt->nBidPrice[i] = msg->m_sMarket.nBidPrice[i];
				mkt->nBidVol[i] = msg->m_sMarket.nBidVol[i];
			}
			_m_cPhaseGener->CurrentPhase(mkt->phase);
		}

		return mkt;
	}

	out_msg::CLast * CMessageGenerator::GetCLast(in_msg::CBaseInputMessage *m, int nVolDta)
	{
		out_msg::CLast *lst = NULL;
		if (m->cMsgType == in_msg::InMsgType::FutureOption)
		{
			in_msg::CFutureOptionInputMessage * msg = static_cast<in_msg::CFutureOptionInputMessage*>(m);
			if (msg == NULL)
				return NULL;

			lst = new out_msg::CLast();
			if (lst == NULL)
				return NULL;

			lst->cMsgType = out_msg::Last;
			EExchanges exch = GetExchanges(msg->m_stFutureOptionPrice.szWindCode);

			if (exch == EExchanges::XSHG)
			{
				//ShangHai Stock Exchange
				strcpy(lst->exchange, "XSHG");
			}

			lst->isin[0] = 0;
			strcpy(lst->localid, msg->m_stFutureOptionPrice.szCode);

			lst->prc = msg->m_stFutureOptionPrice.nMatch;
			sprintf(lst->tick, "%02d:%02d:%02d.%03d", msg->m_stFutureOptionPrice.nTime / 10000000, (msg->m_stFutureOptionPrice.nTime % 10000000) / 100000,
				(msg->m_stFutureOptionPrice.nTime % 100000) / 1000, msg->m_stFutureOptionPrice.nTime % 1000);
			//		lst->qty = msg->m_stFutureOptionPrice
			lst->qty = nVolDta;
			lst->total_qty = msg->m_stFutureOptionPrice.iVolume;
			lst->total_trades = msg->m_stFutureOptionPrice.iTurnover;
		}
		else if (m->cMsgType == in_msg::InMsgType::StockTras)
		{
			in_msg::CStockInputMessage * msg = static_cast<in_msg::CStockInputMessage*>(m);
			if (msg == NULL)
				return NULL;

			lst = new out_msg::CLast();
			if (lst == NULL)
				return NULL;

			lst->cMsgType = out_msg::Last;
			EExchanges exch = GetExchanges(msg->m_sTrans.szWindCode);

			if (exch == EExchanges::XSHG)
			{
				//ShangHai Stock Exchange
				strcpy(lst->exchange, "XSHG");
			}

			strcpy(lst->localid, msg->m_sTrans.szCode);

			lst->isin[0] = 0;
			/*
			boost::container::map<char *, char *, sol3_memory_queue::ptrCmp>::iterator it = _m_local2isin.begin();
			it = _m_local2isin.find(lst->localid);
			if (it != _m_local2isin.end())
			{
				strcpy(lst->isin, it->second);
			}
			*/

			lst->prc = msg->m_sTrans.nPrice;
			sprintf(lst->tick, "%02d:%02d:%02d.%03d", msg->m_sTrans.nTime / 10000000, (msg->m_sTrans.nTime % 10000000) / 100000,
				(msg->m_sTrans.nTime % 100000) / 1000, msg->m_sTrans.nTime % 1000);
			//		lst->qty = msg->m_stFutureOptionPrice
			lst->qty = msg->m_sTrans.nVolume;
			lst->total_qty = 0;
			lst->total_trades = 0;
		}

		return lst;
	}

	out_msg::CClosing * CMessageGenerator::GetCClosing(in_msg::CBaseInputMessage *m)
	{
		out_msg::CClosing * cls = NULL;
		if (m->cMsgType == in_msg::InMsgType::FutureOption)
		{
			in_msg::CFutureOptionInputMessage * msg = static_cast<in_msg::CFutureOptionInputMessage*>(m);
			if (msg == NULL)
				return NULL;


			cls = new out_msg::CClosing();
			if (cls == NULL)
				return NULL;
			//closing_price {exchange="XSGE", local_id="ag1601", close_prc=0.015, close_date="20151217" }
			cls->cMsgType = out_msg::Closing;
			EExchanges exch = GetExchanges(msg->m_stFutureOptionPrice.szWindCode);
			if (exch == EExchanges::XSHG)
			{
				//ShangHai Stock Exchange
				strcpy(cls->exchange, "XSHG");
			}
			cls->isin[0] = 0;
			strcpy(cls->localid, msg->m_stFutureOptionPrice.szCode);
			cls->close_prc = msg->m_stFutureOptionPrice.nClose;
			sprintf(cls->close_date, "%d", msg->m_stFutureOptionPrice.nTradingDay);
		}
		else if (m->cMsgType == in_msg::InMsgType::StockMarket)
		{
			in_msg::CFutureOptionInputMessage * msg = static_cast<in_msg::CFutureOptionInputMessage*>(m);
			if (msg == NULL)
				return NULL;

			cls = new out_msg::CClosing();
			if (cls == NULL)
				return NULL;
			//closing_price {exchange="XSGE", local_id="ag1601", close_prc=0.015, close_date="20151217" }
			cls->cMsgType = out_msg::Closing;
			EExchanges exch = GetExchanges(msg->m_stFutureOptionPrice.szWindCode);
			if (exch == EExchanges::XSHG)
			{
				//ShangHai Stock Exchange
				strcpy(cls->exchange, "XSHG");
			}
			strcpy(cls->localid, msg->m_stFutureOptionPrice.szCode);
			cls->isin[0] = 0;
			/*
			boost::container::map<char *, char *, sol3_memory_queue::ptrCmp>::iterator it = _m_local2isin.begin();
			it = _m_local2isin.find(cls->localid);
			if (it != _m_local2isin.end())
			{
				strcpy(cls->isin, it->second);
			}
			*/	
			cls->close_prc = msg->m_stFutureOptionPrice.nClose;
			sprintf(cls->close_date, "%d", msg->m_stFutureOptionPrice.nTradingDay);
		}
		return cls;
	}

	bool CMessageGenerator::bIsNewMessage(char *szCode,out_msg::CPrice * pPirce)
	{
		if (pPirce == NULL)
			return false;

		boost::container::map<char *, out_msg::CPrice, sol3_memory_queue::ptrCmp>::iterator it = _m_mpSym2Price.end();
		it = _m_mpSym2Price.find(szCode);
		if (it == _m_mpSym2Price.end())
		{
			char *t = new char[13];
			strcpy(t, szCode);
			_m_mpSym2Price.insert(pair<char *, out_msg::CPrice>(t, *pPirce));
			return true;
		}
		else
		{
			if (it->second.close == pPirce->close &&
				it->second.open == pPirce->open &&
				it->second.high == pPirce->high&&
				it->second.low == pPirce->low)
			{
				return false;
			}
			else
			{
				_m_mpSym2Price[szCode] = *pPirce;
				return true;
			}
		}

		return true;
	}

	bool CMessageGenerator::bIsNewMessage(char * szCode, out_msg::CMarket *pMarket)
	{
		if (pMarket == NULL)
			return false;

		boost::container::map<char *, out_msg::CMarket, sol3_memory_queue::ptrCmp>::iterator it = _m_mpSym2Market.end();
		it = _m_mpSym2Market.find(szCode);
		if (it ==_m_mpSym2Market.end())
		{
			char *t = new char[13];
			strcpy(t, szCode);
			_m_mpSym2Market.insert(pair<char *, out_msg::CMarket>(t, *pMarket));
			return true;
		}
		else
		{
			bool bIdentical = true;
			for (int i = 0; i < 5; i++)
			{
				if (it->second.nAskPrice[i] == pMarket->nAskPrice[i] &&
					it->second.nAskVol[i] == pMarket->nAskVol[i] &&
					it->second.nBidPrice[i] == pMarket->nBidPrice[i] &&
					it->second.nBidVol[i] == pMarket->nBidVol[i] &&
					strcmp(it->second.phase, pMarket->phase) == 0)
				{
					continue;
				//	return false;
				}
				else
				{
					bIdentical = false;
					break;
				}
			}

			if (bIdentical == true)
				return false;

			_m_mpSym2Market[szCode] = *pMarket;
			return true;

		}

		return true;
	}

	bool CMessageGenerator::bIsNewMessage(char * szCode, out_msg::CLast *pLast)
	{

		if (pLast == NULL)
			return false;

		if (pLast->qty == 0)
			return false;
		else
			return true;
	}

	bool CMessageGenerator::bIsNewMessage(char * szCode, out_msg::CClosing *)
	{
		return true;
	}

	CMessageGenerator::CMessageGenerator() :_m_vCodeList(g_subOptionCode)
	{
		_m_cPhaseGener = new sol3_phase_generator::Sol3PhaseGenerator;
		char * k;
		char * v;
		k = new char[32];
		v = new char[32];
		k[0] = 0;
		v[0] = 0;
		strcpy(k, "510050");
		strcpy(v, "CNE000001LM6");
		_m_local2isin.insert(pair<char *, char*>(k,v));
	}


	in_msg::CFutureOptionInputMessage::CFutureOptionInputMessage(TDF_FUTURE_DATA & FOPrice)
	{
		m_stFutureOptionPrice = FOPrice;
		cMsgType = in_msg::FutureOption;
	}


	out_msg::CPrice::CPrice() :out_msg::CBaseOutMessage(out_msg::Prices)
	{

	}


	out_msg::CMarket::CMarket(OutMsgType t) :CBaseOutMessage(t)
	{

	}

	out_msg::CMarket::CMarket() : CBaseOutMessage(out_msg::Market)
	{

	}


	out_msg::CLast::CLast() :CBaseOutMessage(out_msg::Last)
	{

	}


	out_msg::CClosing::CClosing() :CBaseOutMessage(out_msg::Closing)
	{

	}


	in_msg::COptionPhaseInputMessage::COptionPhaseInputMessage() :CBaseInputMessage(in_msg::Phase)
	{

	}


	in_msg::CBaseInputMessage::CBaseInputMessage()
	{

	}

	in_msg::CBaseInputMessage::CBaseInputMessage(in_msg::InMsgType m) :cMsgType(m)
	{

	}


	out_msg::CPhase::CPhase() :CBaseOutMessage(out_msg::Phase)
	{
	}


	in_msg::CStockInputMessage::CStockInputMessage()
	{

	}

	in_msg::CStockInputMessage::CStockInputMessage(TDF_MARKET_DATA & sMarket) :CBaseInputMessage(InMsgType::StockMarket)
	{
		m_sMarket = sMarket;
	}

	in_msg::CStockInputMessage::CStockInputMessage(TDF_TRANSACTION & sTrans) : CBaseInputMessage(InMsgType::StockTras)
	{
		m_sTrans = sTrans;
	}

}

/*
int _tmain(int argc, _TCHAR* argv[])
{
	sol3_message_generator::CMessageGenerator cMsgGenerator;

	return 0;
}
*/
