#include "pch.h"
#include "ServerManager.h"


void ServerManager::PrintClinetInfo(SOCKET socket, string message)
{
#ifdef _DEBUG  // ����� ��忡���� ����
	struct sockaddr_in clientaddr;
	char addr[32];
	int addrlen;
	addrlen = sizeof(clientaddr);

	::getpeername(socket, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	cout << "Ŭ���̾�Ʈ : IP �ּ�:" << addr << "��Ʈ ��ȣ : " << ntohs(clientaddr.sin_port) << " ";
	cout << message << "\n";
#endif
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
		switch (cmd.Code) // ��ɺ��� �ؼ��� ���� 
		{
		case Connect:
			ConnectClient(socket);
			break;
		case MatcingStartReady:
			MatchingAccept(socket);
			break;
		case MatchingCancle:
			MathcingOff(socket);
			break;
		case ClientInfoData:
			break;
		case ChattingData:
			SendMessageToAllclinet(socket,cmd.Size); 
			break;
		default:
			ErrorHandler("�˼����� ��ɾ� �����߽��ϴ�.");
			break;
		}

	}

}

void ServerManager::ConnectClient(SOCKET socket)
{
	PrintClinetInfo(socket, "���κ��� Connection ��û �Է¹���");

	MYCMD cmd;
	cmd.Code = Connect;
	cmd.Size = 0;
	cmd.ClientID = IDGenator++;

	::send(socket, (char*)&cmd , sizeof(cmd), 0);
}


void ServerManager::MatchingAccept(SOCKET socket)
{
	PrintClinetInfo(socket, "���κ��� Mathcing ��û �Է¹���");

	lock_guard<mutex> guard(_mutex);
	_readyCount++;
	cout << "_readyCount :" << _readyCount << "\n";

	if (_readyCount == 3)
	{
		MYCMD cmd;

		cmd.Code = MatcingStartReady;
		cmd.Size = 0;
		
		for (auto it = _listClient.unsafe_begin(); it != _listClient.unsafe_end(); ++it)
		{
			::send(*it, (char*)&cmd, sizeof(cmd), 0);
		}
	}
}

void ServerManager::MathcingOff(SOCKET socket)
{
	PrintClinetInfo(socket, "���κ��� Mathcing ��ҿ�û �Է¹���");
	_readyCount--;
	cout << "_readyCount :" << _readyCount << "\n";

}

void ServerManager::SendMessageToAllclinet(SOCKET socket,int size)
{

	PrintClinetInfo(socket,"���κ��� Chat��û �Է¹���");

	char Message[256] = { 0 };

	MYCMD cmd;

	cmd.Code = ChattingData;
	cmd.Size = size; 

	::recv(socket, Message, cmd.Size, MSG_WAITALL);

	for (auto it = _listClient.unsafe_begin(); it != _listClient.unsafe_end(); ++it)
	{
		//�ڱ��ڽſ��� ���ʿ䰡����
		if (*it == socket)
			continue;
		
		::send(*it, (char*)&cmd, sizeof(cmd),0);
		::send(*it, Message, cmd.Size, 0);
	}

}


