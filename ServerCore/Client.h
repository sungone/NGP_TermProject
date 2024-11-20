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

	void SendPlayerInfo(); // �ٸ� Ŭ���̾�Ʈ�� ����ȭ�� ���� ������ ���� �÷��̾��� ��ġ , ���� , ũ�� ������ ������
	void RecvClientInfo(int ClientID , ClientInfoPacket cInfo); // �ٸ� Ŭ���̾�Ʈ�� ������ ������ �ݿ��ϴ� �Լ�

	void updateViewerPosX(ViewerPlayer* pViewer , float PosX);

	void DisConnectClient(); // �� Ŭ���̾�Ʈ�� ������ ���� �Լ�
	void DisConnectOtherClient(int ClientID, bool isMaster); // �ٸ� Ŭ���̾�Ʈ�� ������ ���� �Լ�

	void CreateClientPlayer(int ClientID); // ��� Ŭ���̾�Ʈ�� �����ϴ� �Լ�
	ViewerPlayer* FindClientPlayer(int ClientID); // ��� Ŭ���̾�Ʈ�� ã�� �Լ�
	void RemoveClientPlayer(int ClientID); // ��� Ŭ���̾�Ʈ�� ���ִ� �Լ�

	//�Ʒ��� �����ؾ���
	void BlockCollision();
	void BlockCreate();
	void BlockCreateReceive(BlockCreateInfo info);
	void HpUpdate(int hp);

public:
	int _clientID;
	bool _clientMaster = false;
private:
	SOCKET _connectedSocket;
};

