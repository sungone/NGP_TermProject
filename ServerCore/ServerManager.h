#pragma once

/****************************************************************************
*    ServerManager   : Client ���� �����ϱ����� Ŭ�����̴�  
*****************************************************************************/

class ServerManager
{
public:
	ServerManager();
	~ServerManager();
	
	void	PrintClinetInfo(SOCKET socket, string Message);
	void	PushClient(SOCKET socket);
	void	DeleteClient(SOCKET socket);
	int		GetClinetCount();
public:
	//�������� �Լ�������
	void PacketDecode(SOCKET socket); //Packet�� ������ Ȯ���Ͽ� �������� �Լ��θ� ��������.
	void RecvConnect(SOCKET socket); //��û�� �޾� ID �ο�����
	void RecvSendStartGame(SOCKET socket); // Matching ��û �޾Ƽ� ó������.
	void RecvMathcingCancle(SOCKET socket); //MachingCancle ��û �޾Ƽ� ó������
	void RecvSendChattingData(SOCKET socket,int size); //ä��

	/* �Ʒ��� �����ؾ���*/
	void BlockCollision(SOCKET socket); //�ǰ˻縦 �ؼ� GameOver ��Ű�ų� hp�� ����.
	void MakeBlockSend(SOCKET socket); //��ϻ����ؼ� ������
	void PlayerInfo(SOCKET socketm, MYCMD&cmd); //�÷��̾��� ������ �޾Ƽ� ��ε�ĳ���ù������ ����.
	void Restart(SOCKET socket);  // �����
	void ReturnMenu(SOCKET socket); //������ ���� �ٽ� ���Ī �Ҽ��ְ�����

	void RecvSendDisconnect(SOCKET socket , int clientID , bool isClientMaster);
private:
	mutex _mutex;
	concurrent_queue<SOCKET> _listClient;
	atomic<int> _readyCount = 0;
	atomic<int> _IDGenator = 0;
	//vector<int> _IDGenator;
	atomic<int> _hp = 0;
};

