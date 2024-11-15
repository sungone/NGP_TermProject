#pragma once
#include "PacketStructer.h"
#include "ViewerPlayer.h"

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

