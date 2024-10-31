#pragma once

#include <iostream>
#include <List>
#include <map>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <string>

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <concurrent_queue.h>
#pragma comment(lib, "ws2_32.lib")

#include <windows.h>
#include <assert.h>

#include "ErrorHandler.h"
#include "PacketStructer.h"
#include "GlobalMacro.h"
using namespace std;
using namespace concurrency; // Microsoft concurrent_queue 네임스페이스

