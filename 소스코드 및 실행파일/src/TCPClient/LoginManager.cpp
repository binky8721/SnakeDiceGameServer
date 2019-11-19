#include "LoginManager.h"

LoginManager* LoginManager::instance = nullptr;
LoginManager::LoginManager()
{
	inputType = None;
	myInfo.UserAvatar = "";
	myInfo.UserID = "";
	myInfo.UserPW = "";
}

LoginManager* LoginManager::GetInstance()
{
	if (instance == nullptr)
		instance = new LoginManager();

	return instance;
}

LoginManager::~LoginManager()
{
}

void LoginManager::SelectInputType()
{
	//while (true)
	//{
		/*
		cout << "1: 회원가입	 2: 로그인		" << endl;
		cout << "숫자를 입력해주세요 : ";

		int input = _getch();
		//int input = fgetc(stdin);
		if (input == '1')
		{
			
			break;
		}
		else if (input == '2')
		{
			inputType = LoginType;
			break;

		}
		*/
		int choice = InputInfo <int>(
			"안녕하세요, 로그인을 하시겠습니까 아니면 회원가입을 하시겠습니까?" "\n"
			"[1] 회원가입" "\n"
			"[2] 로그인" "\n"
			"[3] 종료");

		switch (choice)
		{
		case 1:
			inputType = RegisterType;
			break;
		case 2:
			inputType = LoginType;
			break;
		case 3:
			exit(1);
			break;
		}
	//}
}

void LoginManager::InputLoginInfo()
{
	char temp[20];
	cout << "** 아이디를 입력해주세요 :";
	if (fgets(temp, 20, stdin) == NULL)
		return;

	temp[strlen(temp) - 1] = '\0';
	myInfo.UserID = temp;
	ZeroMemory(temp, sizeof(temp));
	cout << "** 비밀번호를 입력해주세요 :";
	if (fgets(temp, 10, stdin) == NULL)
		return;

	temp[strlen(temp) - 1] = '\0';
	myInfo.UserPW = temp;
}

bool LoginManager::StartLoginProcess(ClientCommunicator& communicator)
{
	switch (inputType)
	{
	case RegisterType:
	{
		myInfo.UserID = InputRegisterUserID(); //ID 입력
		myInfo.UserPW = InputRegisterUserPW(); //PW 입력

		RegisterPacket registerPacket;
		registerPacket.header = Register;
		registerPacket.id = myInfo.UserID;
		registerPacket.pw = myInfo.UserPW;
		registerPacket.totalSize = sizeof(RegisterPacket);
		ZeroMemory(buf, BUFSIZE);
		memcpy(buf, &registerPacket, sizeof(RegisterPacket));
		if (!communicator.Send(buf, sizeof(RegisterPacket)))
		{
			// 에러 출력
			cout << "Send failed" << endl;
			return false;
		}

		break;
	}

	case LoginType:
	{
		myInfo.UserID = InputLogInUserID(); //ID 입력
		myInfo.UserPW = InputLogInUserPW(); //PW 입력

		LoginPacket loginPacket;
		loginPacket.header = Login;
		loginPacket.id = myInfo.UserID;
		loginPacket.pw = myInfo.UserPW;
		loginPacket.totalSize = sizeof(LoginPacket);
		ZeroMemory(buf, BUFSIZE);
		memcpy(buf, &loginPacket, sizeof(LoginPacket));
		if (!communicator.Send(buf, sizeof(LoginPacket)))
		{
			// 에러 출력
			cout << "Send failed" << endl;
			return false;
		}
		break;
	}

	default:
		return false;
	}

	return true;
}

InputType LoginManager::GetInputType() const
{
	return inputType;
}

UserInfo LoginManager::GetUserInfo() const
{
	return myInfo;
}

std::string LoginManager::InputRegisterUserID()
{
	std::string username = InputInfo <std::string>("ID를 입력하세요.");
	std::cout << "ID: \"" << username << "\"\n";
	while (InputInfo <int>("맞습니까? [아니오:0 // 네:1]") != 1) {
		username = InputInfo <std::string>("ID를 입력하세요.");
		std::cout << "ID: \"" << username << "\"\n";
	}
	return username;
}

std::string LoginManager::InputLogInUserID()
{
	std::string username = InputInfo <std::string>("ID를 입력하세요.");
	std::cout << "ID: \"" << username << "\"\n";
	return username;
}


std::string LoginManager::InputRegisterUserPW()
{
	std::string password1 = InputInfo <std::string>("PW를 입력하세요.");
	std::string password2 = InputInfo <std::string>("PW를 한번 더 입력하세요.");

	while (password1 != password2) {
		std::cout << "PW가 일치하지 않습니다!" "\n";
		password1 = InputInfo <std::string>("PW를 입력하세요.");
		password2 = InputInfo <std::string>("PW를 한번 더 입력하세요.");
	}

	return password1;
}

std::string LoginManager::InputLogInUserPW()
{
	std::string password = InputInfo <std::string>("PW를 입력하세요.");
	return password;
}

template <typename T>
T LoginManager::InputInfo(const std::string &strQuery)
{
	std::cout << strQuery << "\n> ";
	T out = T();

	while (!(std::cin >> out)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
		std::cout << "Error!" "\n";
		std::cout << strQuery << "\n> ";
	}

	return out;
}