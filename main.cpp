#include "Client.h"
#include <iostream>
#include "DxLib.h"

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    const int WINDOW_WIDTH = 960;
    const int WINDOW_HEIGHT = 640;

    SetWindowText("ƒ^ƒCƒgƒ‹");
    SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);
    ChangeWindowMode(TRUE);

    if (DxLib_Init() == -1)
    {
        return 0;
    }

    SetBackgroundColor(0, 0, 0);
    SetDrawScreen(DX_SCREEN_BACK);


    Client client;


    client.Run();

    client.Close();

    DxLib_End();
    return 0;
}