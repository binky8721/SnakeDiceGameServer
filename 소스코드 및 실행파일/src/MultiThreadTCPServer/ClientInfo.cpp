#include "ClientInfo.h"



ClientInfo::ClientInfo()
{
}

ClientInfo::ClientInfo(int keyId, string userId, string pw, string icon, int winCount, int loseCount, SOCKET mySock)
{
	keyID = keyId;
	clientID = userId;
	clientPW = pw;
	clientIcon = icon;
	record.winCount = winCount;
	record.loseCount = loseCount;
	this->mySock = mySock;
}


ClientInfo::~ClientInfo()
{
}

int ClientInfo::GetKeyID() const
{
	return keyID;
}

string ClientInfo::GetClientID() const
{
	return clientID;
}

SOCKET ClientInfo::GetMySock() const
{
	return mySock;
}
