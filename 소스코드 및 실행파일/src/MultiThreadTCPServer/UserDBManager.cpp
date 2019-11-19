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
	//ID���� ����(12�� �̸�)
	if (userID.length() > 12)
	{
		std::cout << "register user ID so long" << std::endl;
		return 3;
	}
	//PW���� ����(12�� �̸�)
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
	//size_t pos_ID; //�ܾ�˻� ��ȯ���� �ޱ����� ����
	while (inFile.good())
	{
		getline(inFile, line); // ���Ͽ��� ���ڿ��� �ٴ����� �д´�
		auto matchedID = kmp(line, userID); //ID�˻�

		if ((int)matchedID.size() >= 1)//�ϴ� ��ġ�ϴ� ������ �����ϸ�
		{
			//�� �� ���� ���̵�� �Էµ� ���̵��� ���ڿ� ���� ��
			std::string realID = line.substr(0, line.find('#', 1)); //�������̵���� �����ο��� ó�� #��������
			if (userID.length() == realID.length()) //���̵� Ʋ�� (��ġ�ϴ� ���̵� ������ ã������ ���̰� �޶� ���̵� �ƴѰ��) //�� ��� ȸ������ ����
				return 2; //ȸ������ ���̵� �ߺ��� ���� true
		}
		/*
		pos_ID = line.find(userID); // ���ο��� ���ڿ� �˻�
	    if (pos_ID != std::string::npos) // �ش���ο��� ã�� ���ڿ��� �����ҽ�
		{
			if (inFile.is_open())
				inFile.close();
			//std::cout << "ID �ߺ����� ȸ������ �Ұ�" << std::endl;
			return 2; //ȸ������ ���̵� �ߺ��� ���� true
		}
		*/
	}
	if (inFile.is_open())
		inFile.close();
	//std::cout << "ID �ߺ����� ȸ������ ����" << std::endl;
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
	//if (SearchClientInfo(userID,userPW))	// db�� ���������� �����ϴ� ���
	//	return false;
	
	// todo �������̵� �ʹ� ���� ������
	
	return SearchClientInfo(userID, userPW);

	//return true;	//todo ���� �۾� �Ϸ�Ǹ� ����
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
		auto matchedID = kmp(line, userID); //ID�˻�
		if ((int)matchedID.size() >= 1)//�ϴ� ��ġ�ϴ� ������ �����ϸ�
		{
			//�� �� ���� ���̵�� �Էµ� ���̵��� ���ڿ� ���� ��
			std::string realID = line.substr(0, line.find('#', 1)); //�������̵���� �����ο��� ó�� #��������
			if (userID.length() != realID.length()) //���̵� Ʋ�� (��ġ�ϴ� ���̵� ������ ã������ ���̰� �޶� ���̵� �ƴѰ��) //�� ���� �ٽ� �ݺ��ؼ� �˻��Ѵ�.
				continue;

			//PW �˻� (�ش������ id �޺κп��� �˻�)
			auto matchedPW = kmp(line.substr(userID.length() + 1, std::string::npos), userPW);
			std::cout << "pw���� ��ġ��:" << (int)matchedPW.size() << "\n";
			if ((int)matchedPW.size() >=1)
			{
				std::string real_PW = line.substr(userID.length() + 1, userPW.length());
				if (real_PW == userPW) //�ڸ�����(������й�ȣ)�� �Է¹��� ��й�ȣ�� ��ġ�ϴ��� ��
				{
					std::cout << "PW ����!\n";
					return 3; //�α��� ����
				}
				else
				{
					std::cout << "PW Ʋ��! case1\n";
					return 2; //�н����� Ʋ�� (�н����� ������ ��ġ������ ���̰� �޶� Ʋ�����)
				}
			}
			else
			{
				std::cout << "PW Ʋ��! case2\n";
				return 2; //�н����� Ʋ�� (�н����尡 �ƿ� Ʋ�����)
			}
		}
	}
	std::cout << "ID Ʋ�� case2\n";
	return 1; //���̵� Ʋ�� (��ġ�ϴ� ���̵� ��ã�� ���)
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
