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
		case ENUM::Connect:
			RecvConnect(cmd);
			break;
		case ENUM::StartGame: //3���� ����
			RecvStartGame();
			break;
		case ENUM::ChattingData:
			RecvChattingData(cmd.Size);
			break;
		case ENUM::OtherClientIdData:
			OtherClientIdData(cmd.ClientID , cmd.IsClientMaster);
			break;
		case ENUM::ClientInfoData:
			ClientInfoData(cmd.ClientID , cmd.clientInfoPacket);
			break;
		case ENUM::BlockDataRecv:
			BlockCreateReceive();
			break;
		case ENUM::HPSync:
			HpUpdate();
			break;
		default:
			ErrorHandler("�˼����� ��ɾ� �����߽��ϴ�.");
			break;
		}

	}

	puts("���� �����尡 �������ϴ�.");

}

void Client::SendConnect()
{
	MYCMD cmd;
	cmd.Code = ENUM::Connect;
	cmd.Size = 0;
	cmd.ClientID = _clientID;

	::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);
}

void Client::RecvConnect(MYCMD& cmd)
{
	_clientID = cmd.ClientID;
	if (_clientID == 1)
		_clientMaster = true;
	else
		_clientMaster = false;
}


void Client::SendStartGame()
{
	MYCMD cmd;
	cmd.Code = ENUM::StartGame;
	cmd.Size = 0;
	cmd.ClientID = _clientID;

	::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);
}

void Client::RecvStartGame()
{
	std::cout << "��Ī �غ� �Ϸ�Ǿ����ϴ�!" << "\n";
	screen.status = 4;
}


void Client::SendChattingData()
{
	while (1)
	{
		char Message[256] = { 0 };

		memset(Message, 0, sizeof(Message));
		cin >> Message;

		MYCMD cmd;

		cmd.Code = ENUM::ChattingData;
		cmd.Size = strlen(Message);
		cmd.ClientID = _clientID;
		//��ɾ� �Է�.

		::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);
		//����ڰ� �Է��� ���ڿ��� ������ �����Ѵ�.
		::send(_connectedSocket, Message, cmd.Size, 0);
	}
}


void Client::RecvChattingData(int size)
{
	char Message[256] = { 0 };
	int len = ::recv(_connectedSocket, Message, size, MSG_WAITALL);
	cout << Message << "\n";
}



void Client::SendMatchingCancel()
{
	MYCMD cmd;
	cmd.Code = ENUM::MatchingCancle;
	cmd.Size = 0;
	cmd.ClientID = _clientID;

	::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);
}


void Client::BlockCollision()
{
	player.crashOnce = true;

	MYCMD cmd;
	cmd.Code = ENUM::BlockCollision;
	cmd.ClientID = _clientID;

	::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);
	// ��� �浹 ����
}

void Client::BlockCreate()
{
	player.crashOnce = false;
	BlockCreateInfo info;

	info.cur_idx = wall.cur_idx;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			info.color[i][j][0] = wall.getCube(i, j).getColor().x;
			info.color[i][j][1] = wall.getCube(i, j).getColor().y;
			info.color[i][j][2] = wall.getCube(i, j).getColor().z;
		}
	for (int i = 0; i < 36; i++)
		for (int j = 0; j < 3; j++)
			info.random_num[i][j] = wall.random_num[i][j];
	//info.activeBlock = wall.emptyIdx
	MYCMD cmd;
	cmd.Code = ENUM::BlockDataRecv;
	cmd.Size = sizeof(BlockCreateInfo);
	cmd.ClientID = _clientID;
	cmd.IsClientMaster = true;

	::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);
	::send(_connectedSocket, (char*)&info, sizeof(info), 0);
	// ���� ����
}

void Client::BlockCreateReceive()
{
	player.crashOnce = false;
	BlockCreateInfo info;
	::recv(_connectedSocket, (char*)&info, sizeof(BlockCreateInfo), MSG_WAITALL);

	wall.cur_idx = info.cur_idx;

	for (int i=0;i<36;i++)
		for (int j = 0; j < 3; j++)
		wall.random_num[i][j] = info.random_num[i][j];

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			wall.cube[i][j].reset();
			wall.cube[i][j].setPosX(0.3f * j);
			wall.cube[i][j].setPosY(0.3f * i);
			wall.cube[i][j].setColorR(info.color[i][j][0]);
			wall.cube[i][j].setColorG(info.color[i][j][1]);
			wall.cube[i][j].setColorB(info.color[i][j][2]);
			wall.makeWall(i, j, wall.cur_idx);
		}
}

void Client::HpUpdate()
{
	// hp ������Ʈ
	HPInfo info;
	::recv(_connectedSocket, (char*)&info, sizeof(HPInfo), MSG_WAITALL);
	hp = info.hp;

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

void Client::ClientInfoData(int ClientID , ClientInfoPacket cInfo)
{
	ViewerPlayer* pViewer = FindClientPlayer(ClientID);

	updateViewerPosX(pViewer , cInfo.pos_x);
	pViewer->setColorR(cInfo.color_r);
	pViewer->setColorG(cInfo.color_g);
	pViewer->setColorB(cInfo.color_b);
	pViewer->setScaleX(cInfo.scale_x);
	pViewer->setScaleY(cInfo.scale_y);
}

void Client::updateViewerPosX(ViewerPlayer* pViewer , float PosX)
{
	pViewer->setColorR(PosX);
}

void Client::SendPlayerInfo()
{
	MYCMD cmd;
	cmd.Code = ENUM::ClientInfoData;
	cmd.Size = sizeof(ClientInfoPacket);
	cmd.ClientID = _clientID;
	cmd.IsClientMaster = _clientMaster;

	cmd.clientInfoPacket.pos_x = player.getPos().x;
	cmd.clientInfoPacket.color_r = player.getColor().r;
	cmd.clientInfoPacket.color_g = player.getColor().g;
	cmd.clientInfoPacket.color_b = player.getColor().b;
	cmd.clientInfoPacket.scale_x = player.getScale().x;
	cmd.clientInfoPacket.scale_y = player.getScale().y;

	::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);
}

void Client::CreateClientPlayer(int ClientID)
{
	if (viewerPlayer.find(ClientID) == viewerPlayer.end())
	{
		ViewerPlayer* newPlayer = new ViewerPlayer();
		viewerPlayer[ClientID] = newPlayer;
		std::cout << "Created a new client player with ID: " << ClientID << std::endl;
	}
	else
	{
		std::cout << "Client player with ID " << ClientID << " already exists." << std::endl;
	}
}

ViewerPlayer* Client::FindClientPlayer(int ClientID)
{
	auto it = viewerPlayer.find(ClientID);

	if (it != viewerPlayer.end())
	{
		std::cout << "Found client player with ID: " << ClientID << std::endl;
		return it->second;
	}
	else
	{
		std::cout << "Client player with ID " << ClientID << " not found." << std::endl;
		return nullptr;
	}
}

void Client::RemoveClientPlayer(int ClientID)
{
	auto it = viewerPlayer.find(ClientID);
	if (it != viewerPlayer.end())
	{
		delete it->second;
		viewerPlayer.erase(it);
		std::cout << "Removed client player with ID: " << ClientID << std::endl;
	}
	else
	{
		std::cout << "Client player with ID " << ClientID << " does not exist." << std::endl;
	}
}

void Client::DisConnectClient()
{
	if (_connectedSocket != INVALID_SOCKET)
	{
		MYCMD cmd;
		cmd.Code = ENUM::DisconnectClient;
		cmd.Size = 0;
		cmd.ClientID = _clientID;
		cmd.IsClientMaster = _clientMaster;

		::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);

		closesocket(_connectedSocket);
	}
}

void Client::OtherClientIdData(int ClientID, bool isMaster)
{
	if (isMaster)
	{

	}

	RemoveClientPlayer(ClientID);
}


