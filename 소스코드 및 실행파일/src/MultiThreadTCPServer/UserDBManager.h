#pragma once
#include "LoginManager.h"
#include<vector>

/// ���� �����ͺ��̽� ���� Ŭ����
class UserDBManager
{
	UserDBManager();
	static UserDBManager* instance;
	string userInfoFileName;	// ���� �������� ����
	string GameLogFileName;	// ���ӷα� ����

public:
	static UserDBManager* GetInstance();
	~UserDBManager();
	int SearchClientInfo(const string userID, const string userPW);	// ���� Ž������ ���� ������ ã��
	void RegisterUserInfo(ClientInfo userInfo);
	void BinarySort();

	int IsCanRegisterInfo(const string userID, const string userPW);	// ����� ������ �������� ã�� //1.���� 2.���̵��ߺ� 3.���̵𰡳ʹ��� 4.��й�ȣ�� �ʹ���
	int IsCanLoginInfo(const string userID, const string userPW);	// �α��� ���� �����Ǵ�. 1.���̵�Ʋ�� 2.�н�����Ʋ�� 3.�α��μ���(��ȯ���� 3������ ���� �������� ����)

	vector<int> getPi(string p);
	vector<int> kmp(string s, string p);
};

