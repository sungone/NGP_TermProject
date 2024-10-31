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

#include "ErrorHandler.h"
#include "PacketStructer.h"
#include "GlobalMacro.h"
using namespace std;

