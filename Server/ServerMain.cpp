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
			ErrorHandler("Ŭ���̾�Ʈ ��� ������ ������ �� �����ϴ�.");

		cout << "Ŭ���̾�Ʈ�� ����Ǿ����ϴ�." << "\n";
		sessionManager.PrintClinetInfo(clientSocket);
		sessionManager.PushClient(clientSocket);

		thread t(&SessionManager::PacketDecode, &sessionManager, clientSocket);
		t.detach();
	}



}