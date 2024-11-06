#pragma once


//어떠한 데이터를 보낼것인지.
enum CMDCODE
{
	Connect =0, //채널에 입장하자 ClientId 부여받음
	ClientInfoData , // 클라이언트 위치,색깔,크기 정보
	BlockData, //블록에 대한 정보
	ChattingData, // 채팅에 대한정보
	MatcingStartReady, //클라이언트에서 Matching 준비완료가 됬다는 신호 / 서버에서 3명이 되서 Matching 이 준비가 완료됬다는 신호
	MatchingCancle // 클라이언트에서 Matching 을 취소하겠다는 신호를 서버에게 보냄.
	
	//필요한것을 추가할예정
};

struct MYCMD
{
	int Code=0;			//명령코드
	int Size=0;			//질실적인 내용부의 데이터 크기
	int ClientID = 0;
};


