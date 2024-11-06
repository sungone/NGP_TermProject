#pragma once

/****************************************************************************
*    ServerManager   : Client ���� �����ϱ����� Ŭ�����̴�  
*****************************************************************************/

class ServerManager
{
public:
	
	void	PrintClinetInfo(SOCKET socket, string Message);
	void	PushClient(SOCKET socket);
	void	DeleteClient(SOCKET socket);
	int		GetClinetCount();
public:
	//�������� �Լ�������
	void PacketDecode(SOCKET socket); //Packet�� ������ Ȯ���Ͽ� �������� �Լ��θ� ��������.
	void ConnectClient(SOCKET socket); //��û�� �޾� ID �ο�����
	void MatchingAccept(SOCKET socket); // Matching ��û �޾Ƽ� ó������.
	void MathcingOff(SOCKET socket); //MachingCancle ��û �޾Ƽ� ó������
	void SendMessageToAllclinet(SOCKET socket,int size); //ä��
	/* �Ʒ��� �����ؾ���*/
	void BlockCollision(); //�ǰ˻縦 �ؼ� GameOver ��Ű�ų� hp�� ����.
	void MakeBlockSend(SOCKET socket); //��ϻ����ؼ� ������
	void PlayerInfo(SOCKET socket); //�÷��̾��� ������ �޾Ƽ� ��ε�ĳ���ù������ ����.
	void Restart(SOCKET socket);  // �����
	void ReturnMenu(SOCKET socket); //������ ���� �ٽ� ���Ī �Ҽ��ְ�����

private:
	mutex _mutex;
	concurrent_queue<SOCKET> _listClient;
	atomic<int> _readyCount = 0;
	atomic<int> IDGenator =1;
	atomic<int> _hp = 3;
};

