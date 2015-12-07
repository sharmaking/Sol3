// TestRealtimApi.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
vector<char*> g_subOptionCode;

class MySpi : public GetDataSpi{

public:
	virtual void OnRecvDayBegin(GD_ISODateTimeType pDate){
		cout << "OnRecvDateBegin:" << pDate << endl;
	};			//接收当前交易日
	virtual void OnRecvDayEnd(GD_ISODateTimeType pDate){
		cout << "OnRecvDateEnd:" << pDate << endl;
	};			//接收到闭市消息
	
	virtual void OnRecvCode(TDF_CODE* pCode, int codeNum, TDF_OPTION_CODE* pOptionCode, int optionCodeNum){
		for (int i = 0; i < optionCodeNum; i++)
		{
			char* newCode = new char[32];
			strcpy(newCode, (pOptionCode + i)->basicCode.szCode);
			g_subOptionCode.push_back(newCode);
		}
	};		//代码表

	virtual void OnRecvFuture(TDF_FUTURE_DATA* pFuture){
		//m_date.push_back(pFuture);
		cout << pFuture->szCode << ", " << pFuture->nTradingDay << ", " << pFuture->nTime << ", " << pFuture->nClose << endl;
	};

	virtual void OnRecvFutureOption(TDF_FUTURE_DATA* pFuture){
		//m_date.push_back(pFuture);
		cout << pFuture->szCode << ", " << pFuture->nTradingDay << ", " << pFuture->nTime << ", " << pFuture->nClose << endl;
	};
	
	virtual void OnRecvRspOver(){
		cout << "rspOver" << endl;
	};
	

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
	if (suc)	// 连接成功
	{
		api->Login("LB", "LB", err);
	}


	//请求实时行情
	api->ReqRealtimeData(g_subOptionCode, false, err);

	//非交易时间可以请求模拟数据，回调函数与实时数据相同
	//api->ReqSimulationData("2015-12-3 9:30:00", "2015-12-3 19:40:00", g_subOptionCode, false, err);

	while (true)
	{
		int x = 0;
	}
	return 0;
}