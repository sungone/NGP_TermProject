#pragma once

/****************************************************************************
*    ServerManager   : Client ���� �����ϱ����� Ŭ�����̴�  
*    �������� ����Լ� �����δ� �ܼ�ȭ �ϱ����ؼ� ServerManager ���� ó�� 
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
	void MakeBlockRandomSeed(); // �������� �������� ���� Ŭ���̾�Ʈ�� ������ �Լ� -> Ŭ���̾�Ʈ�� �� �������� ������ ������ ������ , ���������� �������� ����

private:
	mutex _mutex;
	concurrent_queue<SOCKET> _listClient;

	atomic<int> _readyCount = 0;
	atomic<int> IDGenator =1;
};

