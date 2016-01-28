#include "MyLog.h"
#include "Sol3PhaseGenerator.h"

class sol3_message_generator::out_msg::CBaseOutMessage;
namespace sol3_phase_generator
{
	bool Sol3PhaseGenerator::bGetPhase(TDF_FUTURE_DATA  &psTick, eMarketPhase * pePhase)
	{

		if (pePhase == NULL)
			return false;

		_m_nStatus = _m_nStatus < psTick.nStatus && psTick.nStatus != 81? psTick.nStatus : _m_nStatus;

		_m_nLatestTickTime = _m_nLatestTickTime < psTick.nTime ? psTick.nTime : _m_nLatestTickTime;

		if (_m_eCurrentPhase == eMarketPhase::NONE && _m_nLatestTickTime < 91500000 && _m_nStatus == 69)
		{
			//preopen
			_m_eCurrentPhase = eMarketPhase::PREOPEN;
			*pePhase = _m_eCurrentPhase;
			return true;
		}
		else if (_m_eCurrentPhase <= eMarketPhase::PREOPEN && ((_m_nLatestTickTime >= 91500000 && _m_nLatestTickTime < 92500000) || _m_nLatestTickTime == 0) && _m_nStatus == 73)
		{
			//AUCTION
			_m_eCurrentPhase = eMarketPhase::AUCT;
			*pePhase = _m_eCurrentPhase;
			return true;
		}
		else if (_m_eCurrentPhase <= eMarketPhase::PREOPEN && _m_nLatestTickTime >= 92500000 && _m_nLatestTickTime < 93000000 && _m_nStatus == 79)
		{
			//AUCTION
			_m_eCurrentPhase = eMarketPhase::OPEN;
			*pePhase = _m_eCurrentPhase;
			return true;
		}
		else if (_m_eCurrentPhase < eMarketPhase::TRAD &&   _m_nLatestTickTime >= 93000000 && _m_nLatestTickTime <= 113000000 && _m_nStatus == 79)
		{
			//TRADE
			_m_eCurrentPhase = eMarketPhase::TRAD;
			*pePhase = _m_eCurrentPhase;
			return true;
		}
		else if (_m_eCurrentPhase <= eMarketPhase::TRAD && _m_nLatestTickTime >= 112959500 && _m_nLatestTickTime <= 130000000 && _m_nStatus == 79)
		{
			//holding
			_m_eCurrentPhase = eMarketPhase::HOLD;
			*pePhase = _m_eCurrentPhase;
			return true;
		}
		else if (_m_eCurrentPhase <= eMarketPhase::HOLD && _m_nLatestTickTime >= 130000000 && _m_nLatestTickTime <= 145659000 && _m_nStatus == 79)
		{
			// trad2
			_m_eCurrentPhase = eMarketPhase::TRAD2;
			*pePhase = _m_eCurrentPhase;
			return true;
		}
		else if (_m_eCurrentPhase <= eMarketPhase::TRAD2 && _m_nLatestTickTime >= 145659000 && _m_nLatestTickTime <= 150000000 && _m_nStatus == 74)
		{
			//auction 2
			_m_eCurrentPhase = eMarketPhase::CLOSE_AUCT;
			*pePhase = _m_eCurrentPhase;
			return true;
		}
		else if (_m_eCurrentPhase <= eMarketPhase::CLOSE_AUCT && _m_nLatestTickTime >= 150000000 && _m_nLatestTickTime <= 160000000 && _m_nStatus == 86)
		{
			//closing
			_m_eCurrentPhase = eMarketPhase::CLOSING;
			*pePhase = _m_eCurrentPhase;
			return true;
		}
		else
		{
			//undefined
			return false;
		}
		return false;
	}

	int Sol3PhaseGenerator::nGeneratePhaseMessage(std::vector<char*> & vCodeList, sol3_message_generator::out_msg::CBaseOutMessage ** poMsg, int & nOSeq)
	{

		if (vCodeList.size() > 0)
		{
			if (nOSeq >= vCodeList.size())
				return -2;
			else
			{
				sol3_message_generator::out_msg::CPhase  * phs = new sol3_message_generator::out_msg::CPhase();
				strcpy(phs->exchange, "XSHG");
				strcpy(phs->localid, vCodeList[nOSeq]);
				phs->isin[0] = 0;
				/*
				enum eMarketPhase
				{
				NONE,
				AUCT,
				PREOPEN,
				TRAD,
				HOLD,
				TRAD2,
				CLOSE_AUCT,
				CLOSING
				};
				phase {instruments={{exchange="XSGE", local_id="ag1601"}}, phase="AUCT"}
				*/
				if (_m_eCurrentPhase == eMarketPhase::AUCT)
					strcpy(phs->phase, "AUCT");
				else if (_m_eCurrentPhase == eMarketPhase::PREOPEN)
					strcpy(phs->phase, "PREOP");
				else if (_m_eCurrentPhase == eMarketPhase::OPEN)
					strcpy(phs->phase, "PRETR");
				else if (_m_eCurrentPhase == eMarketPhase::TRAD || _m_eCurrentPhase == eMarketPhase::TRAD2)
					strcpy(phs->phase, "TRAD");
				else if (_m_eCurrentPhase == eMarketPhase::HOLD)
					strcpy(phs->phase, "HOLD");
				else if (_m_eCurrentPhase == eMarketPhase::CLOSE_AUCT)
					strcpy(phs->phase, "AUCT");
				else if (_m_eCurrentPhase == eMarketPhase::CLOSING)
					strcpy(phs->phase, "POSTT");
				else
					strcpy(phs->phase, "-");

				++nOSeq;
				//poMsg = static_cast<sol3_message_generator::out_msg::CBaseOutMessage *>(phs);
				*poMsg = (sol3_message_generator::out_msg::CBaseOutMessage *)(phs);
				return nOSeq;
			}
		}
		else
			return -1;
	}

	sol3_phase_generator::eMarketPhase Sol3PhaseGenerator::CurrentPhase(char * oszPhase)
	{
		if (oszPhase == NULL)
			return _m_eCurrentPhase;
		else
		{
			if (_m_eCurrentPhase == eMarketPhase::AUCT)
				strcpy(oszPhase,"AUCT");
			else if (_m_eCurrentPhase == eMarketPhase::PREOPEN)
				strcpy(oszPhase, "PREOP");
			else if (_m_eCurrentPhase == eMarketPhase::OPEN)
				strcpy(oszPhase, "PRETR");
			else if (_m_eCurrentPhase == eMarketPhase::TRAD || _m_eCurrentPhase == eMarketPhase::TRAD2)
				strcpy(oszPhase, "TRAD");
			else if (_m_eCurrentPhase == eMarketPhase::HOLD)
				strcpy(oszPhase, "HOLD");
			else if (_m_eCurrentPhase == eMarketPhase::CLOSE_AUCT)
				strcpy(oszPhase, "AUCT");
			else if (_m_eCurrentPhase == eMarketPhase::CLOSING)
				strcpy(oszPhase, "POSTT");
			else
				strcpy(oszPhase, "-");
		}
		return _m_eCurrentPhase;
	}

	Sol3PhaseGenerator::Sol3PhaseGenerator()
	{
		_m_nLatestTickTime = 0;
		_m_nStatus = 0;
		_m_eCurrentPhase = eMarketPhase::NONE;
	}

}
