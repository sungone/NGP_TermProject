#include "pch.h"
#include "SessionManager.h"

void SessionManager::PrintClinetInfo(SOCKET socket)
{
	struct sockaddr_in clientaddr;
	char addr[256];
	int addrlen;
	addrlen = sizeof(clientaddr);

	::getpeername(socket, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	cout<<"Ŭ���̾�Ʈ : IP �ּ�:"<<addr <<"��Ʈ ��ȣ : " << ntohs(clientaddr.sin_port) <<"\n";

}

void SessionManager::PushClient(SOCKET socket)
{
<<<<<<< Updated upstream
	lock_guard<mutex> guard(_mutex);
	_listClient.push_back(socket);
=======
	_clientQueue.push(socket);
>>>>>>> Stashed changes
}

void SessionManager::DeleteClient(SOCKET socket)
{
<<<<<<< Updated upstream
	lock_guard<mutex> guard(_mutex);
	_listClient.remove(socket);
	::closesocket(socket);
=======

	if (_clientQueue.try_pop(socket)) {
		::closesocket(socket);
	}
	else {
		assert(false);
	}
}

int SessionManager::GetClinetCount()
{
	lock_guard<mutex> guard(_queueLock);
	return _clientQueue.unsafe_size();
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
	lock_guard<mutex> guard(_mutex);
=======
	PrintClinetInfo(socket,"���κ��� Chat��û �Է¹���");
>>>>>>> Stashed changes
	char Message[256] = { 0 };

	::recv(socket, Message, sizeof(Message), 0);
	int nLength = strlen(Message);

<<<<<<< Updated upstream
	for (auto it = _listClient.begin(); it != _listClient.end(); ++it)
		::send(*it, Message, sizeof(char) * (nLength), 0);
=======

	std::vector<SOCKET> clientList;

	{
		std::lock_guard<std::mutex> guard(_queueLock);

		for (auto it = _clientQueue.unsafe_begin(); it != _clientQueue.unsafe_end(); ++it) 
		{
			clientList.push_back(*it);
		}
	}


	for (auto it = clientList.begin(); it != clientList.end(); ++it)
	{
		//�ڱ��ڽſ��� ���ʿ䰡����
		if (*it == socket)
			continue;

		::send(*it, Message, sizeof(Message), 0);
	}

>>>>>>> Stashed changes
}


