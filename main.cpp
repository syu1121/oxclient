#include "Client.h"
#include <iostream>
#include "DxLib.h"

using namespace std;

int main()
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

    if (!client.Init("127.0.0.1", 49152))
    {
        cout << "Ú‘±Ž¸”s" << endl;
        return -1;
    }

    client.Run();

    client.Close();

    DxLib_End();
    return 0;
}