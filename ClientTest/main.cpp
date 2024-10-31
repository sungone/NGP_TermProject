#include "pch.h"

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
	//윈속 초기화
	WSADATA wsa = { 0 };
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		puts("ERROR: 윈속을 초기화 할 수 없습니다.");
		return 0;
	}

	//서버에 연결할 소켓 생성
	SOCKET hSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		puts("ERROR: 소켓을 생성할 수 없습니다.");
		return 0;
	}

	//포트 바인딩 및 연결
	SOCKADDR_IN	svraddr = { 0 };
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(PortNum);
	::inet_pton(AF_INET, IPAddress, &svraddr.sin_addr);

	if (::connect(hSocket,(SOCKADDR*)&svraddr, sizeof(svraddr)) == SOCKET_ERROR)
	{
		puts("ERROR: 서버에 연결할 수 없습니다.");
		return 0;
	}

	
	MYCMD cmd;
	cmd.Code = ChattingData;
	cmd.Size = 256;

	//채팅 메시지 송신
	char szBuffer[256];
	puts("채팅을 시작합니다. 메시지를 입력하세요.");

	thread t1(ThreadFunc,hSocket);
	t1.detach();

	while (1)
	{
		memset(szBuffer, 0, sizeof(szBuffer));
		gets_s(szBuffer);
		if (strcmp(szBuffer, "EXIT") == 0)	
			break;

		//명령어 입력.
		::send(hSocket, (char*)&cmd,sizeof(cmd), 0);

		//사용자가 입력한 문자열을 서버에 전송한다.
		::send(hSocket, szBuffer, strlen(szBuffer) + 1, 0);

	}


	::closesocket(hSocket);
	::Sleep(100);
	::WSACleanup();
	return 0;
}
