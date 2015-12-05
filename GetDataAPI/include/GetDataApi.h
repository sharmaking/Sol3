#ifndef GETDATAAPI_H
#define GETDATAAPI_H

#ifndef GetData_API  
#define GetData_API _declspec(dllexport)  
#else  
#define GetData_API _declspec(dllimport)  
#endif

#define MaxIPLen 20

class SysConnection;
class DataConnection;
class GetDataSpi{
public:
	virtual void OnRecvCode(TDF_CODE* pCode, int codeNum, TDF_OPTION_CODE* pOptionCode, int optionCodeNum){};		//�����
	//ʵʱ����
	virtual void OnRecvMarket(TDF_MARKET_DATA* pMarket){};									//����
	virtual void OnRecvFuture(TDF_FUTURE_DATA* pFuture){};									//�ڻ�
	virtual void OnRecvFutureOption(TDF_FUTURE_DATA* pFuture){};							//��Ȩ
	virtual void OnRecvIndex(TDF_INDEX_DATA* pIndex){};										//ָ��
	virtual void OnRecvTransaction(TDF_TRANSACTION* pTransaction){};						//���
	virtual void OnRecvOrderQueue(TDF_ORDER_QUEUE* pOrderQueue){};							//ί�ж���
	virtual void OnRecvOrder(TDF_ORDER* pOrder){};
	
	virtual void OnRecvDayBegin(GD_ISODateTimeType pDate){};			//���յ�ǰ������
	virtual void OnRecvDayEnd(GD_ISODateTimeType pDate){};				//���յ�������Ϣ

	//��ʷ����
	//GD
	virtual void OnRecvGDKLine(GD_Kline_Data* pKline){};
	//TDB
	virtual void OnRecvRspKLine(TDBDefine_KLine* pKLine, CYCTYPE cyctype){};
	virtual void OnRecvRspStock(TDBDefine_Tick* pStock){};
	virtual void OnRecvRspStockAB(TDBDefine_TickAB* pStock){};
	virtual void OnRecvRspFuture(TDBDefine_Future* pFuture){};
	virtual void OnRecvRspFutureAB(TDBDefine_FutureAB* pFuture){};
	virtual void OnRecvRspTransaction(TDBDefine_Transaction* pTransaction){};
	virtual void OnRecvRspOrder(TDBDefine_Order* pOrder){};
	virtual void OnRecvRspOrderQueue(TDBDefine_OrderQueue* pOrderQueue){};
	virtual void OnRecvRspOver(){};

	//ϵͳ��Ϣ
	virtual void OnConnect(){};
	virtual void OnDisconnect(){};

	virtual void OnError(string error){};	//������Ϣ
	GetDataSpi(){};
	~GetDataSpi(){};
};

class GetData_API GetDataApi{ 
private:
	GetDataSpi* m_spi;
	SysConnection* m_config;
	DataConnection* m_realTime;
	DataConnection* m_simulation;
	DataConnection* m_history;
public:
	bool connect(string ip, int port);
	void reConnect();
	//��¼
	bool Login(char* user, char* pass, GD_ErrorType &err);
	//����ʵʱ����
	void ReqRealtimeData(vector<char*> &subCodes, bool isAllMarket, GD_ErrorType &err);
	void ReqUpdateSubStockCode(GD_UpdateSubCodeType reqType, vector<char*> &subCodes, bool isAllMarket, GD_ErrorType &err);		//���Ĺ�Ʊ vecΪ�մ���������
	//����ģ������
	void ReqSimulationData(GD_ISODateTimeType beginTime, GD_ISODateTimeType endTime, vector<char*> &subCodes, bool isAllMarket, GD_ErrorType &err);	//����ģ������
	void ReqGDKLine(GD_CycType type, GD_ISODateTimeType beginTime, GD_ISODateTimeType endTime, vector<char*> &subCodes, bool isAllMarket, GD_ErrorType &err);	//����ģ������	
	//����TDB��ʷ����
	void ReqKline(TDBDefine_ReqKLine &req, GD_ErrorType &err);					//������ʷK��
	void ReqStock(TDBDefine_ReqTick &req, bool isAB, GD_ErrorType &err);		//������ʷ ��Ʊ��ָ�� tick����, isAB != 0 ����������, isAB = 0 ������
	void ReqFuture(TDBDefine_ReqFuture &req, bool isAB, GD_ErrorType &err);		//������ʷ �ڻ�����Ȩ tick����, isAB != 0 ����������, isAB = 0 ������
	void ReqTransaction(TDBDefine_ReqTransaction &req, GD_ErrorType &err);		//������ʳɽ�
	void ReqOrder(TDBDefine_ReqOrder &req, GD_ErrorType &err);					//�������ί�У����У�
	void ReqOrderQueue(TDBDefine_ReqOrderQueue &req, GD_ErrorType &err);		//����ί�ж���

	GetDataApi(GetDataSpi *spi);
	~GetDataApi();	
};

#endif // REALTIMEDATAAPI_H