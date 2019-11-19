#pragma once
#include <iostream>
#include <conio.h>
#include "Server.h"
#include <Windows.h>
#include "Packet.h"
#include "ClientInfo.h"
#include "UserDBManager.h"

using namespace std;
enum E_ExceptionType
{
	AleadyExistUser,
	TooLongUserID,
	TooLongUserPW,
	WrongPacketType,
	SocketError,
	WrongUserId,
	WrongPassword,
};

class LoginManager
{
private:
	LoginPacket loginPacket;
	RegisterPacket registerPacket;
	LoginManager();
	static LoginManager* instance;
	std::string userInfoFileName;
public:
	static LoginManager* GetInstance();
	~LoginManager();
	
	bool StartLoginProcess(SOCKET clientSock, ClientInfo& thisClient);	// 로그인 처리

	void RegisterInfoSave(const RegisterPacket &rp);
	int GetKeyID();
};