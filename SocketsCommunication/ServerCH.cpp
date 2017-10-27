// KR2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<Winsock2.h>
#include<windows.h>
#include<iostream>
using namespace std;
#define PORT_ADDR 80
#define QUEUE_SIZE 5
SOCKET clntsocket[5];
HANDLE hrEvent=OpenEvent(EVENT_ALL_ACCESS, FALSE, NULL);
int clientnum=0;
DWORD WINAPI ThreadFunc(LPVOID param)
{
	int* m=(int*)param;
	int q=*m;
	char buf[256];
	char clientwrote[50];
	char youare[50];
	wsprintf(youare, "You are client #%d \n", q+1);
	int err = send(clntsocket[q], youare, sizeof(youare), 0);
	wsprintf(clientwrote,"Client %d wrote:", q);
	ZeroMemory(buf,256);
	while(TRUE)
	{
		WaitForSingleObject(hrEvent,INFINITE);
		ResetEvent(hrEvent);
		int Errors=recv(clntsocket[q], buf,  256, 0);
		if(Errors==SOCKET_ERROR)
		{
			cout<<WSAGetLastError()<<endl;
			break;
		}
		for(int i=0; i<=clientnum-1;i++)
			if (i!=q)
			{
				Errors=send(clntsocket[i], clientwrote, sizeof(clientwrote), 0);
				if(Errors==SOCKET_ERROR)
				{
					cout<<WSAGetLastError()<<endl;
					break;
				}
				Errors=send(clntsocket[i], buf, sizeof(buf), 0);
				if(Errors==SOCKET_ERROR)
				{
					cout<<WSAGetLastError()<<endl;
					break;
				}
			}
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
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	__try
	{
		char chInfo[64];
		if (gethostname(chInfo,sizeof(chInfo)) )
			printf("Not local host\n");
		else
			printf("Host Name: %s\n",chInfo);
		SOCKET srvsocket;
		srvsocket = socket(AF_INET,	SOCK_STREAM,IPPROTO_TCP);
		if (srvsocket==INVALID_SOCKET)
		{
			printf("Error create socket\n");
			return 1;
		}
		SOCKADDR_IN socketaddr;
		socketaddr.sin_family = AF_INET;
		socketaddr.sin_addr.s_addr = INADDR_ANY;
		socketaddr.sin_port = htons(PORT_ADDR);
		if( bind(srvsocket,(LPSOCKADDR)&socketaddr,sizeof(socketaddr))	==  SOCKET_ERROR )
		{
			printf("Bind socket error %d\n", WSAGetLastError());
			return 1;
		}
		else
			printf("Socket binding success\n");
		int Errors;
		Errors=listen(srvsocket, QUEUE_SIZE);
		if ( Errors == SOCKET_ERROR)
		{
			printf("Listen error %d\n", WSAGetLastError());
			return 1;
		}
		else printf("Listen success\n");
		printf("Wait for connections...\n");
		DWORD ThId[5];//max 5 clients
		while(TRUE)//  
		{
			SOCKET clientsocket;
			SOCKADDR_IN socketclientaddr;
			int CALen = sizeof(socketclientaddr);
			ZeroMemory(&socketclientaddr, CALen);
			clientsocket = accept(srvsocket,(struct sockaddr*)&socketclientaddr, &CALen);
			if ( clientsocket != SOCKET_ERROR) //if acception is successful
			{
				clntsocket[clientnum]=clientsocket;
				int t=clientnum;
				ThId[t]=t;
				CreateThread(NULL, 0, &ThreadFunc, &t, 0, &ThId[t]);
				clientnum++;
			}
		}
		getchar();
		closesocket(srvsocket);
	}
	__finally{
		// Stop Winsock
		WSACleanup();
	}
	return 0;
}


