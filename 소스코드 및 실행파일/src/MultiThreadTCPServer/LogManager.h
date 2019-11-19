#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<stdio.h>
#include<time.h>

// 로그 기록 관련 클래스
class LogManager
{
	LogManager();
	static LogManager* instance;

	std::string logFilename;
public:
	static LogManager* GetInstance();
	~LogManager();

	void UserAccessLog(const std::string userID); //유저접속시 로그(아이디,년,월,시,분,초)저장
	void UserDisconnectLog(const std::string userID); //접속 해제시 로그
	const std::string currentDateTime(); //현재 접속시간 출력 함수
};

