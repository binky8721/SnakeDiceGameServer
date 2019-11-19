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
		cout << "1: ȸ������	 2: �α���		" << endl;
		cout << "���ڸ� �Է����ּ��� : ";

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
			"�ȳ��ϼ���, �α����� �Ͻðڽ��ϱ� �ƴϸ� ȸ�������� �Ͻðڽ��ϱ�?" "\n"
			"[1] ȸ������" "\n"
			"[2] �α���" "\n"
			"[3] ����");

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
	cout << "** ���̵� �Է����ּ��� :";
	if (fgets(temp, 20, stdin) == NULL)
		return;

	temp[strlen(temp) - 1] = '\0';
	myInfo.UserID = temp;
	ZeroMemory(temp, sizeof(temp));
	cout << "** ��й�ȣ�� �Է����ּ��� :";
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
		myInfo.UserID = InputRegisterUserID(); //ID �Է�
		myInfo.UserPW = InputRegisterUserPW(); //PW �Է�

		RegisterPacket registerPacket;
		registerPacket.header = Register;
		registerPacket.id = myInfo.UserID;
		registerPacket.pw = myInfo.UserPW;
		registerPacket.totalSize = sizeof(RegisterPacket);
		ZeroMemory(buf, BUFSIZE);
		memcpy(buf, &registerPacket, sizeof(RegisterPacket));
		if (!communicator.Send(buf, sizeof(RegisterPacket)))
		{
			// ���� ���
			cout << "Send failed" << endl;
			return false;
		}

		break;
	}

	case LoginType:
	{
		myInfo.UserID = InputLogInUserID(); //ID �Է�
		myInfo.UserPW = InputLogInUserPW(); //PW �Է�

		LoginPacket loginPacket;
		loginPacket.header = Login;
		loginPacket.id = myInfo.UserID;
		loginPacket.pw = myInfo.UserPW;
		loginPacket.totalSize = sizeof(LoginPacket);
		ZeroMemory(buf, BUFSIZE);
		memcpy(buf, &loginPacket, sizeof(LoginPacket));
		if (!communicator.Send(buf, sizeof(LoginPacket)))
		{
			// ���� ���
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
	std::string username = InputInfo <std::string>("ID�� �Է��ϼ���.");
	std::cout << "ID: \"" << username << "\"\n";
	while (InputInfo <int>("�½��ϱ�? [�ƴϿ�:0 // ��:1]") != 1) {
		username = InputInfo <std::string>("ID�� �Է��ϼ���.");
		std::cout << "ID: \"" << username << "\"\n";
	}
	return username;
}

std::string LoginManager::InputLogInUserID()
{
	std::string username = InputInfo <std::string>("ID�� �Է��ϼ���.");
	std::cout << "ID: \"" << username << "\"\n";
	return username;
}


std::string LoginManager::InputRegisterUserPW()
{
	std::string password1 = InputInfo <std::string>("PW�� �Է��ϼ���.");
	std::string password2 = InputInfo <std::string>("PW�� �ѹ� �� �Է��ϼ���.");

	while (password1 != password2) {
		std::cout << "PW�� ��ġ���� �ʽ��ϴ�!" "\n";
		password1 = InputInfo <std::string>("PW�� �Է��ϼ���.");
		password2 = InputInfo <std::string>("PW�� �ѹ� �� �Է��ϼ���.");
	}

	return password1;
}

std::string LoginManager::InputLogInUserPW()
{
	std::string password = InputInfo <std::string>("PW�� �Է��ϼ���.");
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