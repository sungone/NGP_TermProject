#include "pch.h"
#include "Client.h"
#include "ThreadManager.h"


int main()
{
	Client client;

	client.Init();

	cout << "ä���� �����մϴ�. �޼����� �Է����ּ���." << "\n";

	ThreadManager::Launch([&client]()
		{
			client.PacketDecode();
		});

	//���ξ����忡�� ���� ���� / ���ӷ����� ���� ���� �������ϳ������� �����
	client.SendMessageToAllclinet();

	return 0;
}
