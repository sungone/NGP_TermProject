
#include "pch.h"
#include "Client.h"
#include "ClientData.h"

Client::Client()
{
	int a = 5;
}

Client::~Client()
{
	//DisConnectClient();
	//::closesocket(_connectedSocket);
	//::Sleep(100);
	//::WSACleanup();
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
		case ENUM::DisConnectClientInfo:
			DisConnectClientInfo(cmd.ClientID, cmd.IsClientMaster);
			break;
		case ENUM::ClientInfoData:
			ClientInfoData(cmd);
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

	cout << "�����κ��� " << _clientID << " �ο� ����" << "\n";
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
	screen.ChangeScene(E::HP100);
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

void Client::InitViewerPlayer(int MyClientID)
{
	for (int i = 0; i < 3; ++i)
	{
		CreateClientPlayer(i + 1);
	}

	RemoveClientPlayer(MyClientID);
}

void Client::BlockCollision()
{
	//player.crashOnce = true;

	MYCMD cmd;
	cmd.Code = ENUM::BlockCollision;
	cmd.ClientID = _clientID;

	::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);
	// ��� �浹 ����
}

void Client::BlockCreate()
{
	//player.crashOnce = false;
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
	wall.emptyIdx.clear();
	BlockCreateInfo info;
	::recv(_connectedSocket, (char*)&info, sizeof(BlockCreateInfo), MSG_WAITALL);

	wall.cur_idx = info.cur_idx;

	for (int i = 0; i < 36; i++)
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
}

void Client::ClientInfoData(MYCMD& cmd)
{

	ClientInfoPacket cInfo;

	::recv(_connectedSocket, (char*)&cInfo, sizeof(ClientInfoPacket), MSG_WAITALL);

	ViewerPlayer* pViewer = FindClientPlayer(cmd.ClientID);
	updateViewerPosX(pViewer, cInfo.pos_x);
	updateViewerColor(pViewer, cInfo.color_r, cInfo.color_g, cInfo.color_b);
	updateViewerScale(pViewer, cInfo.scale_x, cInfo.scale_y);

	cout << "Ŭ���̾�Ʈ " << cmd.ClientID << " �� ��ġ������ ����" << '\n';
}

void Client::updateViewerPosX(ViewerPlayer* pViewer, float PosX)
{
	pViewer->setPosX(PosX);
}

void Client::updateViewerColor(ViewerPlayer* pViewer, float r, float g, float b)
{
	pViewer->setColorR(r);
	pViewer->setColorG(g);
	pViewer->setColorB(b);
}

void Client::updateViewerScale(ViewerPlayer* pViewer, float x, float y)
{
	pViewer->setScaleX(x);
	pViewer->setScaleY(y);
}

void Client::SendPlayerInfo()
{
	MYCMD cmd;
	cmd.Code = ENUM::ClientInfoData;
	cmd.Size = sizeof(ClientInfoPacket);
	cmd.ClientID = _clientID;
	cmd.IsClientMaster = _clientMaster;

	::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);

	ClientInfoPacket Packet;

	Packet.pos_x = player.getPos().x;
	Packet.color_r = player.getColor().r;
	Packet.color_g = player.getColor().g;
	Packet.color_b = player.getColor().b;
	Packet.scale_x = player.getScale().x;
	Packet.scale_y = player.getScale().y;

	::send(_connectedSocket, (char*)&Packet, sizeof(Packet), 0);
}

void Client::CreateClientPlayer(int ClientID)
{
	if (viewerPlayer.find(ClientID) == viewerPlayer.end())
	{
		ViewerPlayer* newPlayer = new ViewerPlayer();
		viewerPlayer[ClientID] = newPlayer;
	}
}

ViewerPlayer* Client::FindClientPlayer(int ClientID)
{
	auto it = viewerPlayer.find(ClientID);

	if (it != viewerPlayer.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

void Client::RemoveClientPlayer(int ClientID)
{
	auto it = viewerPlayer.find(ClientID);
	if (it != viewerPlayer.end())
	{
		it->second->isRender = false;
		viewerPlayer.erase(it);
	}
}

void Client::GameOver()
{
	MYCMD cmd;
	cmd.Code = ENUM::GAMEOVER;
	cmd.Size = 0;
	cmd.ClientID = _clientID;
	::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);
}

void Client::DisConnectClient()
{

	MYCMD cmd;
	cmd.Code = ENUM::DisconnectClient;
	cmd.Size = 0;
	cmd.ClientID = _clientID;
	cmd.IsClientMaster = _clientMaster;

	::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);

};

void Client::DisConnectClientInfo(int ClientID, bool isMaster)
{
	std::cout << ClientID << " ���� ����\n";

	RemoveClientPlayer(ClientID);

	if (isMaster)
	{
		if (viewerPlayer.size() != 0)
		{
			if ((viewerPlayer.begin()->first) > client._clientID)
			{
				client._clientMaster = true;
				cout << client._clientID << "���� ���� ������Ŭ���̾�Ʈ�� ���ӹ޾Ҵ�" << endl;
			}
		}
		else
		{
			client._clientMaster = true;
			cout << client._clientID << "���� ���� ������Ŭ���̾�Ʈ�� ���ӹ޾Ҵ�" << endl;
		}
	}
}


