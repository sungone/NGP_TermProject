#pragma once

/****************************************************************************
*    SessionManager   : Client 들을 관리하기위한 클래스이다  
*    실질적인 모든함수 구현부는 단순화 하기위해선 SessionManager 에서 처리 
*****************************************************************************/

class SessionManager
{
public:
<<<<<<< Updated upstream
	void	PrintClinetInfo(SOCKET socket);
	void	PushClient(SOCKET socket);
	void	DeleteClient(SOCKET socket);
	int		GetClinetCount() { return _listClient.size(); }
=======
	void	PrintClinetInfo(SOCKET socket, string Message);
	void	PushClient(SOCKET socket);
	void	DeleteClient(SOCKET socket);
	int		GetClinetCount();
>>>>>>> Stashed changes

public:
	//실질적인 함수구현부

	void PacketDecode(SOCKET socket); //Packet의 정보를 확인하여 실질적인 함수부를 실행해줌.
	void SendMessageToAllclinet(SOCKET socket); //채팅


private:
	mutex _queueLock; 
	concurrent_queue<SOCKET> _clientQueue; 
};

