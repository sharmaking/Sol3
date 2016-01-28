#include <winsock.h>  
#include <iostream> 
#pragma comment(lib, "ws2_32.lib")

using namespace std;

class SocketObject  
{  
public:  
	SOCKET skSocket;  
	SocketObject();     // 初始化socket，申请套件口
	~SocketObject();  
	bool Bind(char *szServerIP, int iPort); //  将套接口绑定到地址端口，形成套接字
	void Disconnect();                      //  释放套接字
	int Listen(void);                      //  监听（服务端使用）
	bool Accept(SocketObject &skAcceptSocket);  //  等待客户端连接（服务端使用）
	bool Connect(char* szServerAddress,int iPort);  //  连接（客户端使用）
	int Send(char *szBuffer,int iBufLen, int iFlags);  // 发送数据
	int Recv(char *szBuffer,int iBufLen,int iFlags);   // 接受数据（未收到数据则一直阻塞）
protected:  
private:  
};  