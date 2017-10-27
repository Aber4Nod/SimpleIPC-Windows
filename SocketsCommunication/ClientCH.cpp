// client_.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<Winsock2.h>
#include<iostream>
#include<windows.h>
#include<string>
#include<conio.h>
using namespace std;
SOCKET srvsocket;
HANDLE hwEvent=CreateEvent(NULL, FALSE, FALSE, NULL);
DWORD WINAPI readfunc(LPVOID param)
{
	char buf[256];
	while(TRUE)
	{
		int Errors=recv(srvsocket, buf,  256, 0);
		if(Errors==SOCKET_ERROR)
		{
			printf("Connection with this client is broken/n");
			break;
		}
		cout<<buf<<endl;
	}
	CloseHandle(GetCurrentThread());
	return 0;
}
DWORD WINAPI writefunc(LPVOID param)
{
	char buf[256];
	while(TRUE)
	{
		cin.getline(buf,256);
		int Err=send(srvsocket,buf,sizeof(buf),0);
		if (Err==SOCKET_ERROR)
		{
			cout<<WSAGetLastError();
			break;
		}
		SetEvent(hwEvent);
	}
	CloseHandle(GetCurrentThread());
	return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);//век24
		return 1;
	}
	__try
	{
		
		char chInfo[64];
		if (gethostname(chInfo,sizeof(chInfo)) )
			printf("Not local host\n");
		else
			printf("Host Name: %s\n",chInfo);
		
		int clientnum=0;
		srvsocket = socket(AF_INET,	SOCK_STREAM,IPPROTO_TCP);
		if (srvsocket==INVALID_SOCKET)
		{
			printf("Error create socket\n");
			return 1;
		}
		SOCKADDR_IN socketaddr;
		socketaddr.sin_family = AF_INET;
		
			if((argv[1])&&(argv[2])){
		socketaddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
		int k = atoi(argv[2]);
		socketaddr.sin_port = htons(k);
		}
			else 
				{
					printf("Check your coomand line arguments");
					getch();
					return 0;
			}
			
		connect(srvsocket,(struct sockaddr*)&socketaddr,sizeof(struct sockaddr));

		CreateThread(NULL, 0, &readfunc, NULL, 0, NULL);
		CreateThread(NULL, 0, &writefunc, NULL, 0, NULL);
		while(TRUE)
		{}		
		getchar();
		
	}
	__finally{
		WSACleanup();
		CloseHandle(hwEvent);
		closesocket(srvsocket);
	}
	return 0;
}




