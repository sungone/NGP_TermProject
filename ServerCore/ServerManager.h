#pragma once

/****************************************************************************
*    ServerManager   : Client 들을 관리하기위한 클래스이다  
*    실질적인 모든함수 구현부는 단순화 하기위해서 ServerManager 에서 처리 
*****************************************************************************/

class ServerManager
{
public:
	
	void	PrintClinetInfo(SOCKET socket, string Message);
	void	PushClient(SOCKET socket);
	void	DeleteClient(SOCKET socket);
	int		GetClinetCount();

public:
	//실질적인 함수구현부
	void PacketDecode(SOCKET socket); //Packet의 정보를 확인하여 실질적인 함수부를 실행해줌.
	void SendMessageToAllclinet(SOCKET socket,int size); //채팅
	// 서버에서 랜덤값을 만들어서 클라이언트로 보내는 함수 -> 클라이언트는 이 랜덤값을 가지고 블럭들을 생성함 , 서버에서는 랜덤값만 보냄
	void MakeBlockRandomSeed();

private:
	mutex _mutex;
	concurrent_queue<SOCKET> _listClient;
};

