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

	//int nOpt = 1;

	//if (::setsockopt(_listenSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&nOpt, sizeof(nOpt)) == SOCKET_ERROR)
	//{
	//	ErrorHandler("네이글 알고리즘 OFF 실패 .");
	//}


	// 바인딩 전에 IP주소와 포트를 재사용하도록 소켓 옵션을 변경한다.
	bool option = TRUE;
	if (::setsockopt(_listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&option, sizeof(bool)) == SOCKET_ERROR)
	{
		ErrorHandler("SO_REUSEADDR 실패");
	}

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
