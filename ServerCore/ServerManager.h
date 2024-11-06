#pragma once

/****************************************************************************
*    ServerManager   : Client 들을 관리하기위한 클래스이다  
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
	/* 아래는 구현해야함*/
	void BlockCollision(); //피검사를 해서 GameOver 시키거나 hp를 깍음.
	void MakeBlockSend(SOCKET socket); //블록생성해서 보내줌
	void PlayerInfo(SOCKET socket); //플레이어의 정보를 받아서 브로드캐스팅방식으로 쏴줌.
	void Restart(SOCKET socket);  // 재시작
	void ReturnMenu(SOCKET socket); //연결을 끊고 다시 방매칭 할수있게해줌

private:
	mutex _mutex;
	concurrent_queue<SOCKET> _listClient;
	atomic<int> _readyCount = 0;
	atomic<int> IDGenator =1;
	atomic<int> _hp = 3;
};

