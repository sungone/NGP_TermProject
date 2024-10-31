#include "pch.h"
#include <iostream>
#include <Listener.h>
#include <SessionManager.h>
#include <thread>


using namespace std;





int main()
{
	SessionManager sessionManager;
	Listener listener;
	SOCKET listenSocket = listener.Init();

	SOCKADDR_IN clientaddr = { 0 };
	int nAddrLen = sizeof(clientaddr);
	SOCKET clientSocket;
	 
	while (true)
	{
		clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientaddr, &nAddrLen);
		if (clientSocket == INVALID_SOCKET)
			ErrorHandler("클라이언트 통신 소켓을 생성할 수 없습니다.");

		sessionManager.PrintClinetInfo(clientSocket, "클라이언트가 연결되었습니다.");
		sessionManager.PushClient(clientSocket);

		thread t(&SessionManager::PacketDecode, &sessionManager, clientSocket);
		t.detach();
	}



}