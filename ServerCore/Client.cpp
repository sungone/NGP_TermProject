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

void Client::PacketDecode()
{
	MYCMD cmd;

	while (::recv(_connectedSocket, (char*)&cmd, sizeof(MYCMD), MSG_WAITALL) > 0)
	{
		switch (cmd.Code) // ��ɺ��� �ؼ��� ���� 
		{
		case MatcingStartReady: //3���� ����
			std::cout << "��Ī �غ� �Ϸ�Ǿ����ϴ�!" << std::endl;
			screen.status = 1;
			break;
		case ClientInfoData:

			break;
		case BlockData:

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
	::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);
}


