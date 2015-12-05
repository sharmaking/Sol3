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
	virtual void OnRecvCode(TDF_CODE* pCode, int codeNum, TDF_OPTION_CODE* pOptionCode, int optionCodeNum){};		//代码表
	//实时数据
	virtual void OnRecvMarket(TDF_MARKET_DATA* pMarket){};									//行情
	virtual void OnRecvFuture(TDF_FUTURE_DATA* pFuture){};									//期货
	virtual void OnRecvFutureOption(TDF_FUTURE_DATA* pFuture){};							//期权
	virtual void OnRecvIndex(TDF_INDEX_DATA* pIndex){};										//指数
	virtual void OnRecvTransaction(TDF_TRANSACTION* pTransaction){};						//逐笔
	virtual void OnRecvOrderQueue(TDF_ORDER_QUEUE* pOrderQueue){};							//委托队列
	virtual void OnRecvOrder(TDF_ORDER* pOrder){};
	
	virtual void OnRecvDayBegin(GD_ISODateTimeType pDate){};			//接收当前交易日
	virtual void OnRecvDayEnd(GD_ISODateTimeType pDate){};				//接收到闭市消息

	//历史数据
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

	//系统信息
	virtual void OnConnect(){};
	virtual void OnDisconnect(){};

	virtual void OnError(string error){};	//错误信息
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
	//登录
	bool Login(char* user, char* pass, GD_ErrorType &err);
	//请求实时行情
	void ReqRealtimeData(vector<char*> &subCodes, bool isAllMarket, GD_ErrorType &err);
	void ReqUpdateSubStockCode(GD_UpdateSubCodeType reqType, vector<char*> &subCodes, bool isAllMarket, GD_ErrorType &err);		//订阅股票 vec为空代表订阅所有
	//请求模拟行情
	void ReqSimulationData(GD_ISODateTimeType beginTime, GD_ISODateTimeType endTime, vector<char*> &subCodes, bool isAllMarket, GD_ErrorType &err);	//请求模拟行情
	void ReqGDKLine(GD_CycType type, GD_ISODateTimeType beginTime, GD_ISODateTimeType endTime, vector<char*> &subCodes, bool isAllMarket, GD_ErrorType &err);	//请求模拟行情	
	//请求TDB历史数据
	void ReqKline(TDBDefine_ReqKLine &req, GD_ErrorType &err);					//请求历史K线
	void ReqStock(TDBDefine_ReqTick &req, bool isAB, GD_ErrorType &err);		//请求历史 股票、指数 tick数据, isAB != 0 包含买卖盘, isAB = 0 不包含
	void ReqFuture(TDBDefine_ReqFuture &req, bool isAB, GD_ErrorType &err);		//请求历史 期货、期权 tick数据, isAB != 0 包含买卖盘, isAB = 0 不包含
	void ReqTransaction(TDBDefine_ReqTransaction &req, GD_ErrorType &err);		//请求逐笔成交
	void ReqOrder(TDBDefine_ReqOrder &req, GD_ErrorType &err);					//请求逐笔委托（深市）
	void ReqOrderQueue(TDBDefine_ReqOrderQueue &req, GD_ErrorType &err);		//请求委托队列

	GetDataApi(GetDataSpi *spi);
	~GetDataApi();	
};

#endif // REALTIMEDATAAPI_H