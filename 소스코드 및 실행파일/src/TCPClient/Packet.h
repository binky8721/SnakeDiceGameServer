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
	RecvGameInfo,
	SendGameInfo,	

	Response, // ����(Ŭ�󿡼���)
};

struct PacketType	// � ��Ŷ���� ����
{
	PacketHeader header;
	int totalSize;
};

struct RegisterPacket :PacketType
{
	string id;
	string pw;
};

struct LoginPacket :PacketType
{
	string id;
	string pw;
};

struct OpposingUserInfoPacket :PacketType
{
	string opposingId;
};

struct ChattingPacket :PacketType
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

struct RecvGameInfoPacket :PacketType
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

struct SendGameInfoPacket :PacketType
{
	string chat;
};