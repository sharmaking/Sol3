#pragma once

#include "../GetDataAPI/include/GetDataStruct.h"
#include "../GetDataAPI/include/GetDataApi.h"

#include "MemoryQueue.h"
//#include "Sol3PhaseGenerator.h"
namespace sol3_phase_generator
{
	class Sol3PhaseGenerator;
}

namespace sol3_message_generator
{
	typedef char INSTR_ID[32];
	typedef enum
	{
		UNKNOWN,
		CCFX,
		CSSX,
		SGEX,
		XCFE,
		XDCE,
		XSGE,
		XINE,
		XSHE,
		XSEC,
		XSHG,
		XSSC,
		XZCE
	} EExchanges;
	extern EExchanges GetExchanges(char * szWindCode);
	namespace out_msg
	{
		typedef enum
		{
			Market = 1,
			Update,
			Prices,
			Last,
			Phase,
			FXRate,
			Closing,
			Unknown

		} OutMsgType;

		class CBaseOutMessage
		{
		public:
			CBaseOutMessage(OutMsgType t) :cMsgType(t){};
			INSTR_ID isin;
			INSTR_ID localid;
			char exchange[5];
			OutMsgType cMsgType;
		protected:
		};



		class CMarket :public CBaseOutMessage
		{
		public:
			CMarket(OutMsgType t);
			CMarket();
			char phase[12];
			unsigned int nAskPrice[5];			
			unsigned int nAskVol[5];			
			unsigned int nBidPrice[5];			
			unsigned int nBidVol[5];			
		};

		class CPrice :public CBaseOutMessage
		{
		public:
			CPrice();
			unsigned int open;
			unsigned int  close;
			unsigned int  high;
			unsigned int  low;
		};
		class CLast :public CBaseOutMessage
		{
		public:
			CLast();
			unsigned int  prc;
			unsigned int qty;
			unsigned int total_qty;
			unsigned int total_trades;
			char tick[13];
		};

		class CClosing :public CBaseOutMessage
		{
		public:
			CClosing();
			unsigned int  close_prc;
			char close_date[9];
		};

		class CPhase :public CBaseOutMessage
		{
		public:
			CPhase();
			char phase[5];
			INSTR_ID local_id;
			char exchange[5];
		};

	}

	namespace in_msg
	{
		typedef enum
		{
			FutureOption,
			Stock,
			StockMarket,
			StockTras,
			Market,
			Code,
			Phase,
			Raw
		} InMsgType;

		class CBaseInputMessage
		{
		public:
			CBaseInputMessage();
			CBaseInputMessage(in_msg::InMsgType);
			InMsgType cMsgType;
		};

		class CStockInputMessage :public CBaseInputMessage
		{
		public:
			CStockInputMessage();
			CStockInputMessage(TDF_MARKET_DATA &);
			CStockInputMessage(TDF_TRANSACTION &);
			TDF_MARKET_DATA m_sMarket;
			TDF_TRANSACTION m_sTrans;
		};

		class CFutureOptionInputMessage :public CBaseInputMessage
		{
		public:
			CFutureOptionInputMessage(TDF_FUTURE_DATA & FOPrice);
			TDF_FUTURE_DATA m_stFutureOptionPrice;
		};
	
		class COptionPhaseInputMessage :public CBaseInputMessage
		{
		public:
			COptionPhaseInputMessage();
			INSTR_ID local_id;
			char phase[12];
			char exchange[5];
		};
	}

	using std::tr1::shared_ptr;

	class TickPrice
	{
	public:
		TDF_FUTURE_DATA Pre;
		TDF_FUTURE_DATA Cur;
		void vUpdate(TDF_FUTURE_DATA &p);

	};
	class CMessageGenerator
	{
	public:
		CMessageGenerator();
		void vSetOutputQueue(sol3_memory_queue::CMemoryQueue<out_msg::CBaseOutMessage*> * q);
		void vSetInputQueue(sol3_memory_queue::CMemoryQueue<in_msg::CBaseInputMessage*> * q);
		void Start();
		void Stop();
		void Processing();
		out_msg::CPrice * GetCPrice(in_msg::CBaseInputMessage *);
		out_msg::CMarket * GetCMarket(in_msg::CBaseInputMessage *);
		out_msg::CLast * GetCLast(in_msg::CBaseInputMessage *, int );
		out_msg::CClosing * GetCClosing(in_msg::CBaseInputMessage *);
		bool bIsNewMessage(char *,out_msg::CPrice *);
		bool bIsNewMessage(char *,out_msg::CMarket *);
		bool bIsNewMessage(char *,out_msg::CLast *);
		bool bIsNewMessage(char *,out_msg::CClosing *);
	private:
		sol3_memory_queue::CMemoryQueue<in_msg::CBaseInputMessage *> *_m_pmqWindDataQueue;
		sol3_memory_queue::CMemoryQueue<out_msg::CBaseOutMessage*> *_m_pmqSol3DataQueue;
		shared_ptr<boost::thread> _m_thWorkingThread;
		boost::container::map<char *, TickPrice, sol3_memory_queue::ptrCmp> _m_mpSym2Tick;
		boost::container::map<char *, out_msg::CPrice, sol3_memory_queue::ptrCmp> _m_mpSym2Price;
		boost::container::map<char *, out_msg::CMarket, sol3_memory_queue::ptrCmp> _m_mpSym2Market;
		boost::container::map<char *, out_msg::CLast, sol3_memory_queue::ptrCmp> _m_mpSym2Last;
		boost::container::map<char *, char *, sol3_memory_queue::ptrCmp> _m_local2isin;
		sol3_phase_generator::Sol3PhaseGenerator *_m_cPhaseGener;
		std::vector<char*> & _m_vCodeList;

	};

}