#include "pch.h"
#include "Client.h"

Client::~Client()
{
	::closesocket(_connectedSocket);
	::Sleep(100);
	::WSACleanup();
}

void Client::Init()
{
	WSADATA wsa = { 0 };
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		ErrorHandler("ERROR: 윈속을 초기화 할 수 없습니다.");
	}

	//서버에 연결할 소켓 생성
	_connectedSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_connectedSocket == INVALID_SOCKET)
	{
		ErrorHandler("ERROR: 소켓을 생성할 수 없습니다.");

	}

	//포트 바인딩 및 연결
	SOCKADDR_IN	svraddr = { 0 };
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(PortNum);
	::inet_pton(AF_INET, IPAddress, &svraddr.sin_addr);

	if (::connect(_connectedSocket, (SOCKADDR*)&svraddr, sizeof(svraddr)) == SOCKET_ERROR)
	{
		ErrorHandler("ERROR: 서버에 연결할 수 없습니다.");
	}

}

void Client::PacketDecode()
{
	MYCMD cmd;
	 
	while (::recv(_connectedSocket, (char*)&cmd, sizeof(MYCMD), MSG_WAITALL) > 0)
	{
		cout << cmd.Code << "\n";

		switch (cmd.Code) // 명령부의 해석에 따라 
		{
		case ClientInfoData:

			break;
		case BlockData:

			break;
		case ChattingData:
			RecvMessageFromServer();
			break;
		default:
			ErrorHandler("알수없는 명령어 수신했습니다.");
			break;
		}

	}

	puts("수신 스레드가 끝났습니다.");

}

void Client::RecvMessageFromServer()
{
	char Message[256] = { 0 };
	int len = ::recv(_connectedSocket, Message, sizeof(Message), MSG_WAITALL);
	cout << Message << "\n";
}

void Client::SendMessageToAllclinet()
{
	while (1)
	{
		char Message[256] = { 0 };

		memset(Message, 0, sizeof(Message));
		gets_s(Message);

		MYCMD cmd;

		cmd.Code = ChattingData;
		cmd.Size = 256;

		//명령어 입력.
		::send(_connectedSocket, (char*)&cmd, sizeof(cmd), 0);

		//사용자가 입력한 문자열을 서버에 전송한다.
		::send(_connectedSocket, Message, sizeof(Message), 0);
	}
}
