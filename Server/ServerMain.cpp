#include "pch.h"
#include <iostream>
#include "TestClass.h"

using namespace std;

int main()
{
	TestClass a;
	a.TestFunc();

	WSADATA wsa = { 0 };
	::WSAStartup(MAKEWORD(2, 2), &wsa);

	WSACleanup();
}