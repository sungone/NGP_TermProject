#pragma once
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
	void SendMatchignCancle();//서버에게 Maching 취소요청

	//아래는 구현해야함
	void BlockCollision();
	void PlayerInfo();
public:
	int _clientID;
private:
	SOCKET _connectedSocket;
};

