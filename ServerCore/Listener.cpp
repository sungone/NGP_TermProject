#include "pch.h"
#include "Listener.h"

Listener::~Listener()
{
	::closesocket(_listenSocket);
	::WSACleanup();
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

	//int nOpt = 1;

	//if (::setsockopt(_listenSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&nOpt, sizeof(nOpt)) == SOCKET_ERROR)
	//{
	//	ErrorHandler("���̱� �˰��� OFF ���� .");
	//}


	// ���ε� ���� IP�ּҿ� ��Ʈ�� �����ϵ��� ���� �ɼ��� �����Ѵ�.
	bool option = TRUE;
	if (::setsockopt(_listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&option, sizeof(bool)) == SOCKET_ERROR)
	{
		ErrorHandler("SO_REUSEADDR ����");
	}

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
