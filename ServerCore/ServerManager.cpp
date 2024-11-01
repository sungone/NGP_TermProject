#include "pch.h"
#include "ServerManager.h"
#include <random>
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis(0, 99);

void ServerManager::PrintClinetInfo(SOCKET socket, string message)
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

void ServerManager::PushClient(SOCKET socket)
{
	_listClient.push(socket);
}

void ServerManager::DeleteClient(SOCKET socket)
{
	if (_listClient.try_pop(socket))
	{
		::closesocket(socket);
	}
}

int ServerManager::GetClinetCount()
{
	lock_guard<mutex> lock(mutex);
	return _listClient.unsafe_size();
}

void ServerManager::PacketDecode(SOCKET socket)
{
	MYCMD cmd;

	while (::recv(socket, (char*)&cmd, sizeof(MYCMD), MSG_WAITALL) > 0)
	{
		switch (cmd.Code) // ���ɺ��� �ؼ��� ���� 
		{
		case ClientInfoData:
			break;
		case BlockData:
			break;
		case ChattingData:
			SendMessageToAllclinet(socket); 
			break;
		default:
			ErrorHandler("�˼����� ���ɾ� �����߽��ϴ�.");
			break;
		}

	}

}

void ServerManager::SendMessageToAllclinet(SOCKET socket)
{

	PrintClinetInfo(socket,"���κ��� Chat��û �Է¹���");

	char Message[256] = { 0 };

	::recv(socket, Message, sizeof(Message), 0);

	for (auto it = _listClient.unsafe_begin(); it != _listClient.unsafe_end(); ++it)
	{
		//�ڱ��ڽſ��� ���ʿ䰡����
		if (*it == socket)
			continue;

		::send(*it, Message, sizeof(Message), 0);
	}

}

void ServerManager::MakeBlockRandomSeed()
{
	char randomNum[108];
	int col = 36;
	int row = 3;

	for (int i = 0; i < (col * row); ++i)
	{
		randomNum[i] = dis(gen);
	}

	for (auto it = _listClient.unsafe_begin(); it != _listClient.unsafe_begin(); ++it)
		::send(*it, randomNum, sizeof(randomNum), 0);
}


