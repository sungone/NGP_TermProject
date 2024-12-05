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
 0 : ���� ȭ�� 1 : �ΰ��� ��ũ�� (HP 100) 2 : ���� �¸� 3 : ���� ����
 4 : �ΰ��� ��ũ�� (HP 66) 5 : �ΰ��� ��ũ�� (HP 33) 6 : ��Ī ȭ��
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
