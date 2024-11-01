#include "pch.h"
#include "ServerManager.h"
#include <random>
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis(0, 99);

void ServerManager::PrintClinetInfo(SOCKET socket, string message)
{
	struct sockaddr_in clientaddr;
	char addr[32];
	int addrlen;
	addrlen = sizeof(clientaddr);

	::getpeername(socket, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	cout << "클라이언트 : IP 주소:" << addr << "포트 번호 : " << ntohs(clientaddr.sin_port) << " ";
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
		switch (cmd.Code) // 명령부의 해석에 따라 
		{
		case MatcingStartReady:
			MatchingAccept(socket);
			break;
		case MatchingCancle:
			MathcingOff(socket);
			break;
		case ClientInfoData:
			break;
		case BlockData:
			break;
		case ChattingData:
			SendMessageToAllclinet(socket,cmd.Size); 
			break;
		default:
			ErrorHandler("알수없는 명령어 수신했습니다.");
			break;
		}

	}

}

void ServerManager::MatchingAccept(SOCKET socket)
{
	PrintClinetInfo(socket, "으로부터 Mathcing 요청 입력받음");

	_readyCount++;
	cout << "_readyCount :" << _readyCount << "\n";

	if (_readyCount == 3)
	{
		MYCMD cmd;

		cmd.Code = MatcingStartReady;
		cmd.Size = 0;
		
		lock_guard<mutex> lock(_mutex);

		for (auto it = _listClient.unsafe_begin(); it != _listClient.unsafe_end(); ++it)
		{
			::send(*it, (char*)&cmd, sizeof(cmd), 0);
		}
	}
}

void ServerManager::MathcingOff(SOCKET socket)
{
	PrintClinetInfo(socket, "으로부터 Mathcing 취소요청 입력받음");

	_readyCount--;
	cout << "_readyCount :" << _readyCount << "\n";

}

void ServerManager::SendMessageToAllclinet(SOCKET socket,int size)
{

	PrintClinetInfo(socket,"으로부터 Chat요청 입력받음");

	char Message[256] = { 0 };

	MYCMD cmd;

	cmd.Code = ChattingData;
	cmd.Size = size; 

	::recv(socket, Message, cmd.Size, MSG_WAITALL);

	for (auto it = _listClient.unsafe_begin(); it != _listClient.unsafe_end(); ++it)
	{
		//자기자신에게 쏠필요가없음
		if (*it == socket)
			continue;
		
		::send(*it, (char*)&cmd, sizeof(cmd),0);
		::send(*it, Message, cmd.Size, 0);
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



