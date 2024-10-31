#pragma once


//어떠한 데이터를 보낼것인지.
enum CMDCODE
{
	ClientInfoData = 0, // 클라이언트 위치,색깔,크기 정보
	BlockData, //블록에 대한 정보
	ChattingData, // 채팅에 대한정보
	ExitData // 클라이언트의 종료신호

};

struct MYCMD
{
	int Code;			//명령코드 (CMDCODE)
	int Size;			//실질적인 내용부의 데이터 크기
};

