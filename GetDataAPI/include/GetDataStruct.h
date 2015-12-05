#ifndef GETDATASTRUCT_H
#define GETDATASTRUCT_H

//////////////////////////////////////////////////////////////////////////
//TDF �ṹ��
//////////////////////////////////////////////////////////////////////////
#ifndef __TDF_API_STRUCT_H__
#define __TDF_API_STRUCT_H__
struct TDF_MARKET_CLOSE
{
	char    szMarket[8];        //����������
	int		nTime;				//ʱ��(HHMMSSmmm)
	char	chInfo[64];			//������Ϣ
};

struct TDF_CODE
{
	char szWindCode[32];		//Wind Code: AG1302.SHF
	char szMarket[8];			//market code: SHF
	char szCode[32];			//original code:ag1302
	char szENName[32];
	char szCNName[32];			//chinese name: ����1302
	int nType;
};

struct TDF_OPTION_CODE
{
	TDF_CODE basicCode;

	char szContractID[32];			// ��Ȩ��Լ����
	char szUnderlyingSecurityID[32];//// ���֤ȯ����
	char chCallOrPut;               // �Ϲ��Ϲ�C1        �Ϲ������ֶ�Ϊ��C������Ϊ�Ϲ������ֶ�Ϊ��P��
	int  nExerciseDate;             // ��Ȩ��Ȩ�գ�YYYYMMDD

	//�����ֶ�
	char chUnderlyingType;			// ���֤ȯ����C3    0-A�� 1-ETF (EBS �C ETF�� ASH �C A ��)
	char chOptionType;              // ŷʽ��ʽC1        ��Ϊŷʽ��Ȩ�����ֶ�Ϊ��E������Ϊ��ʽ��Ȩ�����ֶ�Ϊ��A��

	char chPriceLimitType;          // �ǵ�����������C1 ��N����ʾ���ǵ�����������, ��R����ʾ���ǵ�����������
	int  nContractMultiplierUnit;	// ��Լ��λ,         ������Ȩ��Ϣ������ĺ�Լ��λ, һ��������
	int  nExercisePrice;            // ��Ȩ��Ȩ��,       ������Ȩ��Ϣ���������Ȩ��Ȩ�ۣ��Ҷ��룬��ȷ����
	int  nStartDate;                // ��Ȩ�׸�������,YYYYMMDD
	int  nEndDate;                  // ��Ȩ�������/��Ȩ�գ�YYYYMMDD
	int  nExpireDate;               // ��Ȩ�����գ�YYYYMMDD
};

//������Ϣ��MSG_DATA_MARKET ��Ӧ�Ľṹ
struct TDF_MARKET_DATA
{
	char        szWindCode[32];         //600001.SH 
	char        szCode[32];             //ԭʼCode
	int         nActionDay;             //ҵ������(��Ȼ��)
	int         nTradingDay;            //������
	int			 nTime;					//ʱ��(HHMMSSmmm)
	int			 nStatus;				//״̬
	unsigned int nPreClose;				//ǰ���̼�
	unsigned int nOpen;					//���̼�
	unsigned int nHigh;					//��߼�
	unsigned int nLow;					//��ͼ�
	unsigned int nMatch;				//���¼�
	unsigned int nAskPrice[10];			//������
	unsigned int nAskVol[10];			//������
	unsigned int nBidPrice[10];			//�����
	unsigned int nBidVol[10];			//������
	unsigned int nNumTrades;			//�ɽ�����
	__int64		 iVolume;				//�ɽ�����
	__int64		 iTurnover;				//�ɽ��ܽ��
	__int64		 nTotalBidVol;			//ί����������
	__int64		 nTotalAskVol;			//ί����������
	unsigned int nWeightedAvgBidPrice;	//��Ȩƽ��ί��۸�
	unsigned int nWeightedAvgAskPrice;  //��Ȩƽ��ί���۸�
	int			 nIOPV;					//IOPV��ֵ��ֵ
	int			 nYieldToMaturity;		//����������
	unsigned int nHighLimited;			//��ͣ��
	unsigned int nLowLimited;			//��ͣ��
	char		 chPrefix[4];			//֤ȯ��Ϣǰ׺
	int			 nSyl1;					//��ӯ��1
	int			 nSyl2;					//��ӯ��2
	int			 nSD2;					//����2���Ա���һ�ʣ�
};

//������Ϣ��MSG_DATA_INDEX ��Ӧ�Ľṹ
struct TDF_INDEX_DATA
{
	char        szWindCode[32];         //600001.SH 
	char        szCode[32];             //ԭʼCode
	int         nActionDay;             //ҵ������(��Ȼ��)
	int         nTradingDay;            //������
	int         nTime;			//ʱ��(HHMMSSmmm)
	int		    nOpenIndex;		//����ָ��
	int 	    nHighIndex;		//���ָ��
	int 	    nLowIndex;		//���ָ��
	int 	    nLastIndex;		//����ָ��
	__int64	    iTotalVolume;	//���������Ӧָ���Ľ�������
	__int64	    iTurnover;		//���������Ӧָ���ĳɽ����
	int		    nPreCloseIndex;	//ǰ��ָ��
};

//������Ϣ��MSG_DATA_FUTURE ��Ӧ�Ľṹ
struct TDF_FUTURE_DATA
{
	char        szWindCode[32];         //600001.SH 
	char        szCode[32];             //ԭʼCode
	int         nActionDay;             //ҵ������(��Ȼ��)
	int         nTradingDay;            //������
	int			 nTime;					//ʱ��(HHMMSSmmm)	
	int			 nStatus;				//״̬
	__int64		 iPreOpenInterest;		//��ֲ�
	unsigned int nPreClose;				//�����̼�
	unsigned int nPreSettlePrice;		//�����
	unsigned int nOpen;					//���̼�	
	unsigned int nHigh;					//��߼�
	unsigned int nLow;					//��ͼ�
	unsigned int nMatch;				//���¼�
	__int64		 iVolume;				//�ɽ�����
	__int64		 iTurnover;				//�ɽ��ܽ��
	__int64		 iOpenInterest;			//�ֲ�����
	unsigned int nClose;				//������
	unsigned int nSettlePrice;			//�����
	unsigned int nHighLimited;			//��ͣ��
	unsigned int nLowLimited;			//��ͣ��
	int			 nPreDelta;			    //����ʵ��
	int			 nCurrDelta;            //����ʵ��
	unsigned int nAskPrice[5];			//������
	unsigned int nAskVol[5];			//������
	unsigned int nBidPrice[5];			//�����
	unsigned int nBidVol[5];			//������

	//Add 20140605
	int	lAuctionPrice;		//�������жϲο���
	int	lAuctionQty;		//�������жϼ��Ͼ�������ƥ����
};

//������Ϣ��MSG_DATA_ORDERQUEUE ��Ӧ�Ľṹ
struct TDF_ORDER_QUEUE
{
	char    szWindCode[32]; //600001.SH 
	char    szCode[32];     //ԭʼCode
	int     nActionDay;     //��Ȼ��
	int 	nTime;			//ʱ��(HHMMSSmmm)
	int     nSide;			//��������('B':Bid 'A':Ask)
	int		nPrice;			//ί�м۸�
	int 	nOrders;		//��������
	int 	nABItems;		//��ϸ����
	int 	nABVolume[200];	//������ϸ
};

//������Ϣ��MSG_DATA_TRANSACTION ��Ӧ�Ľṹ
struct TDF_TRANSACTION
{
	char    szWindCode[32]; //600001.SH 
	char    szCode[32];     //ԭʼCode
	int     nActionDay;     //��Ȼ��
	int 	nTime;		    //�ɽ�ʱ��(HHMMSSmmm)
	int 	nIndex;		    //�ɽ����
	int		nPrice;		    //�ɽ��۸�
	int 	nVolume;	    //�ɽ�����
	int		nTurnover;	    //�ɽ����
	int     nBSFlag;        //��������(��'B', ����'A', ������' ')
	char    chOrderKind;    //�ɽ����
	char    chFunctionCode; //�ɽ�����
	int	    nAskOrder;	    //������ί�����
	int	    nBidOrder;	    //����ί�����
};

//������Ϣ��MSG_DATA_ORDER ��Ӧ�Ľṹ
struct TDF_ORDER
{
	char    szWindCode[32]; //600001.SH 
	char    szCode[32];     //ԭʼCode
	int 	nActionDay;	    //ί������(YYMMDD)
	int 	nTime;			//ί��ʱ��(HHMMSSmmm)
	int 	nOrder;	        //ί�к�
	int		nPrice;			//ί�м۸�
	int 	nVolume;		//ί������
	char    chOrderKind;	//ί�����
	char    chFunctionCode;	//ί�д���('B','S','C')
};
#endif // __TDF_API_STRUCT_H__

//////////////////////////////////////////////////////////////////////////
// TDB �ṹ��
//////////////////////////////////////////////////////////////////////////
#ifndef _TDBAPISTRUCT_H_
#define _TDBAPISTRUCT_H_
//��Ȩ��ʽ
typedef enum REFILLFLAG
{
	REFILL_NONE = 0,			//����Ȩ
	REFILL_BACKWARD = 1,		//ȫ����ǰ��Ȩ�����������ȥ��
	REFILL_FORWARD = 2,			//ȫ�����Ȩ���ӹ�ȥ�����ڣ�
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

//��������
typedef enum ORDERSIDE
{
	ORDERSIDE_BID = 0,      //��
	ORDERSIDE_ASK,          //��
};

//K������ KLine
struct TDBDefine_ReqKLine
{
	char chCode[32];            //֤ȯ��ô���(AG1312.SHF)
	REFILLFLAG nCQFlag;         //��Ȩ��־������Ȩ����ǰ��Ȩ�����Ȩ
	int nCQDate;                //��Ȩ����(<=0:ȫ�̸�Ȩ) ��ʽ��YYMMDD������20130101��ʾ2013��1��1��
	int nQJFlag;                //ȫ�۱�־(ծȯ)(0:���� 1:ȫ��)
	CYCTYPE nCycType;           //�������ڣ����ߡ����ӡ����ߡ����ߡ����ߡ����ߡ������ߡ�����
	int nCycDef;                //��������������nCycTypeȡֵ���롢���ӡ����ߡ����ߡ�����ʱ������ֶ���Ч��
	int nAutoComplete;          //�Զ����룺��1�����ߡ�1������֧�������־������Ϊ0�����룻0�������룩
	int nBeginDate;             //��ʼ����(�����գ�<0:���������ڿ�ʼ�� 0:�ӽ��쿪ʼ)
	int nEndDate;               //��������(�����գ�<=0:��nBeginDateһ��) 
	int nBeginTime;             //��ʼʱ�䣬<=0��ʾ�ӿ�ʼ����ʽ����HHMMSSmmm������94500000 ��ʾ 9��45��00��000����
	int nEndTime;               //����ʱ�䣬<=0��ʾ����������ʽ����HHMMSSmmm������94500000 ��ʾ 9��45��00��000����
};

struct TDBDefine_KLine
{
	char chWindCode[32];            //��ô���(AG1312.SHF)
	char chCode[32];                //����������(ag1312)
	int nDate;                      //���ڣ���Ȼ�գ���ʽ��YYMMDD������20130101��ʾ2013��1��1�գ�0��ʾ����
	int nTime;                      //ʱ�䣨HHMMSSmmm������94500000 ��ʾ 9��45��00��000����
	int    nOpen;                   //����((a double number + 0.00005) *10000)
	int    nHigh;                   //���((a double number + 0.00005) *10000)
	int    nLow;                    //���((a double number + 0.00005) *10000)
	int    nClose;                  //����((a double number + 0.00005) *10000)
	__int64 iVolume;                //�ɽ���
	__int64    iTurover;            //�ɽ���(Ԫ)
	int    nMatchItems;             //�ɽ�����
	int nInterest;                  //�ֲ���(�ڻ�)��IOPV(����)����Ϣ(ծȯ)
};

struct TDBDefine_Tick
{
	char chWindCode[32];                //��ô���(AG1312.SHF)
	char chCode[32];                    //����������(ag1312)
	int nDate;                          //���ڣ���Ȼ�գ�
	int nTime;                          //ʱ�䣨HHMMSSmmm������94500000 ��ʾ 9��45��00��000����
	int nPrice;                         //�ɽ���((a double number + 0.00005) *10000)
	__int64 iVolume;                    //�ɽ���
	__int64    iTurover;                //�ɽ���(Ԫ)
	int nMatchItems;                    //�ɽ�����
	int nInterest;                      //IOPV(����)����Ϣ(ծȯ)
	char chTradeFlag;                   //�ɽ���־
	char chBSFlag;                      //BS��־
	__int64 iAccVolume;                 //�����ۼƳɽ���
	__int64    iAccTurover;             //���ճɽ���(Ԫ)
	int nHigh;                          //���((a double number + 0.00005) *10000)
	int nLow;                           //���((a double number + 0.00005) *10000)
	int    nOpen;                       //����((a double number + 0.00005) *10000)
	int    nPreClose;                   //ǰ����((a double number + 0.00005) *10000)

	//������ֶ�ָ��ʹ��
	int        nIndex;                  //����Ȩָ��
	int        nStocks;                 //Ʒ������
	int        nUps;                    //����Ʒ����
	int        nDowns;                  //�µ�Ʒ����
	int        nHoldLines;              //��ƽƷ����
};


struct TDBDefine_ReqTick
{
	char chCode[32];    //֤ȯ��ô���(AG1312.SHF)
	int  nBeginDate;    //��ʼ���ڣ������գ�,Ϊ0��ӽ��죬��ʽ��YYMMDD������20130101��ʾ2013��1��1��
	int  nEndDate;      //�������ڣ������գ���Ϊ0���nBeginDateһ��
	int  nBeginTime;    //��ʼʱ�䣺��<=0���ͷ����ʽ����HHMMSSmmm������94500000 ��ʾ 9��45��00��000����
	int  nEndTime;      //����ʱ�䣺��<=0�������
};

struct TDBDefine_TickAB
{
	char chWindCode[32];                //��ô���(AG1312.SHF)
	char chCode[32];                    //����������(ag1312)
	int nDate;                          //���ڣ���Ȼ�գ���ʽYYMMDD
	int nTime;                          //ʱ�䣨HHMMSSmmm������94500000 ��ʾ 9��45��00��000����
	int nPrice;                         //�ɽ���((a double number + 0.00005) *10000)
	__int64 iVolume;                    //�ɽ���
	__int64    iTurover;                //�ɽ���(Ԫ)
	int nMatchItems;                    //�ɽ�����
	int nInterest;                      //IOPV(����)����Ϣ(ծȯ)
	char chTradeFlag;                   //�ɽ���־
	char chBSFlag;                      //BS��־
	__int64 iAccVolume;                 //�����ۼƳɽ���
	__int64    iAccTurover;             //���ճɽ���(Ԫ)
	int nHigh;                          //���((a double number + 0.00005) *10000)
	int nLow;                           //���((a double number + 0.00005) *10000)
	int    nOpen;                       //����((a double number + 0.00005) *10000)
	int    nPreClose;                   //ǰ����((a double number + 0.00005) *10000)
	int    nAskPrice[10];               //������((a double number + 0.00005) *10000)
	unsigned nAskVolume[10];            //������
	int    nBidPrice[10];               //�����((a double number + 0.00005) *10000)
	unsigned nBidVolume[10];            //������
	int    nAskAvPrice;                 //��Ȩƽ��������(�Ϻ�L2)((a double number + 0.00005) *10000)
	int    nBidAvPrice;                 //��Ȩƽ�������(�Ϻ�L2)((a double number + 0.00005) *10000)
	__int64  iTotalAskVolume;           //��������(�Ϻ�L2)
	__int64  iTotalBidVolume;           //��������(�Ϻ�L2)

	//������ֶ�ָ��ʹ��
	int        nIndex;                  //����Ȩָ��
	int        nStocks;                 //Ʒ������
	int        nUps;                    //����Ʒ����
	int        nDowns;                  //�µ�Ʒ����
	int        nHoldLines;              //��ƽƷ����
};


struct TDBDefine_ReqFuture
{
	char chCode[32];    //֤ȯ��ô���(AG1312.SHF)
	int  nBeginDate;    //��ʼ���ڣ������գ�,Ϊ0��ӵ��죬����20130101
	int  nEndDate;      //�������ڣ������գ���С�ڵ���0���nBeginDate��ͬ
	int  nBeginTime;    //��ʼʱ�䣺��<=0���ͷ����ʽ����HHMMSSmmm������94500000 ��ʾ 9��45��00��000����
	int  nEndTime;      //����ʱ�䣺��<=0�������

	int nAutoComplete;  //�Զ������־:( 0�����Զ����룬1:�Զ����룩
};

struct TDBDefine_FutureAB
{
	char chWindCode[32];                //��ô���(AG1312.SHF)
	char chCode[32];                    //����������(ag1312)
	int nDate;                          //���ڣ���Ȼ�գ���ʽ��YYMMDD
	int nTime;                          //ʱ�䣨��ȷ�����룬HHMMSSmmm��

	__int64 iVolume;                    //�ɽ���
	__int64    iTurover;                //�ɽ���(Ԫ)

	int nSettle;                        //�����((a double number + 0.00005) *10000)
	int nPosition;                       //�ֲ���
	int nCurDelta;                      //��ʵ��

	char chTradeFlag;                   //�ɽ���־

	__int64 iAccVolume;                 //�����ۼƳɽ���
	__int64    iAccTurover;             //���ճɽ���(Ԫ)

	int nOpen;                          //����((a double number + 0.00005) *10000)
	int nHigh;                          //���((a double number + 0.00005) *10000)
	int nLow;                           //���((a double number + 0.00005) *10000)
	int nPrice;                         //�ɽ���((a double number + 0.00005) *10000)

	int    nAskPrice[5];               //������((a double number + 0.00005) *10000)
	unsigned int nAskVolume[5];        //������
	int    nBidPrice[5];               //�����((a double number + 0.00005) *10000)
	unsigned int nBidVolume[5];        //������

	int nPreClose;                     //ǰ����((a double number + 0.00005) *10000)
	int nPreSettle;                    //�����((a double number + 0.00005) *10000)
	int nPrePosition;                  //��ֲ�
};

struct TDBDefine_Future
{
	char chWindCode[32];               //��ô���(AG1312.SHF)
	char chCode[32];                   //����������(ag1312)
	int nDate;                         //���ڣ���Ȼ�գ���ʽ��YYMMDD
	int nTime;                         //ʱ�䣬��ʽ��HHMMSSmmm

	__int64 iVolume;                   //�ɽ���
	__int64    iTurover;               //�ɽ���(Ԫ)

	int nSettle;                       //�����((a double number + 0.00005) *10000)
	int nPosition;                     //�ֲ���
	int nCurDelta;                     //��ʵ��

	char chTradeFlag;                  //�ɽ���־���۹���ֵ��

	__int64 iAccVolume;                //�����ۼƳɽ���
	__int64    iAccTurover;            //���ճɽ���(Ԫ)

	int nHigh;                         //���((a double number + 0.00005) *10000)
	int nLow;                          //���((a double number + 0.00005) *10000)
	int nOpen;                         //����((a double number + 0.00005) *10000)
	int nPrice;                        //�ɽ���((a double number + 0.00005) *10000)

	int nPreClose;                     //ǰ����((a double number + 0.00005) *10000)
	int nPreSettle;                    //�����((a double number + 0.00005) *10000)
	int nPrePosition;                  //��ֲ�
};


//��ʳɽ����� Transaction
struct TDBDefine_ReqTransaction
{
	char chCode[32];            //֤ȯ��ô���(AG1312.SHF)
	int  nBeginDate;            //��ʼ���ڣ������գ�����ʽYYMMDD
	int  nEndDate;              //�������ڣ������գ�С�ڵ���0��nBeginDate��ͬ
	int  nBeginTime;            //��ʼʱ��:<=0��ʾ��0��ʼ����ʽ��HHMMSSmmm
	int  nEndTime;              //����ʱ�䣺<=0��ʾ�����
};

struct TDBDefine_Transaction
{
	char    chWindCode[32];     //��ô���(AG1312.SHF)
	char    chCode[32];         //����������(ag1312)
	int     nDate;              //���ڣ���Ȼ�գ���ʽ:YYMMDD
	int     nTime;              //�ɽ�ʱ��(��ȷ������HHMMSSmmm)
	int     nIndex;             //�ɽ����(��1��ʼ������1)
	char    chFunctionCode;     //�ɽ�����: 'C', 0
	char    chOrderKind;        //ί�����
	char    chBSFlag;           //BS��־
	int     nTradePrice;        //�ɽ��۸�((a double number + 0.00005) *10000)
	int     nTradeVolume;       //�ɽ�����
	int     nAskOrder;          //�������
	int     nBidOrder;          //�������
};

//���ί������ Order (SZ Level2 Only) 
typedef struct TDBDefine_ReqTransaction TDBDefine_ReqOrder;

struct TDBDefine_Order
{
	char chWindCode[32];        //��ô���(AG1312.SHF)
	char chCode[32];            //����������(ag1312)
	int  nDate;                 //���ڣ���Ȼ�գ���ʽYYMMDD
	int  nTime;                 //ί��ʱ�䣨��ȷ������HHMMSSmmm��
	int  nIndex;                //ί�б�ţ���1��ʼ������1
	int  nOrder;                //������ί�к�
	char chOrderKind;           //ί�����
	char chFunctionCode;        //ί�д���, B, S, C
	int  nOrderPrice;           //ί�м۸�((a double number + 0.00005) *10000)
	int  nOrderVolume;          //ί������
};

//ί�ж������� OrderQueue
typedef struct TDBDefine_ReqTransaction TDBDefine_ReqOrderQueue;

struct TDBDefine_OrderQueue
{
	char    chWindCode[32];         //��ô���(AG1312.SHF)
	char    chCode[32];             //����������(ag1312)
	int     nDate;                  //���ڣ���Ȼ�գ���ʽYYMMDD
	int     nTime;                  //����ʱ��(��ȷ������HHMMSSmmm)
	int     nSide;                  //��������('B':Bid 'A':Ask)
	int     nPrice;                 //�ɽ��۸�((a double number + 0.00005) *10000)
	int     nOrderItems;            //��������
	int     nABItems;               //��ϸ����
	int     nABVolume[50];          //������ϸ
};

#endif // _TDBAPISTRUCT_H_

typedef char GD_CodeType[32];
typedef char GD_ISODateTimeType[21];	//���ں�ʱ������(��ʽ yyyy-MM-dd hh:mm:ss)

enum GD_ErrorType
{
	none_GD_ErrorType,					//��
	//��¼
	userError_GD_ErrorType = 1,			//�û������ڻ��û����������
	overLoginNumError_GD_ErrorType,		//��¼������
	//�������
	serverConnectedError_GD_ErrorType	//���������Ӵ���
};

enum GD_StockType{
	stock_GD_StockType = 0,			//��Ʊ
	index_GD_StockType,				//ָ��
	future_GD_StockType,			//�ڻ�
	option_GD_StockType,			//��Ȩ

	none_GD_StockType = -1
};

typedef enum GD_CycType			//Ϊ���� CYCTYPE ���ݣ���100��ʼ
{
	second_GD_CycType = 100,	//��
	second_10_GD_CycType,		//10��
	minute_GD_CycType,			//��
	minute_5_GD_CycType,		//5��
	minute_15_GD_CycType,		//15��
	minute_30_GD_CycType,		//30��
	hour_GD_CycType				//Сʱ
};

enum GD_UpdateSubCodeType
{
	add_GD_UpdateSubCodeType = 0,		//��Ӷ��Ĵ���
	remove_GD_UpdateSubCodeType,		//�Ƴ����ж��Ĵ���
	replace_GD_UpdateSubCodeType		//�滻ȫ�����Ĵ���
};

enum GD_ServerType		//��������������
{
	realTime_GD_ServerType = 0,			//ʵʱ���������
	simulation_GD_ServerType,			//ģ�����������
	history_GD_ServerType				//��ʷ���ݷ�����
};

struct GD_Server		//��������Ϣ����
{
	char szName[32];
	char szIp[32];
	int nPort;
	GD_ServerType nType;		//����������
	char szVersion[32];			//�������api�汾
};

struct GD_User 
{
	char szName[32];
	char szPass[32];
	int nMaxLoginNum;
	char szRealTimeServer[32];		//ʵʱ��������� ����
	char szSimulationServer[32];	//ģ����������� ����
	char szHistoryServer[32];		//��ʷ��������� ����
};

//////////////////////////////////////////////////////////////////////////
//����
struct GD_Login_Req			//��¼����
{
	char szName[32];		//�û���
	char szPass[32];		//����
	char szSystem[32];		//apiϵͳ��Ϣ
	char szVersion[32];		//api�汾
};

struct GD_UserInfo_Req		//�����û���Ϣ
{
	char szName[32];		//�û���
	char szVersion[32];		//Api �汾
	char szSystem[32];		//ϵͳ��Ϣ
};

struct GD_RealTime_Req					//ʵ����������
{
	bool	isAllMarket;				//�Ƿ���ȫ�г�
	int		nCodeNum;					//��������
	GD_CodeType szCodes[1];				//���Ĵ����׵�ַ
};

struct GD_Simulation_Req				//ģ����������
{
	GD_ISODateTimeType szBeginDatetime;	//����ʼ��ʱ��
	GD_ISODateTimeType szEndDatetime;	//���������ʱ��
	bool	isAllMarket;				//�Ƿ���ȫ�г�
	int		nCodeNum;					//��������
	GD_CodeType szCodes[1];				//���Ĵ����׵�ַ
};

struct GD_UpdateSubCodes_Req
{
	GD_UpdateSubCodeType nType;			//��������
	bool	isAllMarket;				//�Ƿ���ȫ�г�
	int		nCodeNum;					//��������
	GD_CodeType szCodes[1];				//���Ĵ����׵�ַ
};

struct GD_Kline_Req						//ģ����������
{
	GD_CycType nType;					//��������
	GD_ISODateTimeType szBeginDatetime;	//����ʼ��ʱ��
	GD_ISODateTimeType szEndDatetime;	//���������ʱ��
	bool	isAllMarket;				//�Ƿ���ȫ�г�
	int		nCodeNum;					//��������
	GD_CodeType szCodes[1];				//���Ĵ����׵�ַ
};

//////////////////////////////////////////////////////////////////////////
//�ص�
struct GD_Login_Rsp			//��¼�ص�
{
	GD_ErrorType nErrCode;				//������
	char szErr[32];						//��������
	GD_Server realTimeServer;
	GD_Server simulationServer;
	GD_Server historyServer;
	int nCodesNum;
	int nOptionInfosNum;
	TDF_CODE* code;						//�����
	TDF_OPTION_CODE* optionInfo;		//��Ȩ�����
};

typedef GD_ISODateTimeType GD_TradingDateBegin_Rsp;		//�����տ�ʼ
typedef GD_ISODateTimeType GD_TradingDateEnd_Rsp;		//�����ս���

struct GD_Kline_Data						//ģ����������
{
	GD_CycType nType;					//��������
	char	szCode[32];					//�Ƿ���ȫ�г�
	GD_ISODateTimeType szDatetime;		//ʱ��
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
