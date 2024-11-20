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
	void RecvConnect(MYCMD& cmd);
	void RecvStartGame();
	void SendConnect(); //�������� �����ҋ� ID �ο� ������ �޼�������
	void RecvChattingData(int size); //ä��
	void SendChattingData(); //ä��
	void SendStartGame(); //�������� Maching ��û
	void SendMatchingCancel();//�������� Maching ��ҿ�û

	void SendPlayerInfo(); // �ٸ� Ŭ���̾�Ʈ�� ����ȭ�� ���� ������ ���� �÷��̾��� ��ġ , ���� , ũ�� ������ ������
	void ClientInfoData(int ClientID , ClientInfoPacket cInfo); // �ٸ� Ŭ���̾�Ʈ�� ������ ������ �ݿ��ϴ� �Լ�

	void updateViewerPosX(ViewerPlayer* pViewer , float PosX);

	void DisConnectClient(); // �� Ŭ���̾�Ʈ�� ������ ���� �Լ�
	void OtherClientIdData(int ClientID, bool isMaster); // �ٸ� Ŭ���̾�Ʈ�� ������ ���� �Լ�

	void CreateClientPlayer(int ClientID); // ��� Ŭ���̾�Ʈ�� �����ϴ� �Լ�
	ViewerPlayer* FindClientPlayer(int ClientID); // ��� Ŭ���̾�Ʈ�� ã�� �Լ�
	void RemoveClientPlayer(int ClientID); // ��� Ŭ���̾�Ʈ�� ���ִ� �Լ�

	//�Ʒ��� �����ؾ���
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

