#pragma once
#include "PacketStructer.h"
#include "ViewerPlayer.h"

class Client
{
public:
	Client() = default;
	~Client();

	void Init();
	void Send(); //매프레임 메인 쓰레드 에서 필요한 데이터를 서버에게 전송

public:
	void PacketDecode(); //서버로부터 패킷을 받아서 실질적인 해석하여 실질적인 함수부를 구현.
	void SendConnectServer(); //서버에게 접속할떄 ID 부여 받을떄 메세지보냄
	void RecvMessageFromServer(int size); //채팅
	void SendMessageToAllclinet(); //채팅
	void SendMatchingStart(); //서버에게 Maching 요청
	void SendMatchingCancel();//서버에게 Maching 취소요청

	//아래는 구현해야함
	void BlockCollision();
	void BlockCreate();
	void BlockCreateReceive(BlockCreateInfo info);
	void HpUpdate(int hp);
	void PlayerInfo();

public:
	int _clientID;
	bool _clientMaster = false;
private:
	SOCKET _connectedSocket;

public :
	void CreateClientPlayer(int ClientID);
	ViewerPlayer* FindClientPlayer(int ClientID);
	void RemoveClientPlayer(int ClientID);

private :
	map<int, ViewerPlayer*> ViewerClient;

public :
	void DisConnectClient();
	void DeleteOtherClient(int ClientID , bool isMaster);
};

