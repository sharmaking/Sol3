#include <winsock.h>  
#include <iostream> 
#pragma comment(lib, "ws2_32.lib")

using namespace std;

class SocketObject  
{  
public:  
	SOCKET skSocket;  
	SocketObject();     // ��ʼ��socket�������׼���
	~SocketObject();  
	bool Bind(char *szServerIP, int iPort); //  ���׽ӿڰ󶨵���ַ�˿ڣ��γ��׽���
	void Disconnect();                      //  �ͷ��׽���
	int Listen(void);                      //  �����������ʹ�ã�
	bool Accept(SocketObject &skAcceptSocket);  //  �ȴ��ͻ������ӣ������ʹ�ã�
	bool Connect(char* szServerAddress,int iPort);  //  ���ӣ��ͻ���ʹ�ã�
	int Send(char *szBuffer,int iBufLen, int iFlags);  // ��������
	int Recv(char *szBuffer,int iBufLen,int iFlags);   // �������ݣ�δ�յ�������һֱ������
protected:  
private:  
};  