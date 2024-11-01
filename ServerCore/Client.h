#pragma once
class Client
{
public:
	Client() = default;
	~Client();

	void Init();

public:
	void PacketDecode(); //서버로부터 패킷을 받아서 실질적인 해석하여 실질적인 함수부를 구현.
	void RecvMessageFromServer(int size);
	void SendMessageToAllclinet(); //채팅
	void SendMatchingStart();

private:
	
	SOCKET _connectedSocket;
};

