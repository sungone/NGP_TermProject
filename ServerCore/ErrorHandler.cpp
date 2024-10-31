#include "pch.h"
#include "ErrorHandler.h"

void ErrorHandler(const char* Message)
{
	cout << Message << WSAGetLastError() << endl;
	::WSACleanup();
	exit(1);
}
