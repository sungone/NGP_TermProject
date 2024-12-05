#pragma once

class Listener
{

public:
	Listener() = default;
	~Listener();

	SOCKET Init();
	
private:
	SOCKET _listenSocket;

};

