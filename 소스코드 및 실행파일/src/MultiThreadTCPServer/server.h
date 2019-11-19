#pragma once
#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include "Packet.h"
#include "GameManager.h"
#include "LoginManager.h"
#include "LogManager.h"

#define SERVERPORT 9000
#define BUFSIZE    512

extern  CRITICAL_SECTION cs;

class Server
{
	WSADATA wsa;
	SOCKET listen_sock;
	SOCKADDR_IN serveraddr;

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;
	char buf[BUFSIZE+1];
	static Server* instance;
	Server();

	static DWORD WINAPI ProcessClient(LPVOID arg);
	static DWORD WINAPI ProcessGame(LPVOID arg);

	void err_quit(const char *msg);
	void err_display(const char *msg);
	
	LogManager *logManager;
public:
		
	static Server* GetInstace();
	~Server();
	bool Init();
	bool Bind();
	bool Listen();
	bool Accept();
	void Release();

	bool Send(SOCKET sock, char * data, int len);
	int Recvn(SOCKET sock, PacketType* packet, int len);	// 받을 패킷타입이 정해져있을경우
	int Recvn(SOCKET sock, char* data, int len);	// 받을 패킷이 불분명하여 헤더로 판별해야할 경우
	SOCKET GetClientSock() const;

	//LogManager& GetLogManager() { return *logManager; }
};

