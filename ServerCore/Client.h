#pragma once
class Client
{
public:
	Client() = default;
	~Client();

	void Init();
	void Send();

public:
	void PacketDecode(); //�����κ��� ��Ŷ�� �޾Ƽ� �������� �ؼ��Ͽ� �������� �Լ��θ� ����.
	void SendConnectServer(); //�������� �����ҋ� ID �ο� ������ �޼�������
	void RecvMessageFromServer(int size); //ä��
	void SendMessageToAllclinet(); //ä��
	void SendMatchingStart(); //�������� Maching ��û
	void SendMatchignCancle();//�������� Maching ��ҿ�û


public:
	int _clientID;
private:

	SOCKET _connectedSocket;
};

