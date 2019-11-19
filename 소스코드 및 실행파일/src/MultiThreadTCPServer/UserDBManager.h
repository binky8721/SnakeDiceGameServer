#pragma once
#include "LoginManager.h"
#include<vector>

/// 유저 데이터베이스 관리 클래스
class UserDBManager
{
	UserDBManager();
	static UserDBManager* instance;
	string userInfoFileName;	// 유저 계정정보 파일
	string GameLogFileName;	// 게임로그 파일

public:
	static UserDBManager* GetInstance();
	~UserDBManager();
	int SearchClientInfo(const string userID, const string userPW);	// 이진 탐색으로 유저 정보를 찾기
	void RegisterUserInfo(ClientInfo userInfo);
	void BinarySort();

	int IsCanRegisterInfo(const string userID, const string userPW);	// 등록이 가능한 유저인지 찾기 //1.성공 2.아이디중복 3.아이디가너무김 4.비밀번호가 너무김
	int IsCanLoginInfo(const string userID, const string userPW);	// 로그인 가능 여부판단. 1.아이디틀림 2.패스워드틀림 3.로그인성공(반환값이 3이지만 딱히 사용되지는 않음)

	vector<int> getPi(string p);
	vector<int> kmp(string s, string p);
};

