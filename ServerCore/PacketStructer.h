#pragma once

//어떠한 데이터를 보낼것인지.
enum CMDCODE
{
	Connect =0, //채널에 입장하자 ClientId 부여받음
	ClientInfoData , // 클라이언트 위치,색깔,크기 정보
	BlockData, //블록에 대한 정보
	ChattingData, // 채팅에 대한정보
	MatcingStartReady, //클라이언트에서 Matching 준비완료가 됬다는 신호 / 서버에서 3명이 되서 Matching 이 준비가 완료됬다는 신호
	MatchingCancle, // 클라이언트에서 Matching 을 취소하겠다는 신호를 서버에게 보냄.
	//필요한것을 추가할예정

	DisconnectClient, // 인게임 중 클라이언트가 게임을 종료하였을 때 서버로 종료했다는 걸 보내줌
	OtherClientIdData , // 클라이언트 ID , 마스터 클라이언트 여부 정보 -> 클라가 종료했을 때 나머지 클라에게 정보를 보내기 위함
};

// 플레이어 위치 정보 패킷
struct ClientInfoPacket
{
	float pos_x;

	float color_r;
	float color_g;
	float color_b;

	float scale_x;
	float scale_y;
};

// 블록을 생성할 때 사용하는 랜덤 시드값
struct BlockCreateInfo
{
	int random_seed[36][3];
};


struct MYCMD
{
	int Code = 0;			//명령코드
	int Size = 0;			//질실적인 내용부의 데이터 크기
	int ClientID = 0;
	bool IsClientMaster = false;
	union
	{
		ClientInfoPacket clientInfoPacket;
		BlockCreateInfo blockRamdomSeedPacket;
	};
};





