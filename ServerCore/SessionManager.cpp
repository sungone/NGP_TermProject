#include "pch.h"
#include "SessionManager.h"

void SessionManager::PrintClinetInfo(SOCKET socket, string message)
{
	struct sockaddr_in clientaddr;
	char addr[256];
	int addrlen;
	addrlen = sizeof(clientaddr);

	::getpeername(socket, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	cout << "Ŭ���̾�Ʈ : IP �ּ�:" << addr << "��Ʈ ��ȣ : " << ntohs(clientaddr.sin_port) << " ";
	cout << message << "\n";
}

void SessionManager::PushClient(SOCKET socket)
{
	lock_guard<mutex> guard(_mutex);
	_listClient.push_back(socket);
}

void SessionManager::DeleteClient(SOCKET socket)
{
	lock_guard<mutex> guard(_mutex);
	_listClient.remove(socket);
	::closesocket(socket);
}

void SessionManager::PacketDecode(SOCKET socket)
{
	MYCMD cmd;

	while (::recv(socket, (char*)&cmd, sizeof(MYCMD), MSG_WAITALL) > 0)
	{
		switch (cmd.Code) // ��ɺ��� �ؼ��� ���� 
		{
		case ClientInfoData:
			break;
		case BlockData:
			break;
		case ChattingData:
			SendMessageToAllclinet(socket);
			break;
		default:
			ErrorHandler("�˼����� ��ɾ� �����߽��ϴ�.");
			break;
		}

	}

}

void SessionManager::SendMessageToAllclinet(SOCKET socket)
{
	lock_guard<mutex> guard(_mutex);

	PrintClinetInfo(socket,"���κ��� Chat��û �Է¹���");

	char Message[256] = { 0 };

	::recv(socket, Message, sizeof(Message), 0);

	for (auto it = _listClient.begin(); it != _listClient.end(); ++it)
	{
		//�ڱ��ڽſ��� ���ʿ䰡����
		if (*it == socket)
			continue;

		::send(*it, Message, sizeof(Message), 0);
	}
}


