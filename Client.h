#pragma once


#include <WinSock2.h>
#include <ws2tcpip.h>

class Client
{
private:
	SOCKET sock;
	int board[3][3];

public:
	bool Init();
	void Run();
	void Close();
};
