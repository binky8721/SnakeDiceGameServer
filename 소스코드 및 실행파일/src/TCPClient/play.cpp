//#include <iostream>
//#include "Board.h"
//#include "Player.h"
//
//int play()
//{
//	Board* b = new Board();
//	int numberOfPlayer;
//	char ch;
//
//	std::cout<<"enter no of players : " ;
//	std::cin>>numberOfPlayer;//서버에서 값을 전달
//
//	std::cout<<"enter players name : "<<std::endl;//서버에서 값을 전달
//	fflush(stdin);
//	for(int i=0;i<numberOfPlayer;i++)//클라에 있어야함, 유저정보 초기화 부분
//	{
//		std::string name;
//		std::cin>>name;
//		Player* p = new Player(name);
//		b->AddPlayer(p);
//	}
//
//	if(b->NextPlayer() == nullptr)
//		return 0;
//
//	std::cout<<"start"<<std::endl;
//
//	int choice;
//	do
//	{
//		b->DisplayStatus();
//		Player* p = b->NextPlayer();
//		std::cout<<"1 > '" +  p->GetName() << "' is playing, through dice."<<std::endl ;
//		std::cout<<"2 > exit game"<<std::endl;
//		fflush(stdin);
//		std::cin>>choice;
//
//		switch (choice)
//		{
//
//		case 1://채팅입력은 클라, 값을 계산하고 넘겨주는건 서버
//			{
//				int diceNumber = p->ThroughDice();
//				int position = b->UpdatedPosition(p->GetPosition()+diceNumber);
//				Player::Status status = p->Move(position);
//				if(status == Player::Status::Win)
//				{
//					std::cout<< p->GetName()<<" won" <<std::endl;
//					choice = 2;
//				}
//			}
//
//			break;
//		case 2:
//			std::cout<< "Thank you, Bye"<<std::endl;
//			break;
//		
//		default:
//				std::cout<<"wrong input" <<std::endl;
//				break;
//		}
//
//	}while(choice != 2);
//
//
//
//	return 0;
//}