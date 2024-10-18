#pragma once



#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...

#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <deque>
#include <fstream>
#include <list>
#include <memory>
#include <ranges>
#include <functional>
#include <string>
#include <string_view>
#include <iterator>
#include <numbers>
#include <numeric>
#include <thread>

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

// ���� �Լ� ���� ��� �� ����
inline void err_quit(const char* msg)
{
	void* lpMsgBuf;
	FormatMessageA( //A = Ascii
		FORMAT_MESSAGE_ALLOCATE_BUFFER // ���� �ڵ带 �ʰ� �о��
		| FORMAT_MESSAGE_FROM_SYSTEM, // �׸��� �װ� �ý��ۿ� �־�
		NULL/* ��� ã���� �����ʿ� X*/, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),// MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT) ������ �ü�� �⺻ ���� �������� ������.
		(char*)&lpMsgBuf/*������ ���� �޸�*/, 0/*ũ�⵵ �ڵ� ����*/, NULL/*�ƱԸ�Ʈ�� �ڵ� ����*/);
	//lpMsgBuf�޸� �ѱ��
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1); // ������ �ɰ��ϸ� ������
}


// ���� �Լ� ���� ���
inline void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// ���� �Լ� ���� ���
inline void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[����] %s\n", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}











inline void clearConsole()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

inline bool SocketCheck(int code)
{
	if (code < 0) {
		std::cout << "Socket Error\n";
		exit(-1);
	}
	return code >= 0;
}