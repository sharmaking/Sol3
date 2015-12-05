// TestRealtimApi.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

class MySpi : public GetDataSpi{

public:
	virtual void OnRecvDateBegin(GD_ISODateTimeType pDate){
		cout << "OnRecvDateBegin:" << pDate << endl;
	};			//接收当前交易日
	virtual void OnRecvDateEnd(GD_ISODateTimeType pDate){
		cout << "OnRecvDateEnd:" << pDate << endl;
	};			//接收到闭市消息

	virtual void OnRecvCode(TDF_CODE* pCode, int codeNum, TDF_OPTION_CODE* pOptionCode, int optionCodeNum){
		int x = 0;
	};		//代码表
	virtual void OnRecvMarket(TDF_MARKET_DATA* pMarket){

		//cout << pMarket->szCode << ", " << pMarket->nTradingDay << ", " << pMarket->nTime << ", " << pMarket->nMatch << endl;
	};				//行情

	virtual void OnRecvTransaction(TDF_TRANSACTION* pTransaction){

		//cout << pTransaction->szCode << ", " << pTransaction->nActionDay << ", " << pTransaction->nTime << ", " << pTransaction->nPrice << endl;
	};

	virtual void RecvFutureOption(TDF_FUTURE_DATA* pFuture, TDF_OPTION_CODE *codeInfo){
		//m_date.push_back(pFuture);
		//cout << pFuture->szCode << ", " << pFuture->nTradingDay << ", " << pFuture->nTime << ", " << codeInfo->szContractID << ", " << pFuture->nAskPrice[0] << ", " << pFuture->nBidPrice[0] << endl;
	};

	virtual void OnRecvRspKLine(TDBDefine_KLine *pKLine, CYCTYPE cyctype){
		cout << cyctype << "," << pKLine->chCode << ", " << pKLine->nDate << ", " << pKLine->nTime << ", " << pKLine->nClose << endl;
	};
	/*
	virtual void OnRecvRspTransaction(TDBDefine_Transaction* pTransaction){
	cout << pTransaction->chCode << ", " << pTransaction->nDate << ", " << pTransaction->nTime << ", " << pTransaction->nTradePrice << ", " << pTransaction->nTradeVolume << endl;
	}

	virtual void OnRecvRspStock(TDBDefine_Tick* pStock){
	cout << pStock->chCode << ", " << pStock->nTime << endl;
	}

	virtual void OnRecvRspStockAB(TDBDefine_TickAB* pStock){
	cout << pStock->chCode << ", " << pStock->nTime << endl;
	}
	*/

	virtual void OnRecvGDKLine(GD_Kline_Data* pKline){
		cout << pKline->szCode << ", " << pKline->szDatetime << ", " << pKline->nOpen << ", " << pKline->nType << endl;
	};

	virtual void OnRecvRspOver(){
		cout << "rspOver" << endl;
	};

	virtual void OnConnect(){
		cout << "OnConnect" << endl;
	};

	virtual void OnDisconnect(){
		cout << "OnDisconnect" << endl;
	}

	virtual void OnError(string error)
	{
		cout << error.c_str() << endl;
	}


	MySpi(){};
	~MySpi(){};
};


int _tmain(int argc, _TCHAR* argv[])
{
	GetDataSpi * spi = new MySpi();
	GetDataApi * api = new GetDataApi(spi);


	//连接
	int suc = api->connect("116.228.204.37", 18500);
	GD_ErrorType err;

	//登录
	if (suc)
	{
		api->Login("LB", "LB", err);
	}

	system("pause");

	//请求实时行情
	//Sleep(500);
	vector<char*> subCode;

	for (size_t i = 0; i < 9; i++)
	{
		char* _stock = new GD_CodeType;
		memset(_stock, 0, sizeof(GD_CodeType));
		sprintf(_stock, "60000%d", i);
		subCode.push_back(_stock);
	}

	api->ReqSimulationData("2015-12-03 9:30:00", "2015-12-04 9:40:00", subCode, false, err);
	//api->ReqGDKLine(minute_GD_CycType,"2015-11-30 9:30:00", "2015-11-30 9:40:00", subCode, false, err);


	//api->ReqRealtimeData(subCode, false, err);

	system("pause");

	// 更新订阅代码表
	for (size_t i = 10; i < 99; i++)
	{
		char* _stock = new GD_CodeType;
		memset(_stock, 0, sizeof(GD_CodeType));
		sprintf(_stock, "6000%d", i);
		subCode.push_back(_stock);
	}
	api->ReqUpdateSubStockCode(add_GD_UpdateSubCodeType, subCode, true, err);

	system("pause");


	//TDB 接口 
	// 	//TDBDefine_ReqKLine reqK = { "", nFlag, 0, 0, CYC_DAY, nCycDef, nAutoComplete, 20150604, 20150612, 0, 0 };
	// 	TDBDefine_ReqKLine reqK = { "000001.sz", REFILL_NONE, 0, 0, CYC_MINUTE, 1, 0, 20150101, 20150615, -1, -1 };
	// 	api->ReqKline(reqK, err);
	// 
	// 	system("pause");


	return 0;
}