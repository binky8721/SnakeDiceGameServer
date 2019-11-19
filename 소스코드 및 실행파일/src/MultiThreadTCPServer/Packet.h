#pragma once
#include <string>
#include <iostream>
using namespace std;

enum PacketHeader
{
	Register,
	Login,
	OpposingUserInfo,
	Chatting,
	SuccessLogin,
	MatchingSuccess,
	SendGameInfo,
	RecvGameInfo,
	
	Response, // 응답(클라에서만)
};

struct PacketType	// 어떤 패킷인지 구분
{
	PacketHeader header;
	int totalSize;
};

struct RegisterPacket : public PacketType
{
	string id;
	string pw;
};

struct LoginPacket : public PacketType
{
	string id;
	string pw;
};

struct OpposingUserInfoPacket : PacketType
{
	string opposingId;
};

struct ChattingPacket : PacketType
{
	string message;
};

struct ResponsePacket : PacketType
{
	bool isSuccess;
	string errMessage;
};

struct SuccessLoginPacket :PacketType
{
	int gameState;
};

struct MatchingSuccessPacket :PacketType
{
	string id;
	int gameState;
};

struct SendGameInfoPacket :PacketType
{
	bool isGameOver;
	int dice;
	int pos;
	int posX;
	int posY;
	bool isMyTurn;
	string id;
	string chat;
	string changePosChat;
};

struct RecvGameInfoPacket :PacketType
{
	string chat;
};