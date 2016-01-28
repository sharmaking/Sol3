#include "SocketObject.h"  


SocketObject::SocketObject(){  
	WSADATA wsaData;  
	WORD wVersionRequested;  

	wVersionRequested = MAKEWORD(2,0);  
	skSocket = INVALID_SOCKET;  
	int iStatus = WSAStartup(wVersionRequested,&wsaData); //0代表成功,非0:错误编号  
//	printf("%d\n",iStatus);  
	skSocket = socket(AF_INET,SOCK_STREAM,0);  
}  

SocketObject::~SocketObject(){  
	Disconnect();  
}  

void SocketObject::Disconnect(){  
	if(skSocket!= INVALID_SOCKET){  
		closesocket(skSocket); 
		skSocket = INVALID_SOCKET;  
	}  
}  

bool SocketObject::Bind(char *szServerIP, int iPort){  
	sockaddr_in saServerAddress;  
	if (skSocket == INVALID_SOCKET)  
	{
		return false;  
		cout<<"**ERROR** Could Not Create Socket"<<endl;  
	}  
	memset(&saServerAddress,0,sizeof(sockaddr_in));  
	saServerAddress.sin_family = AF_INET;  
	saServerAddress.sin_addr.s_addr = inet_addr(szServerIP);  
	saServerAddress.sin_port = htons(iPort);  
	if (bind(skSocket,(sockaddr*)&saServerAddress,sizeof(sockaddr))==SOCKET_ERROR)  
	{  
		Disconnect();  
		cout<<"bind failed!"<<endl;  
		return false;  
	}  
	else  
	{
		cout << "bind ip:" << szServerIP << "port:" << endl;
		return true;
	}
}  

int SocketObject::Listen(void){  
	return listen(skSocket,32);  
}  

bool SocketObject::Accept(SocketObject &skAcceptSocket)
{ 
	sockaddr_in saClientAddress;  
	int iClientSize = sizeof(sockaddr_in); 
	memset(&saClientAddress,0,sizeof(sockaddr_in)); 
	skAcceptSocket.skSocket = accept(skSocket,(struct sockaddr*)&saClientAddress,&iClientSize);  
	if(skAcceptSocket.skSocket == INVALID_SOCKET){  
		return false;  
	}
	else
	{  
		return true;  
	}     
} 

bool SocketObject::Connect(char* szServerAddress,int iPort){  
	struct sockaddr_in serv_addr;  
	LPHOSTENT lpHost;  
	int err;  
	skSocket = socket(AF_INET,SOCK_STREAM,0);  
	if (skSocket == INVALID_SOCKET)  
	{  
		return false;  
	}  
	memset(&serv_addr,0,sizeof(sockaddr_in));  
	serv_addr.sin_family = AF_INET;  
	serv_addr.sin_addr.s_addr = inet_addr(szServerAddress);  
	if(serv_addr.sin_addr.s_addr == INADDR_NONE){  
		lpHost = gethostbyname(szServerAddress);  
		if (lpHost!=NULL)  
		{  
			serv_addr.sin_addr.s_addr = ((LPIN_ADDR)lpHost->h_addr)->s_addr;  
		}else{  
			return false;  
		}  
	}  
	serv_addr.sin_port = htons(iPort);  
	err = connect(skSocket,(struct sockaddr*)&serv_addr,sizeof(sockaddr));  
	if(err == SOCKET_ERROR){  
		Disconnect();  
		return false;  
	}  
	return true;  
}  

int SocketObject::Send(char *szBuffer,int iBufLen, int iFlags){  
	return send(skSocket,szBuffer,iBufLen,iFlags);  
}  

int SocketObject::Recv(char *szBuffer,int iBufLen,int iFlags){  
	return recv(skSocket,szBuffer,iBufLen,iFlags);  
}  