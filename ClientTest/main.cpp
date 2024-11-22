#include "pch.h"
#include "Client.h"
#include "ThreadManager.h"

int main()
{
	Client client;

	client.Init();

	cout << "채팅을 시작합니다. 메세지를 입력해주세요." << "\n";

	ThreadManager::Launch([&client]()
		{
			client.PacketDecode();
		});

	//메인쓰레드에서 수신 할지 / 게임루프랑 따로 뺴서 쓰레드하나더뺄지 고민중
	client.SendChattingData();

	return 0;
}
