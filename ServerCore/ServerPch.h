#pragma once

#include <iostream>
#include <List>
#include <map>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <string>
#include <functional>
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <windows.h>
#include <concurrent_queue.h>
#include "ErrorHandler.h"
#include "PacketStructer.h"
#include "GlobalMacro.h"

using namespace std;
using namespace concurrency;

/******************************************
			<Screen Status>
 0 : 메인 화면 1 : 인게임 스크린 (HP 100) 2 : 게임 승리 3 : 게임 오버
 4 : 인게임 스크린 (HP 66) 5 : 인게임 스크린 (HP 33) 6 : 매칭 화면
******************************************/

namespace E
{
	enum ScreenState
	{
		Main,
		HP100,
		WIN,
		GAMEOVER,
		HP66,
		HP33,
		MATCHING
	};
}
