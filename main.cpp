#define WIN32_LEAN_AND_MEAN
#include "DxLib.h"
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <vector>

#pragma comment(lib,"ws2_32.lib")
using namespace std;


struct CIRCLE
{
	int centerX;
	int centerY;
	int radius;
	int color;
};

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 640;

vector<CIRCLE> Circle;

const char* SERVER_IPADDRESS = "192.168.42.148";
const unsigned short SERVER_PORT = 8888;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// WinSock2.2　初期化
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return -1;
	}

	// ソケットの作成
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		return -1;
	}

	// ノンブロッキングモード設定
	unsigned long arg = 0x01;
	ioctlsocket(sock, FIONBIO, &arg);

	// サーバアドレス設定
	SOCKADDR_IN serverSockAddress;
	memset(&serverSockAddress, 0, sizeof(serverSockAddress));
	serverSockAddress.sin_family = AF_INET;
	serverSockAddress.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_IPADDRESS, &serverSockAddress.sin_addr.s_addr);

	if (connect(sock, (SOCKADDR*)&serverSockAddress, sizeof(serverSockAddress)) == SOCKET_ERROR)
	{

	}

	// DxLib初期化まわり
	SetWindowText("タイトル");
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)
	{
		return 0;
	}

	SetBackgroundColor(0, 0, 0);
	SetDrawScreen(DX_SCREEN_BACK);

	// 自分の円
	CIRCLE myCircle = { 0,0,100,GetColor(255,0,0) };

	// メインループ
	while (TRUE)
	{
		ClearDrawScreen();

		// マウス位置を取得して更新
		GetMousePoint(&myCircle.centerX, &myCircle.centerY);

		// 円の描画
		//DrawCircle(myCircle.centerX, myCircle.centerY, myCircle.radius, myCircle.color, TRUE);

		// サーバに送信する構造体をネットワークバイトオーダーで作成
		CIRCLE sendData;
		sendData.centerX = htonl(myCircle.centerX);
		sendData.centerY = htonl(myCircle.centerY);
		sendData.radius = htonl(myCircle.radius);
		sendData.color = htonl(myCircle.color);

		// 送信
		send(sock, (char*)&sendData, sizeof(sendData), 0);

		CIRCLE buff[256];
		int ret = recv(sock, (char*)buff, sizeof(buff), 0);

		if (ret > 0)
		{
			Circle.clear();
			int count = ret / sizeof(CIRCLE);

			for (int i = 0; i < count; i++)
			{
				CIRCLE recvData;
				recvData.centerX = ntohl(buff[i].centerX);
				recvData.centerY = ntohl(buff[i].centerY);
				recvData.radius = ntohl(buff[i].radius);
				recvData.color = ntohl(buff[i].color);
				Circle.push_back(recvData);

				//DrawCircle(recvData.centerX, recvData.centerY, recvData.radius, recvData.color, TRUE);
			}
		}

		for (int i = 0; i < Circle.size(); i++)
		{
			CIRCLE& recv = Circle[i];
			DrawCircle(recv.centerX, recv.centerY, recv.radius, recv.color, TRUE);
		}

		for (int i = 1; i <= 4; i++)
		{
			for (int j = 1; j <= 4; j++)
			{
				DrawLine(100, 100 * j, 400, 100 * j, GetColor(0, 0, 255));
				DrawLine(100 * i, 100, 100 * i, 400, GetColor(0, 255, 255));

			}
		}
		// 描画更新と待機
		ScreenFlip();
		Sleep(16);

		if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}
	}

	// ソケット破棄
	closesocket(sock);
	WSACleanup();

	// DxLib終了
	DxLib_End();
	return 0;

}