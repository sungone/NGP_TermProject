#pragma once
class Client
{
public:
	Client() = default;
	~Client();

	void Init();

public:
	void PacketDecode(); //�����κ��� ��Ŷ�� �޾Ƽ� �������� �ؼ��Ͽ� �������� �Լ��θ� ����.
	void RecvMessageFromServer(int size);
	void SendMessageToAllclinet(); //ä��
	void SendMatchingStart();

private:
	
	SOCKET _connectedSocket;
};

