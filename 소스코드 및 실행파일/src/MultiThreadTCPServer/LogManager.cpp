#include "LogManager.h"


LogManager* LogManager::instance = nullptr;
LogManager::LogManager()
{
	logFilename = "UserAccessLog.txt";
}

LogManager * LogManager::GetInstance()
{
	if (instance == nullptr)
		instance = new LogManager();

	return instance;
}

LogManager::~LogManager()
{
}


void LogManager::UserAccessLog(const std::string userID)
{
	std::ofstream outfile(logFilename, std::ios_base::out | std::ios_base::app); //������,�ǳ����� �������߰�

	if (outfile.is_open())
	{
		//Todo : ���̵� �α׿� ����
		outfile << userID << " connected#";
		outfile << currentDateTime(); //������ �ð� �α׿� ����
	}
	else
		std::cout << "UserAccessLog.txt ���� open ����" << std::endl;

	outfile.close(); //���ϴݱ�
}

void LogManager::UserDisconnectLog(const std::string userID)
{
	std::ofstream outfile(logFilename, std::ios_base::out | std::ios_base::app);
	if (outfile.is_open())
	{
		outfile << userID << " disconnected#";
		outfile << currentDateTime();
	}
	else
		std::cout << "UserAccessLog.txt ���� open ����" << std::endl;
	outfile.close();
}

const std::string LogManager::currentDateTime()
{
	time_t     now = time(0); //���� �ð��� time_t Ÿ������ ����
	struct tm  tstruct;
	char       buf[80];
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "LogTime : %Y-%m-%d/%X\n", &tstruct); // YYYY-MM-DD.HH:mm:ss ������ ��Ʈ��

	return buf;
}