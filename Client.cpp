#define WIN32_LEAN_AND_MEAN

#include "Client.h"
#include "DxLib.h"
#include <iostream>

#pragma comment(lib,"ws2_32.lib")


const char* SERVER_IPADDRESS = "192.168.42.148";
const unsigned short SERVER_PORT = 8888;

bool Client::Init(const char* ip, int port)
{
	// WinSock2.2@‰Šú‰»
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return false;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		return false;
	}

	sockaddr_in addr;
	memset(board, 0, sizeof(board));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	//addr.sin_addr.S_un.S_addr = inet_addr(ip);
	inet_pton(AF_INET, SERVER_IPADDRESS, &addr.sin_addr.s_addr);
	//inet_pton(AF_INET, SERVER_IPADDRESS, &serverSockAddress.sin_addr.s_addr);
	if (connect(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		return false;
	}

	

	return true;
}

void Client::Run()
{
	while (ProcessMessage() == 0)
	{
		ClearDrawScreen();

		int mouse = GetMouseInput();
		static int prevMouse = 0;

		if ((mouse & MOUSE_INPUT_LEFT) && !prevMouse)
		{
			int mx;
			int my;
			GetMousePoint(&mx, &my);

			if (mx >= 100 && mx < 400 && my >= 100 && my < 400)
			{
				int x = (mx - 100) / 100;
				int y = (my - 100) / 100;

				char sendBuff[32];
				sprintf_s(sendBuff, "%d %d", x, y);
				send(sock, sendBuff, strlen(sendBuff), 0);
			}
		}

		prevMouse = mouse & MOUSE_INPUT_LEFT;

		char recvBuff[256];
		int ret = recv(sock, recvBuff, sizeof(recvBuff) - 1, 0);

		if (ret > 0)
		{
			recvBuff[ret] = '\0';

			int index = 0;
			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{
					board[y][x] = recvBuff[index++] - '0';
				}
			}
		}

		for (int i = 1; i < 3; i++)
		{
			DrawLine(100, 100 * i + 100, 400, 100 * i + 100, GetColor(255, 255, 255));
			DrawLine(100 * i + 100, 100, 100 * i + 100, 400, GetColor(255, 255, 255));
		}

		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				int dx = 150 + x * 100;
				int dy = 150 + y * 100;

				if (board[y][x] == 1)
				{
					DrawCircle(dx, dy, 30, GetColor(255, 0, 0), FALSE);
				}
				else if (board[y][x] == 2)
				{
					
					DrawCircle(dx, dy, 30, GetColor(0, 255, 0), TRUE);
				}
			}
		}

		ScreenFlip();
	}
}

void Client::Close()
{
	shutdown(sock, SD_BOTH);
	closesocket(sock);
	WSACleanup();
}
