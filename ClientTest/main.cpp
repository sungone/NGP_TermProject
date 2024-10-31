#include "pch.h"

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
	//���� �ʱ�ȭ
	WSADATA wsa = { 0 };
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		puts("ERROR: ������ �ʱ�ȭ �� �� �����ϴ�.");
		return 0;
	}

	//������ ������ ���� ����
	SOCKET hSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		puts("ERROR: ������ ������ �� �����ϴ�.");
		return 0;
	}

	//��Ʈ ���ε� �� ����
	SOCKADDR_IN	svraddr = { 0 };
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(PortNum);
	::inet_pton(AF_INET, IPAddress, &svraddr.sin_addr);

	if (::connect(hSocket,(SOCKADDR*)&svraddr, sizeof(svraddr)) == SOCKET_ERROR)
	{
		puts("ERROR: ������ ������ �� �����ϴ�.");
		return 0;
	}

	
	MYCMD cmd;
	cmd.Code = ChattingData;
	cmd.Size = 256;

	//ä�� �޽��� �۽�
	char szBuffer[256];
	puts("ä���� �����մϴ�. �޽����� �Է��ϼ���.");

	thread t1(ThreadFunc,hSocket);
	t1.detach();

	while (1)
	{
		memset(szBuffer, 0, sizeof(szBuffer));
		gets_s(szBuffer);
		if (strcmp(szBuffer, "EXIT") == 0)	
			break;

		//��ɾ� �Է�.
		::send(hSocket, (char*)&cmd,sizeof(cmd), 0);

		//����ڰ� �Է��� ���ڿ��� ������ �����Ѵ�.
		::send(hSocket, szBuffer, strlen(szBuffer) + 1, 0);

	}


	::closesocket(hSocket);
	::Sleep(100);
	::WSACleanup();
	return 0;
}
