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

	cout << "클라이언트 : IP 주소:" << addr << "포트 번호 : " << ntohs(clientaddr.sin_port) << " ";
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
		switch (cmd.Code) // 명령부의 해석에 따라 
		{
		case ClientInfoData:
			break;
		case BlockData:
			break;
		case ChattingData:
			SendMessageToAllclinet(socket);
			break;
		default:
			ErrorHandler("알수없는 명령어 수신했습니다.");
			break;
		}

	}

}

void SessionManager::SendMessageToAllclinet(SOCKET socket)
{
	lock_guard<mutex> guard(_mutex);

	PrintClinetInfo(socket,"으로부터 Chat요청 입력받음");

	char Message[256] = { 0 };

	::recv(socket, Message, sizeof(Message), 0);

	for (auto it = _listClient.begin(); it != _listClient.end(); ++it)
	{
		//자기자신에게 쏠필요가없음
		if (*it == socket)
			continue;

		::send(*it, Message, sizeof(Message), 0);
	}
}


