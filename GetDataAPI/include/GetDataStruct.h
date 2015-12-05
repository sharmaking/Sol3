#ifndef GETDATASTRUCT_H
#define GETDATASTRUCT_H

//////////////////////////////////////////////////////////////////////////
//TDF 结构体
//////////////////////////////////////////////////////////////////////////
#ifndef __TDF_API_STRUCT_H__
#define __TDF_API_STRUCT_H__
struct TDF_MARKET_CLOSE
{
	char    szMarket[8];        //交易所名称
	int		nTime;				//时间(HHMMSSmmm)
	char	chInfo[64];			//闭市信息
};

struct TDF_CODE
{
	char szWindCode[32];		//Wind Code: AG1302.SHF
	char szMarket[8];			//market code: SHF
	char szCode[32];			//original code:ag1302
	char szENName[32];
	char szCNName[32];			//chinese name: 沪银1302
	int nType;
};

struct TDF_OPTION_CODE
{
	TDF_CODE basicCode;

	char szContractID[32];			// 期权合约代码
	char szUnderlyingSecurityID[32];//// 标的证券代码
	char chCallOrPut;               // 认购认沽C1        认购，则本字段为“C”；若为认沽，则本字段为“P”
	int  nExerciseDate;             // 期权行权日，YYYYMMDD

	//扩充字段
	char chUnderlyingType;			// 标的证券类型C3    0-A股 1-ETF (EBS – ETF， ASH – A 股)
	char chOptionType;              // 欧式美式C1        若为欧式期权，则本字段为“E”；若为美式期权，则本字段为“A”

	char chPriceLimitType;          // 涨跌幅限制类型C1 ‘N’表示有涨跌幅限制类型, ‘R’表示无涨跌幅限制类型
	int  nContractMultiplierUnit;	// 合约单位,         经过除权除息调整后的合约单位, 一定是整数
	int  nExercisePrice;            // 期权行权价,       经过除权除息调整后的期权行权价，右对齐，精确到厘
	int  nStartDate;                // 期权首个交易日,YYYYMMDD
	int  nEndDate;                  // 期权最后交易日/行权日，YYYYMMDD
	int  nExpireDate;               // 期权到期日，YYYYMMDD
};

//数据消息：MSG_DATA_MARKET 对应的结构
struct TDF_MARKET_DATA
{
	char        szWindCode[32];         //600001.SH 
	char        szCode[32];             //原始Code
	int         nActionDay;             //业务发生日(自然日)
	int         nTradingDay;            //交易日
	int			 nTime;					//时间(HHMMSSmmm)
	int			 nStatus;				//状态
	unsigned int nPreClose;				//前收盘价
	unsigned int nOpen;					//开盘价
	unsigned int nHigh;					//最高价
	unsigned int nLow;					//最低价
	unsigned int nMatch;				//最新价
	unsigned int nAskPrice[10];			//申卖价
	unsigned int nAskVol[10];			//申卖量
	unsigned int nBidPrice[10];			//申买价
	unsigned int nBidVol[10];			//申买量
	unsigned int nNumTrades;			//成交笔数
	__int64		 iVolume;				//成交总量
	__int64		 iTurnover;				//成交总金额
	__int64		 nTotalBidVol;			//委托买入总量
	__int64		 nTotalAskVol;			//委托卖出总量
	unsigned int nWeightedAvgBidPrice;	//加权平均委买价格
	unsigned int nWeightedAvgAskPrice;  //加权平均委卖价格
	int			 nIOPV;					//IOPV净值估值
	int			 nYieldToMaturity;		//到期收益率
	unsigned int nHighLimited;			//涨停价
	unsigned int nLowLimited;			//跌停价
	char		 chPrefix[4];			//证券信息前缀
	int			 nSyl1;					//市盈率1
	int			 nSyl2;					//市盈率2
	int			 nSD2;					//升跌2（对比上一笔）
};

//数据消息：MSG_DATA_INDEX 对应的结构
struct TDF_INDEX_DATA
{
	char        szWindCode[32];         //600001.SH 
	char        szCode[32];             //原始Code
	int         nActionDay;             //业务发生日(自然日)
	int         nTradingDay;            //交易日
	int         nTime;			//时间(HHMMSSmmm)
	int		    nOpenIndex;		//今开盘指数
	int 	    nHighIndex;		//最高指数
	int 	    nLowIndex;		//最低指数
	int 	    nLastIndex;		//最新指数
	__int64	    iTotalVolume;	//参与计算相应指数的交易数量
	__int64	    iTurnover;		//参与计算相应指数的成交金额
	int		    nPreCloseIndex;	//前盘指数
};

//数据消息：MSG_DATA_FUTURE 对应的结构
struct TDF_FUTURE_DATA
{
	char        szWindCode[32];         //600001.SH 
	char        szCode[32];             //原始Code
	int         nActionDay;             //业务发生日(自然日)
	int         nTradingDay;            //交易日
	int			 nTime;					//时间(HHMMSSmmm)	
	int			 nStatus;				//状态
	__int64		 iPreOpenInterest;		//昨持仓
	unsigned int nPreClose;				//昨收盘价
	unsigned int nPreSettlePrice;		//昨结算
	unsigned int nOpen;					//开盘价	
	unsigned int nHigh;					//最高价
	unsigned int nLow;					//最低价
	unsigned int nMatch;				//最新价
	__int64		 iVolume;				//成交总量
	__int64		 iTurnover;				//成交总金额
	__int64		 iOpenInterest;			//持仓总量
	unsigned int nClose;				//今收盘
	unsigned int nSettlePrice;			//今结算
	unsigned int nHighLimited;			//涨停价
	unsigned int nLowLimited;			//跌停价
	int			 nPreDelta;			    //昨虚实度
	int			 nCurrDelta;            //今虚实度
	unsigned int nAskPrice[5];			//申卖价
	unsigned int nAskVol[5];			//申卖量
	unsigned int nBidPrice[5];			//申买价
	unsigned int nBidVol[5];			//申买量

	//Add 20140605
	int	lAuctionPrice;		//波动性中断参考价
	int	lAuctionQty;		//波动性中断集合竞价虚拟匹配量
};

//数据消息：MSG_DATA_ORDERQUEUE 对应的结构
struct TDF_ORDER_QUEUE
{
	char    szWindCode[32]; //600001.SH 
	char    szCode[32];     //原始Code
	int     nActionDay;     //自然日
	int 	nTime;			//时间(HHMMSSmmm)
	int     nSide;			//买卖方向('B':Bid 'A':Ask)
	int		nPrice;			//委托价格
	int 	nOrders;		//订单数量
	int 	nABItems;		//明细个数
	int 	nABVolume[200];	//订单明细
};

//数据消息：MSG_DATA_TRANSACTION 对应的结构
struct TDF_TRANSACTION
{
	char    szWindCode[32]; //600001.SH 
	char    szCode[32];     //原始Code
	int     nActionDay;     //自然日
	int 	nTime;		    //成交时间(HHMMSSmmm)
	int 	nIndex;		    //成交编号
	int		nPrice;		    //成交价格
	int 	nVolume;	    //成交数量
	int		nTurnover;	    //成交金额
	int     nBSFlag;        //买卖方向(买：'B', 卖：'A', 不明：' ')
	char    chOrderKind;    //成交类别
	char    chFunctionCode; //成交代码
	int	    nAskOrder;	    //叫卖方委托序号
	int	    nBidOrder;	    //叫买方委托序号
};

//数据消息：MSG_DATA_ORDER 对应的结构
struct TDF_ORDER
{
	char    szWindCode[32]; //600001.SH 
	char    szCode[32];     //原始Code
	int 	nActionDay;	    //委托日期(YYMMDD)
	int 	nTime;			//委托时间(HHMMSSmmm)
	int 	nOrder;	        //委托号
	int		nPrice;			//委托价格
	int 	nVolume;		//委托数量
	char    chOrderKind;	//委托类别
	char    chFunctionCode;	//委托代码('B','S','C')
};
#endif // __TDF_API_STRUCT_H__

//////////////////////////////////////////////////////////////////////////
// TDB 结构体
//////////////////////////////////////////////////////////////////////////
#ifndef _TDBAPISTRUCT_H_
#define _TDBAPISTRUCT_H_
//复权方式
typedef enum REFILLFLAG
{
	REFILL_NONE = 0,			//不复权
	REFILL_BACKWARD = 1,		//全程向前复权（从现在向过去）
	REFILL_FORWARD = 2,			//全程向后复权（从过去向现在）
};

typedef enum CYCTYPE
{
	CYC_SECOND,
	CYC_MINUTE,
	CYC_DAY,
	CYC_WEEK,
	CYC_MONTH,
	CYC_SEASON,
	CYC_HAFLYEAR,
	CYC_YEAR,
	CYC_TICKBAR,
};

//买卖方向
typedef enum ORDERSIDE
{
	ORDERSIDE_BID = 0,      //买
	ORDERSIDE_ASK,          //卖
};

//K线数据 KLine
struct TDBDefine_ReqKLine
{
	char chCode[32];            //证券万得代码(AG1312.SHF)
	REFILLFLAG nCQFlag;         //除权标志：不复权，向前复权，向后复权
	int nCQDate;                //复权日期(<=0:全程复权) 格式：YYMMDD，例如20130101表示2013年1月1日
	int nQJFlag;                //全价标志(债券)(0:净价 1:全价)
	CYCTYPE nCycType;           //数据周期：秒线、分钟、日线、周线、月线、季线、半年线、年线
	int nCycDef;                //周期数量：仅当nCycType取值：秒、分钟、日线、周线、月线时，这个字段有效。
	int nAutoComplete;          //自动补齐：仅1秒钟线、1分钟线支持这个标志，（不为0：补齐；0：不补齐）
	int nBeginDate;             //开始日期(交易日，<0:从上市日期开始； 0:从今天开始)
	int nEndDate;               //结束日期(交易日，<=0:跟nBeginDate一样) 
	int nBeginTime;             //开始时间，<=0表示从开始，格式：（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
	int nEndTime;               //结束时间，<=0表示到结束，格式：（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
};

struct TDBDefine_KLine
{
	char chWindCode[32];            //万得代码(AG1312.SHF)
	char chCode[32];                //交易所代码(ag1312)
	int nDate;                      //日期（自然日）格式：YYMMDD，例如20130101表示2013年1月1日，0表示今天
	int nTime;                      //时间（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
	int    nOpen;                   //开盘((a double number + 0.00005) *10000)
	int    nHigh;                   //最高((a double number + 0.00005) *10000)
	int    nLow;                    //最低((a double number + 0.00005) *10000)
	int    nClose;                  //收盘((a double number + 0.00005) *10000)
	__int64 iVolume;                //成交量
	__int64    iTurover;            //成交额(元)
	int    nMatchItems;             //成交笔数
	int nInterest;                  //持仓量(期货)、IOPV(基金)、利息(债券)
};

struct TDBDefine_Tick
{
	char chWindCode[32];                //万得代码(AG1312.SHF)
	char chCode[32];                    //交易所代码(ag1312)
	int nDate;                          //日期（自然日）
	int nTime;                          //时间（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
	int nPrice;                         //成交价((a double number + 0.00005) *10000)
	__int64 iVolume;                    //成交量
	__int64    iTurover;                //成交额(元)
	int nMatchItems;                    //成交笔数
	int nInterest;                      //IOPV(基金)、利息(债券)
	char chTradeFlag;                   //成交标志
	char chBSFlag;                      //BS标志
	__int64 iAccVolume;                 //当日累计成交量
	__int64    iAccTurover;             //当日成交额(元)
	int nHigh;                          //最高((a double number + 0.00005) *10000)
	int nLow;                           //最低((a double number + 0.00005) *10000)
	int    nOpen;                       //开盘((a double number + 0.00005) *10000)
	int    nPreClose;                   //前收盘((a double number + 0.00005) *10000)

	//下面的字段指数使用
	int        nIndex;                  //不加权指数
	int        nStocks;                 //品种总数
	int        nUps;                    //上涨品种数
	int        nDowns;                  //下跌品种数
	int        nHoldLines;              //持平品种数
};


struct TDBDefine_ReqTick
{
	char chCode[32];    //证券万得代码(AG1312.SHF)
	int  nBeginDate;    //开始日期（交易日）,为0则从今天，格式：YYMMDD，例如20130101表示2013年1月1日
	int  nEndDate;      //结束日期（交易日），为0则和nBeginDate一样
	int  nBeginTime;    //开始时间：若<=0则从头，格式：（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
	int  nEndTime;      //结束时间：若<=0则至最后
};

struct TDBDefine_TickAB
{
	char chWindCode[32];                //万得代码(AG1312.SHF)
	char chCode[32];                    //交易所代码(ag1312)
	int nDate;                          //日期（自然日）格式YYMMDD
	int nTime;                          //时间（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
	int nPrice;                         //成交价((a double number + 0.00005) *10000)
	__int64 iVolume;                    //成交量
	__int64    iTurover;                //成交额(元)
	int nMatchItems;                    //成交笔数
	int nInterest;                      //IOPV(基金)、利息(债券)
	char chTradeFlag;                   //成交标志
	char chBSFlag;                      //BS标志
	__int64 iAccVolume;                 //当日累计成交量
	__int64    iAccTurover;             //当日成交额(元)
	int nHigh;                          //最高((a double number + 0.00005) *10000)
	int nLow;                           //最低((a double number + 0.00005) *10000)
	int    nOpen;                       //开盘((a double number + 0.00005) *10000)
	int    nPreClose;                   //前收盘((a double number + 0.00005) *10000)
	int    nAskPrice[10];               //叫卖价((a double number + 0.00005) *10000)
	unsigned nAskVolume[10];            //叫卖量
	int    nBidPrice[10];               //叫买价((a double number + 0.00005) *10000)
	unsigned nBidVolume[10];            //叫买量
	int    nAskAvPrice;                 //加权平均叫卖价(上海L2)((a double number + 0.00005) *10000)
	int    nBidAvPrice;                 //加权平均叫买价(上海L2)((a double number + 0.00005) *10000)
	__int64  iTotalAskVolume;           //叫卖总量(上海L2)
	__int64  iTotalBidVolume;           //叫买总量(上海L2)

	//下面的字段指数使用
	int        nIndex;                  //不加权指数
	int        nStocks;                 //品种总数
	int        nUps;                    //上涨品种数
	int        nDowns;                  //下跌品种数
	int        nHoldLines;              //持平品种数
};


struct TDBDefine_ReqFuture
{
	char chCode[32];    //证券万得代码(AG1312.SHF)
	int  nBeginDate;    //开始日期（交易日）,为0则从当天，例如20130101
	int  nEndDate;      //结束日期（交易日），小于等于0则和nBeginDate相同
	int  nBeginTime;    //开始时间：若<=0则从头，格式：（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
	int  nEndTime;      //结束时间：若<=0则至最后

	int nAutoComplete;  //自动补齐标志:( 0：不自动补齐，1:自动补齐）
};

struct TDBDefine_FutureAB
{
	char chWindCode[32];                //万得代码(AG1312.SHF)
	char chCode[32];                    //交易所代码(ag1312)
	int nDate;                          //日期（自然日）格式：YYMMDD
	int nTime;                          //时间（精确到毫秒，HHMMSSmmm）

	__int64 iVolume;                    //成交量
	__int64    iTurover;                //成交额(元)

	int nSettle;                        //结算价((a double number + 0.00005) *10000)
	int nPosition;                       //持仓量
	int nCurDelta;                      //虚实度

	char chTradeFlag;                   //成交标志

	__int64 iAccVolume;                 //当日累计成交量
	__int64    iAccTurover;             //当日成交额(元)

	int nOpen;                          //开盘((a double number + 0.00005) *10000)
	int nHigh;                          //最高((a double number + 0.00005) *10000)
	int nLow;                           //最低((a double number + 0.00005) *10000)
	int nPrice;                         //成交价((a double number + 0.00005) *10000)

	int    nAskPrice[5];               //叫卖价((a double number + 0.00005) *10000)
	unsigned int nAskVolume[5];        //叫卖量
	int    nBidPrice[5];               //叫买价((a double number + 0.00005) *10000)
	unsigned int nBidVolume[5];        //叫买量

	int nPreClose;                     //前收盘((a double number + 0.00005) *10000)
	int nPreSettle;                    //昨结算((a double number + 0.00005) *10000)
	int nPrePosition;                  //昨持仓
};

struct TDBDefine_Future
{
	char chWindCode[32];               //万得代码(AG1312.SHF)
	char chCode[32];                   //交易所代码(ag1312)
	int nDate;                         //日期（自然日）格式：YYMMDD
	int nTime;                         //时间，格式：HHMMSSmmm

	__int64 iVolume;                   //成交量
	__int64    iTurover;               //成交额(元)

	int nSettle;                       //结算价((a double number + 0.00005) *10000)
	int nPosition;                     //持仓量
	int nCurDelta;                     //虚实度

	char chTradeFlag;                  //成交标志（港股有值）

	__int64 iAccVolume;                //当日累计成交量
	__int64    iAccTurover;            //当日成交额(元)

	int nHigh;                         //最高((a double number + 0.00005) *10000)
	int nLow;                          //最低((a double number + 0.00005) *10000)
	int nOpen;                         //开盘((a double number + 0.00005) *10000)
	int nPrice;                        //成交价((a double number + 0.00005) *10000)

	int nPreClose;                     //前收盘((a double number + 0.00005) *10000)
	int nPreSettle;                    //昨结算((a double number + 0.00005) *10000)
	int nPrePosition;                  //昨持仓
};


//逐笔成交数据 Transaction
struct TDBDefine_ReqTransaction
{
	char chCode[32];            //证券万得代码(AG1312.SHF)
	int  nBeginDate;            //开始日期（交易日），格式YYMMDD
	int  nEndDate;              //数据日期（交易日）小于等于0和nBeginDate相同
	int  nBeginTime;            //开始时间:<=0表示从0开始，格式：HHMMSSmmm
	int  nEndTime;              //结束时间：<=0表示到最后
};

struct TDBDefine_Transaction
{
	char    chWindCode[32];     //万得代码(AG1312.SHF)
	char    chCode[32];         //交易所代码(ag1312)
	int     nDate;              //日期（自然日）格式:YYMMDD
	int     nTime;              //成交时间(精确到毫秒HHMMSSmmm)
	int     nIndex;             //成交编号(从1开始，递增1)
	char    chFunctionCode;     //成交代码: 'C', 0
	char    chOrderKind;        //委托类别
	char    chBSFlag;           //BS标志
	int     nTradePrice;        //成交价格((a double number + 0.00005) *10000)
	int     nTradeVolume;       //成交数量
	int     nAskOrder;          //叫卖序号
	int     nBidOrder;          //叫买序号
};

//逐笔委托数据 Order (SZ Level2 Only) 
typedef struct TDBDefine_ReqTransaction TDBDefine_ReqOrder;

struct TDBDefine_Order
{
	char chWindCode[32];        //万得代码(AG1312.SHF)
	char chCode[32];            //交易所代码(ag1312)
	int  nDate;                 //日期（自然日）格式YYMMDD
	int  nTime;                 //委托时间（精确到毫秒HHMMSSmmm）
	int  nIndex;                //委托编号，从1开始，递增1
	int  nOrder;                //交易所委托号
	char chOrderKind;           //委托类别
	char chFunctionCode;        //委托代码, B, S, C
	int  nOrderPrice;           //委托价格((a double number + 0.00005) *10000)
	int  nOrderVolume;          //委托数量
};

//委托队列数据 OrderQueue
typedef struct TDBDefine_ReqTransaction TDBDefine_ReqOrderQueue;

struct TDBDefine_OrderQueue
{
	char    chWindCode[32];         //万得代码(AG1312.SHF)
	char    chCode[32];             //交易所代码(ag1312)
	int     nDate;                  //日期（自然日）格式YYMMDD
	int     nTime;                  //订单时间(精确到毫秒HHMMSSmmm)
	int     nSide;                  //买卖方向('B':Bid 'A':Ask)
	int     nPrice;                 //成交价格((a double number + 0.00005) *10000)
	int     nOrderItems;            //订单数量
	int     nABItems;               //明细个数
	int     nABVolume[50];          //订单明细
};

#endif // _TDBAPISTRUCT_H_

typedef char GD_CodeType[32];
typedef char GD_ISODateTimeType[21];	//日期和时间类型(格式 yyyy-MM-dd hh:mm:ss)

enum GD_ErrorType
{
	none_GD_ErrorType,					//无
	//登录
	userError_GD_ErrorType = 1,			//用户不存在或用户名密码错误
	overLoginNumError_GD_ErrorType,		//登录数超限
	//请求错误
	serverConnectedError_GD_ErrorType	//服务器连接错误
};

enum GD_StockType{
	stock_GD_StockType = 0,			//股票
	index_GD_StockType,				//指数
	future_GD_StockType,			//期货
	option_GD_StockType,			//期权

	none_GD_StockType = -1
};

typedef enum GD_CycType			//为了与 CYCTYPE 兼容，从100开始
{
	second_GD_CycType = 100,	//秒
	second_10_GD_CycType,		//10秒
	minute_GD_CycType,			//分
	minute_5_GD_CycType,		//5分
	minute_15_GD_CycType,		//15分
	minute_30_GD_CycType,		//30分
	hour_GD_CycType				//小时
};

enum GD_UpdateSubCodeType
{
	add_GD_UpdateSubCodeType = 0,		//添加订阅代码
	remove_GD_UpdateSubCodeType,		//移除已有订阅代码
	replace_GD_UpdateSubCodeType		//替换全部订阅代码
};

enum GD_ServerType		//服务器所属类型
{
	realTime_GD_ServerType = 0,			//实时行情服务器
	simulation_GD_ServerType,			//模拟行情服务器
	history_GD_ServerType				//历史数据服务器
};

struct GD_Server		//服务器信息定义
{
	char szName[32];
	char szIp[32];
	int nPort;
	GD_ServerType nType;		//服务器类型
	char szVersion[32];			//兼容最低api版本
};

struct GD_User 
{
	char szName[32];
	char szPass[32];
	int nMaxLoginNum;
	char szRealTimeServer[32];		//实时行情服务器 名称
	char szSimulationServer[32];	//模拟行情服务器 名称
	char szHistoryServer[32];		//历史行情服务器 名称
};

//////////////////////////////////////////////////////////////////////////
//请求
struct GD_Login_Req			//登录请求
{
	char szName[32];		//用户名
	char szPass[32];		//密码
	char szSystem[32];		//api系统信息
	char szVersion[32];		//api版本
};

struct GD_UserInfo_Req		//连接用户信息
{
	char szName[32];		//用户名
	char szVersion[32];		//Api 版本
	char szSystem[32];		//系统信息
};

struct GD_RealTime_Req					//实盘数据请求
{
	bool	isAllMarket;				//是否订阅全市场
	int		nCodeNum;					//代码条数
	GD_CodeType szCodes[1];				//订阅代码首地址
};

struct GD_Simulation_Req				//模拟数据请求
{
	GD_ISODateTimeType szBeginDatetime;	//请求开始的时间
	GD_ISODateTimeType szEndDatetime;	//请求结束的时间
	bool	isAllMarket;				//是否订阅全市场
	int		nCodeNum;					//代码条数
	GD_CodeType szCodes[1];				//订阅代码首地址
};

struct GD_UpdateSubCodes_Req
{
	GD_UpdateSubCodeType nType;			//请求类型
	bool	isAllMarket;				//是否订阅全市场
	int		nCodeNum;					//代码条数
	GD_CodeType szCodes[1];				//订阅代码首地址
};

struct GD_Kline_Req						//模拟数据请求
{
	GD_CycType nType;					//周期类型
	GD_ISODateTimeType szBeginDatetime;	//请求开始的时间
	GD_ISODateTimeType szEndDatetime;	//请求结束的时间
	bool	isAllMarket;				//是否订阅全市场
	int		nCodeNum;					//代码条数
	GD_CodeType szCodes[1];				//订阅代码首地址
};

//////////////////////////////////////////////////////////////////////////
//回调
struct GD_Login_Rsp			//登录回调
{
	GD_ErrorType nErrCode;				//错误码
	char szErr[32];						//错误描述
	GD_Server realTimeServer;
	GD_Server simulationServer;
	GD_Server historyServer;
	int nCodesNum;
	int nOptionInfosNum;
	TDF_CODE* code;						//代码表
	TDF_OPTION_CODE* optionInfo;		//期权代码表
};

typedef GD_ISODateTimeType GD_TradingDateBegin_Rsp;		//交易日开始
typedef GD_ISODateTimeType GD_TradingDateEnd_Rsp;		//交易日结束

struct GD_Kline_Data						//模拟数据请求
{
	GD_CycType nType;					//周期类型
	char	szCode[32];					//是否订阅全市场
	GD_ISODateTimeType szDatetime;		//时间
	double	nOpen;
	double	nHigh;
	double	nLow;
	double	nClose;
	double	nHighLimit;
	double	nLowLimit;
	unsigned long long iVolume;
	double	nTurover;
};

#endif // GETDATASTRUCT_H
