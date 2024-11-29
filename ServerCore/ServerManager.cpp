#include "pch.h"
#include "ServerManager.h"

ServerManager::ServerManager()
{
	_IDGenator.resize(4, -1);
}

ServerManager::~ServerManager()
{

}

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
		case ENUM::Connect:
			RecvConnect(socket);
			break;
		case ENUM::StartGame:
			RecvSendStartGame(socket);
			break;
		case ENUM::MatchingCancle:
			RecvMathcingCancle(socket);
			break;
		case ENUM::ClientInfoData:
			PlayerInfo(socket , cmd);
			break;
		case ENUM::ChattingData:
			RecvSendChattingData(socket,cmd.Size); 
			break;
		case ENUM::BlockDataRecv:
			MakeBlockSend(socket);
			break;
		case ENUM::BlockCollision:
			BlockCollision(socket);
			break;
		case ENUM::DisconnectClient :
			RecvSendDisconnect(socket , cmd.ClientID , cmd.IsClientMaster);
			DeleteClient(socket);
			break;
		default:
			ErrorHandler("알수없는 명령어 수신했습니다.");
			break;
		}
	}
}

void ServerManager::RecvConnect(SOCKET socket)
{
	lock_guard<mutex> gard(_mutex);
	PrintClinetInfo(socket, "으로부터 Connection 요청 입력받음");

	//ID부여
	
	MYCMD cmd;
	cmd.Code = ENUM::Connect;
	cmd.Size = 0;


	for (int i = 1; i <= 3; ++i)
	{
		if (_IDGenator[i] == -1)
		{
			_IDGenator[i] = 1;
			cmd.ClientID = i;
			break;
		}
	}


	::send(socket, (char*)&cmd , sizeof(cmd), 0);
}

void ServerManager::RecvSendStartGame(SOCKET socket)
{
	PrintClinetInfo(socket, "으로부터 Mathcing 요청 입력받음");

	lock_guard<mutex> guard(_mutex);
	_readyCount++;
	cout << "_readyCount :" << _readyCount << "\n";

	if (_readyCount == 3)
	{
		MYCMD cmd;

		cmd.Code = ENUM::StartGame;
		cmd.Size = 0;
		
		for (auto it = _listClient.unsafe_begin(); it != _listClient.unsafe_end(); ++it)
		{
			::send(*it, (char*)&cmd, sizeof(cmd), 0);
		}
		_readyCount = 0;
	}
}

void ServerManager::RecvMathcingCancle(SOCKET socket)
{
	PrintClinetInfo(socket, "으로부터 Mathcing 취소요청 입력받음");
	_readyCount--;
	cout << "_readyCount :" << _readyCount << "\n";
}

void ServerManager::RecvSendChattingData(SOCKET socket,int size)
{

	PrintClinetInfo(socket,"으로부터 Chat요청 입력받음");

	char Message[256] = { 0 };

	MYCMD cmd;

	cmd.Code = ENUM::ChattingData;
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

void ServerManager::BlockCollision(SOCKET socket)
{
	lock_guard<mutex> guard(_mutex);

	_hp += 1;
	HPInfo info;
	info.hp = _hp;
	MYCMD cmd;
	PrintClinetInfo(socket, "으로부터 충돌 이벤트 받음");

	cout << "hp :" << _hp << endl;
	cmd.Code = ENUM::HPSync;

	for (auto it = _listClient.unsafe_begin(); it != _listClient.unsafe_end(); ++it)
	{
		::send(*it, (char*)&cmd, sizeof(cmd), 0);
		::send(*it, (char*)&info, sizeof(info), 0);
	}
}

void ServerManager::MakeBlockSend(SOCKET socket)
{
	MYCMD cmd;

	cmd.Code = ENUM::BlockDataRecv;
	cmd.Size = 0;

	BlockCreateInfo blockInfo;

	::recv(socket, (char*)&blockInfo, sizeof(BlockCreateInfo), MSG_WAITALL);

	PrintClinetInfo(socket, "으로부터 블록 정보 전파");

	for (auto it = _listClient.unsafe_begin(); it != _listClient.unsafe_end(); ++it)
	{
		//자기자신에게 쏠필요가없음
		if (*it == socket)
			continue;

		::send(*it, (char*)&cmd, sizeof(cmd), 0);
		::send(*it, (char*)&blockInfo, sizeof(BlockCreateInfo), 0);
	}
}

void ServerManager::PlayerInfo(SOCKET socket  ,MYCMD& cmd)
{
	ClientInfoPacket packet;

	::recv(socket, (char*)&packet, sizeof(ClientInfoPacket), MSG_WAITALL);
	
	for (auto it = _listClient.unsafe_begin(); it != _listClient.unsafe_end(); ++it)
	{
		if (*it == socket)
			continue;

		::send(*it, (char*)&cmd, sizeof(cmd), 0);
		::send(*it, (char*)&packet, sizeof(ClientInfoPacket), 0);
	}
}


void ServerManager::RecvSendDisconnect(SOCKET socket , int clientID, bool isClientMaster)
{
	lock_guard<mutex> gard(_mutex);

	MYCMD cmd;
	cmd.Code = ENUM::DisConnectClientInfo;
	cmd.Size = 0;
	cmd.ClientID = clientID;
	cmd.IsClientMaster = isClientMaster;

	PrintClinetInfo(socket, "으로부터 접속종료 패킷받음.");

	for (auto it = _listClient.unsafe_begin(); it != _listClient.unsafe_end(); ++it)
	{
		//자기자신에게 쏠필요가없음
		if (*it == socket)
			continue;

		::send(*it, (char*)&cmd, sizeof(cmd), 0);
	}


	_IDGenator[clientID] = -1;

}
