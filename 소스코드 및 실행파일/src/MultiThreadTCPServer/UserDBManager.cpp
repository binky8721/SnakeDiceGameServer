#include "UserDBManager.h"


UserDBManager* UserDBManager::instance = nullptr;
UserDBManager::UserDBManager()
{
	std::string filename = "UserInfo.txt";
	std::ofstream outfile(filename, std::ios_base::out | std::ios_base::app);
}

UserDBManager * UserDBManager::GetInstance()
{
	if (instance == nullptr)
		instance = new UserDBManager();

	return instance;
}

UserDBManager::~UserDBManager()
{
}

int UserDBManager::SearchClientInfo(const string userID, const string userPW)
{
	//ID길이 제한(12자 미만)
	if (userID.length() > 12)
	{
		std::cout << "register user ID so long" << std::endl;
		return 3;
	}
	//PW길이 제한(12자 미만)
	if (userPW.length() > 12)
	{
		std::cout << "register user PW so long" << std::endl;
		return 4;
	}

	std::ifstream inFile;
	std::string line;

	inFile.open("UserInfo.txt");

	if (!inFile) {
		std::cout << "Unable to open UserInfo.txt file" << std::endl;
		exit(1);
	}
	//size_t pos_ID; //단어검색 반환값을 받기위한 변수
	while (inFile.good())
	{
		getline(inFile, line); // 파일에서 문자열을 줄단위로 읽는다
		auto matchedID = kmp(line, userID); //ID검사

		if ((int)matchedID.size() >= 1)//일단 일치하는 패턴이 존재하면
		{
			//그 후 원래 아이디와 입력된 아이디의 문자열 길이 비교
			std::string realID = line.substr(0, line.find('#', 1)); //원래아이디길이 각라인에서 처음 #이전까지
			if (userID.length() == realID.length()) //아이디 틀림 (매치하는 아이디 패턴은 찾았지만 길이가 달라 아이디가 아닌경우) //이 경우 회원가입 가능
				return 2; //회원가입 아이디 중복이 있음 true
		}
		/*
		pos_ID = line.find(userID); // 라인에서 문자열 검색
	    if (pos_ID != std::string::npos) // 해당라인에서 찾는 문자열이 존재할시
		{
			if (inFile.is_open())
				inFile.close();
			//std::cout << "ID 중복있음 회원가입 불가" << std::endl;
			return 2; //회원가입 아이디 중복이 있음 true
		}
		*/
	}
	if (inFile.is_open())
		inFile.close();
	//std::cout << "ID 중복없음 회원가입 가능" << std::endl;
	return 1;
}

void UserDBManager::RegisterUserInfo(ClientInfo userInfo)
{
}

void UserDBManager::BinarySort()
{
}

int UserDBManager::IsCanRegisterInfo(const string userID, const string userPW)
{
	//if (SearchClientInfo(userID,userPW))	// db에 유저정보가 존재하는 경우
	//	return false;
	
	// todo 유저아이디가 너무 길진 않은지
	
	return SearchClientInfo(userID, userPW);

	//return true;	//todo 추후 작업 완료되면 수정
}

int UserDBManager::IsCanLoginInfo(const string userID, const string userPW)
{
	std::ifstream inFile;
	std::string line;

	inFile.open("UserInfo.txt");

	if (!inFile) {
		std::cout << "Unable to open file" << std::endl;
		exit(1);
	}
	while (inFile.good())
	{
		getline(inFile, line);
		auto matchedID = kmp(line, userID); //ID검사
		if ((int)matchedID.size() >= 1)//일단 일치하는 패턴이 존재하면
		{
			//그 후 원래 아이디와 입력된 아이디의 문자열 길이 비교
			std::string realID = line.substr(0, line.find('#', 1)); //원래아이디길이 각라인에서 처음 #이전까지
			if (userID.length() != realID.length()) //아이디 틀림 (매치하는 아이디 패턴은 찾았지만 길이가 달라 아이디가 아닌경우) //이 경우는 다시 반복해서 검사한다.
				continue;

			//PW 검사 (해당라인의 id 뒷부분에서 검사)
			auto matchedPW = kmp(line.substr(userID.length() + 1, std::string::npos), userPW);
			std::cout << "pw길이 일치수:" << (int)matchedPW.size() << "\n";
			if ((int)matchedPW.size() >=1)
			{
				std::string real_PW = line.substr(userID.length() + 1, userPW.length());
				if (real_PW == userPW) //자른정보(실제비밀번호)와 입력받은 비밀번호가 일치하는지 비교
				{
					std::cout << "PW 맞음!\n";
					return 3; //로그인 성공
				}
				else
				{
					std::cout << "PW 틀림! case1\n";
					return 2; //패스워드 틀림 (패스워드 패턴은 일치하지만 길이가 달라 틀린경우)
				}
			}
			else
			{
				std::cout << "PW 틀림! case2\n";
				return 2; //패스워드 틀림 (패스워드가 아예 틀린경우)
			}
		}
	}
	std::cout << "ID 틀림 case2\n";
	return 1; //아이디 틀림 (일치하는 아이디를 못찾은 경우)
}

vector<int> UserDBManager::getPi(string p)
{
	int m = (int)p.size(), j = 0;
	vector<int> pi(m, 0);
	for (int i = 1; i < m; i++)
	{
		while (j > 0 && p[i] != p[j]) 
			j = pi[j - 1]; 
		if (p[i] == p[j]) pi[i] = ++j;
	} 
	return pi;

}

vector<int> UserDBManager::kmp(string s, string p)
{
	vector<int> ans;
	auto pi = getPi(p); 
	int n = (int)s.size(), m = (int)p.size(), j = 0;
	for (int i = 0; i < n; i++)
	{ 
		while (j > 0 && s[i] != p[j]) 
			j = pi[j - 1];
		if (s[i] == p[j])
		{ 
			if (j == m - 1) 
			{ 
				ans.push_back(i - m + 1);
				j = pi[j]; 
			}
			else 
			{ j++; }
		}
	}
	return ans;

}
