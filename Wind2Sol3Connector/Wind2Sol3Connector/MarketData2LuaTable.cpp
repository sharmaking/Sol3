#include "MarketData2LuaTable.h"
#define DeScale(x) (x/10000.0)
int MarketData2Sol3::nGetSol3Message(out_msg::CBaseOutMessage *CMsg, char * szOutBuffer, size_t stLength)
{

	if (szOutBuffer == NULL)
		return -1;
	if (CMsg == NULL)
	{
		szOutBuffer[0] = 0;
		return 1;
	}

	char tmp_buffer[1024];

	if (CMsg->cMsgType == out_msg::Market)
	{
#pragma region Market Message
		out_msg::CMarket * m = static_cast<out_msg::CMarket *>(CMsg);
		if (m == NULL)
		{
			return 2;
		}
		//char tmp_buffer[1024];
		char tmp_bufBid[1024];
		char tmp_bufAsk[1024];

		sprintf(tmp_bufBid, "bids={{prc=%.4f,qty=%d},{prc=%.4f,qty=%d},{prc=%.4f,qty=%d}}",
			DeScale(m->nBidPrice[0]), m->nBidVol[0],
			DeScale(m->nBidPrice[1]), m->nBidVol[1],
			DeScale(m->nBidPrice[2]), m->nBidVol[2]);

		sprintf(tmp_bufAsk, "asks={{prc=%.4f,qty=%d},{prc=%.4f,qty=%d},{prc=%.4f,qty=%d}}",
			DeScale(m->nAskPrice[0]), m->nAskVol[0],
			DeScale(m->nAskPrice[1]), m->nAskVol[1],
			DeScale(m->nAskPrice[2]), m->nAskVol[2]);

		if (m->isin[0] == 0)
		{
			sprintf(tmp_buffer, "market { local_id=\"%s\", exchange=\"%s\",  phase=\"%s\", %s,%s }\n",
				m->localid,
				m->exchange,
				m->phase,
				tmp_bufBid,
				tmp_bufAsk
				);
		}
		else
		{
			sprintf(tmp_buffer, "market { isin=\"%s\", exchange=\"%s\",  phase=\"%s\", %s,%s }\n",
				m->isin,
				m->exchange,
				m->phase,
				tmp_bufBid,
				tmp_bufAsk
				);

		}

#pragma endregion Market Message
	}
	else if (CMsg->cMsgType == out_msg::Last)
	{
		out_msg::CLast * L = static_cast<out_msg::CLast *>(CMsg);
		if (L == NULL)
		{
			return 2;
		}

		/*
		last
		{
			isin = "AU000000PDN8", exchange = "XTSX",
			prc = 25.5,
			qty = 50,
			total_qty = 1500,
			total_trades = 20,
			tick = "16:21:00.00"
		}
		*/

		if (L->isin[0] == 0)
		{
			if (L->total_qty == 0 && L->total_trades == 0)
			{
				sprintf(tmp_buffer, "last {local_id=\"%s\",exchange=\"%s\",prc=%.4f,qty=%d,tick=\"%s\"}\n",
					L->localid, L->exchange, DeScale(L->prc), L->qty,  L->tick);
			}
			else
			{
				sprintf(tmp_buffer, "last {local_id=\"%s\",exchange=\"%s\",prc=%.4f,qty=%d,total_qty=%d,total_trades=%d,tick=\"%s\"}\n",
					L->localid, L->exchange, DeScale(L->prc), L->qty, L->total_qty, L->total_trades, L->tick);
			}
		}
		else
		{
			sprintf(tmp_buffer, "last {isin=\"%s\",exchange=\"%s\",prc=%.4f,qty=%d,total_qty=%d,total_trades=%d,tick=\"%s\"}\n",
				L->isin, L->exchange, DeScale(L->prc), L->qty, L->total_qty, L->total_trades, L->tick);
		}
	}
	else if (CMsg->cMsgType == out_msg::Prices)
	{
		/*
		prices
		{
		isin = "AU000000PDN8", exchange = "XTSX",
		open = 15.00,
		close = 14.9,
		high = 15.5,
		low = 14.0,
		}
		*/
		out_msg::CPrice * P = static_cast<out_msg::CPrice *>(CMsg);
		if (P == NULL)
		{
			return 2;
		}
		if (P->isin[0] == 0)
		{
			sprintf(tmp_buffer, "prices {local_id=\"%s\",exchange=\"%s\",open=%.4f,high=%.4f,low=%.4f}\n",
				P->localid, P->exchange, DeScale(P->open), DeScale(P->high), DeScale(P->low));
		}
		else
		{
			sprintf(tmp_buffer, "prices {isin=\"%s\",exchange=\"%s\",open=%.4f,high=%.4f,low=%.4f}\n",
				P->isin, P->exchange, DeScale(P->open), DeScale(P->high), DeScale(P->low));
		}
	}
	else if (CMsg->cMsgType == out_msg::Closing)
	{
		/*
		closing_price
		{
		isin = "AU000000PDN8", exchange = "XTSX",
		close_prc = 14.99,
		close_date = 20081215
		}
		*/
		out_msg::CClosing * C = static_cast<out_msg::CClosing *>(CMsg);
		if (C == NULL)
		{
			return 2;
		}
		/*
		sprintf(tmp_buffer, "closing_price {local_id=\"%s\",exchange=\"%s\",close_prc=%.4f,close_date=%s}\n",
			C->localid, C->exchange, DeScale(C->close_prc), C->close_date );
			*/
		if (C->isin[0] == 0)
		{
			sprintf(tmp_buffer, "prices {local_id=\"%s\",exchange=\"%s\",close=%.4f}\n",
				C->localid, C->exchange, DeScale(C->close_prc));
		}
		else
		{
			sprintf(tmp_buffer, "prices {isin=\"%s\",exchange=\"%s\",close=%.4f}\n",
				C->isin, C->exchange, DeScale(C->close_prc));

		}
	}
	else if (CMsg->cMsgType == out_msg::FXRate)
	{
	}
	else if (CMsg->cMsgType == out_msg::Phase)
	{
		//to do

		/*
		phase
		{
		isin = "US68554W2052",
		exchange = "XLON",
		phase = "TRAD"
		}
		*/
		out_msg::CPhase * C = static_cast<out_msg::CPhase *>(CMsg);
		if (C == NULL)
		{
			return 2;
		}
		/*phase {instruments={{exchange="XSGE", local_id="ag1601"}}, phase="AUCT"}*/
		sprintf(tmp_buffer, "phase {instruments={{local_id=\"%s\",exchange=\"%s\"}},phase=\"%s\"}\n",
			C->localid, C->exchange, C->phase);
	}

	if (strlen(tmp_buffer) <= stLength - 1)
	{
		strcpy(szOutBuffer, tmp_buffer);
	}
	else
	{
		szOutBuffer[0] = 0;
		return 3;
	}
	return 0;
}

