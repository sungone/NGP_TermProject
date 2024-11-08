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
		ErrorHandler("ERROR: ������ �ʱ�ȭ �� �� �����ϴ�.");
	}

	//������ ������ ���� ����
	_connectedSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_connectedSocket == INVALID_SOCKET)
	{
		ErrorHandler("ERROR: ������ ������ �� �����ϴ�.");

	}

	int nOpt = 1;

	if (::setsockopt(_connectedSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&nOpt, sizeof(nOpt)) == SOCKET_ERROR)
	{
		ErrorHandler("���̱� �˰��� OFF ���� .");
	}


	//��Ʈ ���ε� �� ����
	SOCKADDR_IN	svraddr = { 0 };
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(PortNum);
	::inet_pton(AF_INET, IPAddress, &svraddr.sin_addr);

	if (::connect(_connectedSocket, (SOCKADDR*)&svraddr, sizeof(svraddr)) == SOCKET_ERROR)
	{
		ErrorHandler("ERROR: ������ ������ �� �����ϴ�.");
	}

}

void Client::Send()
{
	// ������û���� �������� ��������. �ڱ���ġ���� ��������..


}

void Client::PacketDecode()
{
	MYCMD cmd;

	while (::recv(_connectedSocket, (char*)&cmd, sizeof(MYCMD), MSG_WAITALL) > 0)
	{
		switch (cmd.Code) // ��ɺ��� �ؼ��� ���� 
		{
		case Connect:
			_clientID = cmd.ClientID;
			if (_clientID == 1)
				_clientMaster = true;
			else
				_clientMaster = false;
			break;
		case MatcingStartReady: //3���� ����
			std::cout << "��Ī �غ� �Ϸ�Ǿ����ϴ�!" << "\n";
			screen.status = 4;
			break;
		case ChattingData:
			RecvMessageFromServer(cmd.Size);
			break;
		default:
			ErrorHandler("�˼����� ��ɾ� �����߽��ϴ�.");
			break;
		}

	}

	puts("���� �����尡 �������ϴ�.");

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
		//��ɾ� �Է�.

		::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);
		//����ڰ� �Է��� ���ڿ��� ������ �����Ѵ�.
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
	
	// ��� �浹 ����
}

void Client::BlockCreate()
{
	player.crashOnce = false;
	BlockCreateInfo info;
	// ���� ����
}

void Client::BlockCreateReceive(BlockCreateInfo blockInfo)
{
	player.crashOnce = false;
}

void Client::HpUpdate(int hpData)
{
	// hp ������Ʈ

	hp = hpData;
	if (3 == hp) // 3�� �浹  < ���� ���� >
	{
		screen.status = 3;

		PlaySound(L"sound/closing.wav", NULL, SND_ASYNC | SND_LOOP);//sound

		player.init();
		camera.setCamera(shaderProgramID, 0, cameraMode, player.getPos());
		screen.initTex();
	}
	else if (2 == hp and !hpBarSet[1]) // 2�� �浹
	{
		screen.status = 5;
		screen.initTex();
		hpBarSet[1] = true;
	}
	else if (1 == hp and !hpBarSet[0]) // 1�� �浹
	{
		screen.status = 4;
		screen.initTex();
		hpBarSet[0] = true;
	}
}


void Client::PlayerInfo()
{
}


