#pragma once
#include <iostream>
#define NOMINMAX //c2589에러 방지
#include <limits>
#include <conio.h>
#include "ClientCommunicator.h"
#include <Windows.h>
#include "Packet.h"
using namespace std;

struct UserInfo
{
	string UserID;
	string UserPW;
	string UserAvatar;
};

enum InputType
{
	None,
	RegisterType,
	LoginType,
};

class LoginManager
{
private:
	UserInfo myInfo;
	InputType inputType;
	char buf[BUFSIZE];
	LoginManager();
	static LoginManager* instance;
public:
	static LoginManager* GetInstance();
	~LoginManager();
	void SelectInputType();
	void InputLoginInfo();
	bool StartLoginProcess(ClientCommunicator& communicator);
	InputType GetInputType() const;
	UserInfo GetUserInfo() const;


	std::string InputRegisterUserID();
	std::string InputLogInUserID();
	std::string InputRegisterUserPW();
	std::string InputLogInUserPW();
	template <typename T>
	T InputInfo(const std::string &strQuery);
};