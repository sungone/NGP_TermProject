#pragma once
#include "PacketStructer.h"
class ViewerPlayer;

class Client
{
public:
	Client();
	~Client();

	void Init();
	void Send(); //매프레임 메인 쓰레드 에서 필요한 데이터를 서버에게 전송

public:
	void PacketDecode(); //서버로부터 패킷을 받아서 실질적인 해석하여 실질적인 함수부를 구현.
	void RecvConnect(MYCMD& cmd);
	void RecvStartGame();
	void SendConnect(); //서버에게 접속할떄 ID 부여 받을떄 메세지보냄
	void RecvChattingData(int size); //채팅
	void SendChattingData(); //채팅
	void SendStartGame(); //서버에게 Maching 요청
	void SendMatchingCancel();//서버에게 Maching 취소요청

	void InitViewerPlayer(int MyClientID);

	void SendPlayerInfo(); // 다른 클라이언트와 동기화를 위해 서버로 현재 플레이어의 위치 , 색깔 , 크기 정보를 보내줌
	void ClientInfoData(MYCMD& cmd); // 다른 클라이언트의 정보를 받으면 반영하는 함수

	void updateViewerPosX(ViewerPlayer* pViewer, float PosX);
	void updateViewerColor(ViewerPlayer* pViewer, float r, float g, float b);
	void updateViewerScale(ViewerPlayer* pViewer, float x, float y);

	void DisConnectClient(); // 내 클라이언트의 접속을 끊는 함수
	void DisConnectClientInfo(int ClientID, bool isMaster); // 다른 클라이언트의 접속을 끊는 함수

	void CreateClientPlayer(int ClientID); // 뷰어 클라이언트를 생성하는 함수
	ViewerPlayer* FindClientPlayer(int ClientID); // 뷰어 클라이언트를 찾는 함수
	void RemoveClientPlayer(int ClientID); // 뷰어 클라이언트를 없애는 함수
	void GameOver();

	//아래는 구현해야함
	void BlockCollision();
	void BlockCreate();
	void BlockCreateReceive();
	void HpUpdate();

public:
	int _clientID;
	bool _clientMaster = false;

private:
	SOCKET _connectedSocket;
};

