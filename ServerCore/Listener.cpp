#include "pch.h"
#include "Listener.h"

Listener::~Listener()
{
	::closesocket(_listenSocket);
	::WSACleanup();
	cout << "���� �����մϴ�." << "\n";
}


SOCKET Listener::Init()
{
	WSADATA wsa;

	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		ErrorHandler("���� �ʱ�ȭ ����");
	}

	_listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_listenSocket == INVALID_SOCKET)
	{
		ErrorHandler("listenSocket ���� ����");
	}

	SOCKADDR_IN	serverAddr = { 0 };
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PortNum);
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY); 

	if (::bind(_listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		ErrorHandler("���� ���ε� ����");
	}

	if (::listen(_listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		ErrorHandler("���� ���·� ��ȯ�� �� �����ϴ�.");
	}

	return _listenSocket;
}
