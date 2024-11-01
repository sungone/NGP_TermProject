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

	puts("���� �����尡 �������ϴ�.");
}

int main()
{
	Client client;

	client.Init();

	cout << "ä���� �����մϴ�. �޼����� �Է����ּ���." << "\n";

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
