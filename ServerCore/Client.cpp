#include "pch.h"
#include "Client.h"
#include "Packet.h"
#include "ClientData.h"

Client::~Client()
{
	::closesocket(_connectedSocket);
	::Sleep(100);
	::WSACleanup();
}

void Client::Init()
{

	WSADATA wsa = { 0 };
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		ErrorHandler("ERROR: 윈속을 초기화 할 수 없습니다.");
	}

	//서버에 연결할 소켓 생성
	_connectedSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_connectedSocket == INVALID_SOCKET)
	{
		ErrorHandler("ERROR: 소켓을 생성할 수 없습니다.");

	}

	int nOpt = 1;

	if (::setsockopt(_connectedSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&nOpt, sizeof(nOpt)) == SOCKET_ERROR)
	{
		ErrorHandler("네이글 알고리즘 OFF 실패 .");
	}


	//포트 바인딩 및 연결
	SOCKADDR_IN	svraddr = { 0 };
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(PortNum);
	::inet_pton(AF_INET, IPAddress, &svraddr.sin_addr);

	if (::connect(_connectedSocket, (SOCKADDR*)&svraddr, sizeof(svraddr)) == SOCKET_ERROR)
	{
		ErrorHandler("ERROR: 서버에 연결할 수 없습니다.");
	}

}

void Client::Send()
{
	// 여러요청들을 서버에게 보낼거임. 자기위치정보 보내기등등..


}

void Client::PacketDecode()
{
	MYCMD cmd;

	while (::recv(_connectedSocket, (char*)&cmd, sizeof(MYCMD), MSG_WAITALL) > 0)
	{
		switch (cmd.Code) // 명령부의 해석에 따라 
		{
		case Connect:
			_clientID = cmd.ClientID;
			if (_clientID == 1)
				_clientMaster = true;
			else
				_clientMaster = false;
			break;
		case MatcingStartReady: //3명이 모임
			std::cout << "매칭 준비가 완료되었습니다!" << "\n";
			screen.status = 4;
			break;
		case ChattingData:
			RecvMessageFromServer(cmd.Size);
			break;
		default:
			ErrorHandler("알수없는 명령어 수신했습니다.");
			break;
		}

	}

	puts("수신 스레드가 끝났습니다.");

}

void Client::SendConnectServer()
{
	MYCMD cmd;
	cmd.Code = Connect;
	cmd.Size = 0;
	cmd.ClientID = _clientID;

	::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);
}

void Client::RecvMessageFromServer(int size)
{
	char Message[256] = { 0 };
	int len = ::recv(_connectedSocket, Message, size, MSG_WAITALL);
	cout << Message << "\n";
}

void Client::SendMessageToAllclinet()
{
	while (1)
	{
		char Message[256] = { 0 };

		memset(Message, 0, sizeof(Message));
		cin >> Message;

		MYCMD cmd;

		cmd.Code = ChattingData;
		cmd.Size = strlen(Message);
		cmd.ClientID = _clientID;
		//명령어 입력.

		::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);
		//사용자가 입력한 문자열을 서버에 전송한다.
		::send(_connectedSocket, Message, cmd.Size, 0);
	}
}

void Client::SendMatchingStart()
{
	MYCMD cmd;
	cmd.Code = MatcingStartReady;
	cmd.Size = 0;
	cmd.ClientID = _clientID;

	::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);
}

void Client::SendMatchingCancel()
{
	MYCMD cmd;
	cmd.Code = MatchingCancle;
	cmd.Size = 0;
	cmd.ClientID = _clientID;

	::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);
}

void Client::BlockCollision()
{
	player.crashOnce = true;
	
	// 블록 충돌 전송
}

void Client::BlockCreate()
{
	player.crashOnce = false;
	BlockCreateInfo info;
	// 생성 전송
}

void Client::BlockCreateReceive(BlockCreateInfo blockInfo)
{
	player.crashOnce = false;
}

void Client::HpUpdate(int hpData)
{
	// hp 업데이트

	hp = hpData;
	if (3 == hp) // 3번 충돌  < 게임 오버 >
	{
		screen.status = 3;

		PlaySound(L"sound/closing.wav", NULL, SND_ASYNC | SND_LOOP);//sound

		player.init();
		camera.setCamera(shaderProgramID, 0, cameraMode, player.getPos());
		screen.initTex();
	}
	else if (2 == hp and !hpBarSet[1]) // 2번 충돌
	{
		screen.status = 5;
		screen.initTex();
		hpBarSet[1] = true;
	}
	else if (1 == hp and !hpBarSet[0]) // 1번 충돌
	{
		screen.status = 4;
		screen.initTex();
		hpBarSet[0] = true;
	}
}


void Client::PlayerInfo()
{
}


