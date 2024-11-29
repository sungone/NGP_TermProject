#pragma once
#include "PacketStructer.h"
class ViewerPlayer;

class Client
{
public:
	Client();
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

	void InitViewerPlayer(int MyClientID);

	void SendPlayerInfo(); // �ٸ� Ŭ���̾�Ʈ�� ����ȭ�� ���� ������ ���� �÷��̾��� ��ġ , ���� , ũ�� ������ ������
	void ClientInfoData(MYCMD& cmd); // �ٸ� Ŭ���̾�Ʈ�� ������ ������ �ݿ��ϴ� �Լ�

	void updateViewerPosX(ViewerPlayer* pViewer, float PosX);
	void updateViewerColor(ViewerPlayer* pViewer, float r, float g, float b);
	void updateViewerScale(ViewerPlayer* pViewer, float x, float y);

	void DisConnectClient(); // �� Ŭ���̾�Ʈ�� ������ ���� �Լ�
	void DisConnectClientInfo(int ClientID, bool isMaster); // �ٸ� Ŭ���̾�Ʈ�� ������ ���� �Լ�

	void CreateClientPlayer(int ClientID); // ��� Ŭ���̾�Ʈ�� �����ϴ� �Լ�
	ViewerPlayer* FindClientPlayer(int ClientID); // ��� Ŭ���̾�Ʈ�� ã�� �Լ�
	void RemoveClientPlayer(int ClientID); // ��� Ŭ���̾�Ʈ�� ���ִ� �Լ�
	void GameOver();

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

