#pragma once


//어떠한 데이터를 보낼것인지.
enum CMDCODE
{
	ClientInfoData = 0, // 클라이언트 위치,색깔,크기 정보
	BlockData, //블록에 대한 정보
	ChattingData, // 채팅에 대한정보
	MatcingStartReady, //클라이언트에서 Matching 준비완료가 됬다는 신호 / 서버에서 3명이 되서 Matching 이 준비가 완료됬다는 신호
	MatchingCancle // 클라이언트에서 Matching 을 취소하겠다는 신호를 서버에게 보냄.
};

struct MYCMD
{
	int Code;			//명령코드
	int Size;			//실질적인 내용부의 데이터 크기
};

struct Chat
{
	char message[256];
};
