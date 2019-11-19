#pragma once
#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include "Packet.h"

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512

struct Share {
	char ip[BUFSIZE];
	int share = 0;
};

//struct gameAvatar {		// ������ ���� �ִ� �̸�Ƽ��
//	string rabbit = "U('x')U";
//	string pig = "/('(00)')/";
//	string bird = "/('<>')|";
//	string fish = "<' )))><<";
//}; 



class ClientCommunicator
{
private:

	Share share;	// todo �ʿ�����Ƿ� �����ؾߵ�
	WSADATA wsa;
	SOCKET sock;
	SOCKADDR_IN serveraddr;

	//������ ��ſ� �ʿ��� ����
	in_addr ip;
	char ip_buf[BUFSIZE + 1];
	char buf[BUFSIZE + 1];
	static ClientCommunicator* instance;

	//E_GameState gameState;

	ClientCommunicator();
	void err_quit(const char* msg);
	void err_display(const char* msg);
public:
	static ClientCommunicator* GetInstance();
	~ClientCommunicator();

	bool Init();
	bool Connect();
	bool Send(char* data, int len);
	bool Recv();
	int Recvn( char*buf, int len);
	//int Recvn(PacketType* packet, int len);
	const Share GetShare();
	void AddShareValue(int data);
	char* GetIPBuf();

	void Release();
};

