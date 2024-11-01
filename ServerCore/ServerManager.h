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
	void ConnectClient(SOCKET socket); //요청을 받아 ID 부여해줌
	void MatchingAccept(SOCKET socket); // Matching 요청 받아서 처리해줌.
	void MathcingOff(SOCKET socket); //MachingCancle 요청 받아서 처리해줌
	void SendMessageToAllclinet(SOCKET socket,int size); //채팅
	void MakeBlockRandomSeed(); // 서버에서 랜덤값을 만들어서 클라이언트로 보내는 함수 -> 클라이언트는 이 랜덤값을 가지고 블럭들을 생성함 , 서버에서는 랜덤값만 보냄

private:
	mutex _mutex;
	concurrent_queue<SOCKET> _listClient;

	atomic<int> _readyCount = 0;
	atomic<int> IDGenator =1;
};

