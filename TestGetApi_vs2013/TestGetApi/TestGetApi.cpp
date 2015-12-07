// TestRealtimApi.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
vector<char*> g_subOptionCode;

class MySpi : public GetDataSpi{

public:
	virtual void OnRecvDayBegin(GD_ISODateTimeType pDate){
		cout << "OnRecvDateBegin:" << pDate << endl;
	};			//���յ�ǰ������
	virtual void OnRecvDayEnd(GD_ISODateTimeType pDate){
		cout << "OnRecvDateEnd:" << pDate << endl;
	};			//���յ�������Ϣ
	
	virtual void OnRecvCode(TDF_CODE* pCode, int codeNum, TDF_OPTION_CODE* pOptionCode, int optionCodeNum){
		for (int i = 0; i < optionCodeNum; i++)
		{
			char* newCode = new char[32];
			strcpy(newCode, (pOptionCode + i)->basicCode.szCode);
			g_subOptionCode.push_back(newCode);
		}
	};		//�����

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


	//����
	int suc = api->connect("116.228.204.37", 18500);
	GD_ErrorType err;
	
	//��¼
	if (suc)	// ���ӳɹ�
	{
		api->Login("LB", "LB", err);
	}


	//����ʵʱ����
	api->ReqRealtimeData(g_subOptionCode, false, err);

	//�ǽ���ʱ���������ģ�����ݣ��ص�������ʵʱ������ͬ
	//api->ReqSimulationData("2015-12-3 9:30:00", "2015-12-3 19:40:00", g_subOptionCode, false, err);

	while (true)
	{
		int x = 0;
	}
	return 0;
}