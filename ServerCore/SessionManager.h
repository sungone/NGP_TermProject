#pragma once

/****************************************************************************
*    SessionManager   : Client ���� �����ϱ����� Ŭ�����̴�  
*    �������� ����Լ� �����δ� �ܼ�ȭ �ϱ����ؼ� SessionManager ���� ó�� 
*****************************************************************************/

class SessionManager
{
public:
<<<<<<< Updated upstream
	void	PrintClinetInfo(SOCKET socket);
	void	PushClient(SOCKET socket);
	void	DeleteClient(SOCKET socket);
	int		GetClinetCount() { return _listClient.size(); }
=======
	void	PrintClinetInfo(SOCKET socket, string Message);
	void	PushClient(SOCKET socket);
	void	DeleteClient(SOCKET socket);
	int		GetClinetCount();
>>>>>>> Stashed changes

public:
	//�������� �Լ�������

	void PacketDecode(SOCKET socket); //Packet�� ������ Ȯ���Ͽ� �������� �Լ��θ� ��������.
	void SendMessageToAllclinet(SOCKET socket); //ä��


private:
	mutex _queueLock; 
	concurrent_queue<SOCKET> _clientQueue; 
};

