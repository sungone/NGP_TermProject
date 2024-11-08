#pragma once
class Client
{
public:
	Client() = default;
	~Client();

	void Init();
	void Send(); //�������� ���� ������ ���� �ʿ��� �����͸� �������� ����

public:
	void PacketDecode(); //�����κ��� ��Ŷ�� �޾Ƽ� �������� �ؼ��Ͽ� �������� �Լ��θ� ����.
	void SendConnectServer(); //�������� �����ҋ� ID �ο� ������ �޼�������
	void RecvMessageFromServer(int size); //ä��
	void SendMessageToAllclinet(); //ä��
	void SendMatchingStart(); //�������� Maching ��û
	void SendMatchingCancel();//�������� Maching ��ҿ�û

	//�Ʒ��� �����ؾ���
	void BlockCollision();
	void PlayerInfo();
public:
	int _clientID;
	bool _clientMaster = false;
private:
	SOCKET _connectedSocket;
};

