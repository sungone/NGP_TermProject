#include "pch.h"
#include "Client.h"

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
		cout << cmd.Code << "\n";

		switch (cmd.Code) // ��ɺ��� �ؼ��� ���� 
		{
		case ClientInfoData:

			break;
		case BlockData:

			break;
		case ChattingData:
			RecvMessageFromServer();
			break;
		default:
			ErrorHandler("�˼����� ��ɾ� �����߽��ϴ�.");
			break;
		}

	}

	puts("���� �����尡 �������ϴ�.");

}

void Client::RecvMessageFromServer()
{
	char Message[256] = { 0 };
	int len = ::recv(_connectedSocket, Message, sizeof(Message), MSG_WAITALL);
	cout << Message << "\n";
}

void Client::SendMessageToAllclinet()
{
	while (1)
	{
		char Message[256] = { 0 };

		memset(Message, 0, sizeof(Message));
		gets_s(Message);

		MYCMD cmd;

		cmd.Code = ChattingData;
		cmd.Size = 256;

		//��ɾ� �Է�.
		::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);

		//����ڰ� �Է��� ���ڿ��� ������ �����Ѵ�.
		::send(_connectedSocket, Message, sizeof(Message), 0);
	}
}
