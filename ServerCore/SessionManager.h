#pragma once

/****************************************************************************
*    SessionManager   : Client ���� �����ϱ����� Ŭ�����̴�  
*    �������� ����Լ� �����δ� �ܼ�ȭ �ϱ����ؼ� SessionManager ���� ó�� 
*****************************************************************************/

class SessionManager
{
public:
	
	void	PrintClinetInfo(SOCKET socket, string Message);
	void	PushClient(SOCKET socket);
	void	DeleteClient(SOCKET socket);
	int		GetClinetCount();

public:
	//�������� �Լ�������
	void PacketDecode(SOCKET socket); //Packet�� ������ Ȯ���Ͽ� �������� �Լ��θ� ��������.
	void SendMessageToAllclinet(SOCKET socket); //ä��

	// �������� �������� ���� Ŭ���̾�Ʈ�� ������ �Լ� -> Ŭ���̾�Ʈ�� �� �������� ������ ������ ������ , ���������� �������� ����
	void MakeBlockRandomSeed();

private:
	mutex _mutex;
	concurrent_queue<SOCKET> _listClient;
};

