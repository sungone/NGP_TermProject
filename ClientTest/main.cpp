#include "pch.h"
#include "Client.h"
#include "ThreadManager.h"

void ThreadFunc(SOCKET hSocket)
{
	char szBuffer[256] = { 0 };
	
	while (::recv(hSocket, szBuffer, sizeof(szBuffer), MSG_WAITALL) > 0)
	{
		printf("-> %s\n", szBuffer);
		memset(szBuffer, 0, sizeof(szBuffer));
	}

	puts("수신 스레드가 끝났습니다.");
}

int main()
{
	Client client;

	client.Init();

	cout << "채팅을 시작합니다. 메세지를 입력해주세요." << "\n";

	ThreadManager::Launch([&client]()
		{
			client.SendMessageToAllclinet();
		});

	ThreadManager::Launch([&client]()
		{
			client.PacketDecode();
		});

	while (1)
	{

	}

	return 0;
}
