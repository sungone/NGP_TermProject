#include "pch.h"
#include "Listener.h"

Listener::~Listener()
{
	::closesocket(_listenSocket);
	::WSACleanup();
	cout << "서버 종료합니다." << "\n";
}


SOCKET Listener::Init()
{
	WSADATA wsa;

	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		ErrorHandler("윈속 초기화 실패");
	}

	_listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_listenSocket == INVALID_SOCKET)
	{
		ErrorHandler("listenSocket 생성 실패");
	}

	SOCKADDR_IN	serverAddr = { 0 };
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PortNum);
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY); 

	if (::bind(_listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		ErrorHandler("서버 바인드 실패");
	}

	if (::listen(_listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		ErrorHandler("리슨 상태로 전환할 수 없습니다.");
	}

	return _listenSocket;
}
