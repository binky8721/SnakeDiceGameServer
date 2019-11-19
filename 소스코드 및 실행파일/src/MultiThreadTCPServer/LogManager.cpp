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
	std::ofstream outfile(logFilename, std::ios_base::out | std::ios_base::app); //쓰기모드,맨끝부터 데이터추가

	if (outfile.is_open())
	{
		//Todo : 아이디 로그에 저장
		outfile << userID << " connected#";
		outfile << currentDateTime(); //접속한 시간 로그에 저장
	}
	else
		std::cout << "UserAccessLog.txt 파일 open 실패" << std::endl;

	outfile.close(); //파일닫기
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
		std::cout << "UserAccessLog.txt 파일 open 실패" << std::endl;
	outfile.close();
}

const std::string LogManager::currentDateTime()
{
	time_t     now = time(0); //현재 시간을 time_t 타입으로 저장
	struct tm  tstruct;
	char       buf[80];
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "LogTime : %Y-%m-%d/%X\n", &tstruct); // YYYY-MM-DD.HH:mm:ss 형태의 스트링

	return buf;
}