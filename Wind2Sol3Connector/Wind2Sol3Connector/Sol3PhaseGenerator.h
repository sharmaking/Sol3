#pragma once
#include "../GetDataAPI/include/GetDataStruct.h"
#include "../GetDataAPI/include/GetDataApi.h"
#include "Sol3MessageGenerator.h"
struct TDF_FUTURE_DATA;
namespace sol3_phase_generator
{
	enum eMarketPhase
	{
		NONE,
		PREOPEN,
		AUCT,
		OPEN,
		TRAD,
		HOLD,
		TRAD2,
		CLOSE_AUCT,
		CLOSING
	};
	class Sol3PhaseGenerator
	{
	public:
		Sol3PhaseGenerator();
		eMarketPhase CurrentPhase(char *);
		bool bGetPhase(TDF_FUTURE_DATA & psTick, eMarketPhase * pePhase);
		void Reset();
		int  nGeneratePhaseMessage(std::vector<char*> & vCodeList, sol3_message_generator::out_msg::CBaseOutMessage **, int&);

	private:
		eMarketPhase _m_eCurrentPhase;
		eMarketPhase _TickPhase(TDF_FUTURE_DATA * psTick);
		int		_m_nLatestTickTime;
		int		_m_nStatus;
	};

}
