// TestRealtimApi.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

class MySpi : public GetDataSpi{

public:
	virtual void OnRecvCode(TDF_CODE* pCode, int codeNum, TDF_OPTION_CODE* pOptionCode, int optionCodeNum){
		
	};
	//TDB
	virtual void OnRecvRspKLine(TDBDefine_KLine* pKLine, CYCTYPE cyctype){
		cout << pKLine->chCode << " " << pKLine->nDate << endl;
	};
	virtual void OnRecvRspStock(TDBDefine_Tick* pStock){};
	virtual void OnRecvRspStockAB(TDBDefine_TickAB* pStock){};
	virtual void OnRecvRspFuture(TDBDefine_Future* pFuture){};
	virtual void OnRecvRspFutureAB(TDBDefine_FutureAB* pFuture){};
	

	virtual void OnRecvRspOver(){};


	MySpi(){};
	~MySpi(){};
};


int _tmain(int argc, _TCHAR* argv[])
{
	GetDataSpi * spi = new MySpi();
	GetDataApi * api = CreateGetDataApi(spi);


	//连接
	int suc = api->connect("116.228.204.37", 18500);
	GD_ErrorType err;

	//登录
	if (suc)
	{
		api->Login("HJ", "HJ", err);
	}


	//TDBDefine_ReqKLine reqK = { "", nFlag, 0, 0, CYC_DAY, nCycDef, nAutoComplete, 20150604, 20150612, 0, 0 };
	TDBDefine_ReqKLine reqK = { "000001.sz", REFILL_NONE, 0, 0, CYC_MINUTE, 1, 0, 20150101, 20150615, -1, -1 };
	api->ReqKline(reqK, err);
	
	system("pause");
	
	DelGetDataApi(api);
	delete spi;

	return 0;
}