#pragma once

/****************************************************************************
*    SessionManager   : Client ���� �����ϱ����� Ŭ�����̴�  
*    �������� ����Լ� �����δ� �ܼ�ȭ �ϱ����ؼ� SessionManager ���� ó�� 
*****************************************************************************/

class SessionManager
{
public:
	
	void	PrintClinetInfo(SOCKET socket, string Message = {});
	
	void	PushClient(SOCKET socket);
	void	DeleteClient(SOCKET socket);
	
	int		GetClinetCount() { return _listClient.size(); }

public:
	//�������� �Լ�������
	void PacketDecode(SOCKET socket); //Packet�� ������ Ȯ���Ͽ� �������� �Լ��θ� ��������.
	void SendMessageToAllclinet(SOCKET socket); //ä��

private:
	mutex _mutex;
	std::list<SOCKET> _listClient;
};

