#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<stdio.h>
#include<time.h>

// �α� ��� ���� Ŭ����
class LogManager
{
	LogManager();
	static LogManager* instance;

	std::string logFilename;
public:
	static LogManager* GetInstance();
	~LogManager();

	void UserAccessLog(const std::string userID); //�������ӽ� �α�(���̵�,��,��,��,��,��)����
	void UserDisconnectLog(const std::string userID); //���� ������ �α�
	const std::string currentDateTime(); //���� ���ӽð� ��� �Լ�
};

