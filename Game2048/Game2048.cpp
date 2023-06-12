// Game2048.cpp : 定義應用程式的進入點。
//

#include "framework.h"
#include "Game2048.h"
#include "Gaming.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;                                // 目前執行個體
WCHAR szTitle[MAX_LOADSTRING];                  // 標題列文字
WCHAR szWindowClass[MAX_LOADSTRING];            // 主視窗類別名稱
HWND mainWnd;
RECT rect;

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, 
                      _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAME2048, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    rect = RECT{ 0, 0, 90 + 50 * TABLE_SIZE, 90 + 50 * TABLE_SIZE };

    if (!InitInstance (hInstance, nCmdShow))
        return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAME2048));
    MSG msg;

    gaming::initGame();
    InvalidateRect(mainWnd, &rect, true);

    // 主訊息迴圈:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (gaming::isWin())
        {
            MessageBoxW(NULL,
                _T("太爆了阿強"),
                _T("你贏ㄌ"),
                MB_OK);
            gaming::initGame();
            InvalidateRect(mainWnd, &rect, true);
        }

        if (gaming::isLose())
        {
            MessageBoxW(NULL,
                _T("遜ㄟ"),
                _T("你輸ㄌ"),
                MB_OK);
            gaming::initGame();
            InvalidateRect(mainWnd, &rect, true);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAME2048));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GAME2048);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 將執行個體控制代碼儲存在全域變數中

    HWND mainWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rect.right, rect.bottom, nullptr, nullptr, hInstance, nullptr);
    if (!mainWnd)
        return FALSE;
    ShowWindow(mainWnd, nCmdShow);
    UpdateWindow(mainWnd);
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case UP:
                gaming::gameAction(DIR_U);
                break;
            case DOWN:
                gaming::gameAction(DIR_D);
                break;
            case LEFT:
                gaming::gameAction(DIR_L);
                break;
            case RIGHT:
                gaming::gameAction(DIR_R);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
            InvalidateRect(hWnd, &rect, true);
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此新增任何使用 hdc 的繪圖程式碼...
            for (int i = 0; i < TABLE_SIZE; i++)
            {
                for (int j = 0; j < TABLE_SIZE; j++)
                {
                    int val = gaming::getGameTable(i, j);
                    std::string s = val ? std::to_string(val) : "";
                    std::wstring stemp = std::wstring(s.begin(), s.end());
                    LPCWSTR sw = stemp.c_str();
                    POINT pt[5] = { 
                        {35 + j * 50, 23 + i * 50}, 
                        {81 + j * 50, 23 + i * 50},
                        {81 + j * 50, 69 + i * 50},
                        {35 + j * 50, 69 + i * 50},
                        {35 + j * 50, 23 + i * 50} };
                    RECT r = { pt[0].x, pt[0].y, pt[2].x, pt[2].y };
                    COLORREF color =
                        (val <= 1) ? RGB(220, 220, 220) :
                        (val <= 2) ? RGB(255, 255, 230) :
                        (val <= 4) ? RGB(255, 255, 200) :
                        (val <= 8) ? RGB(255, 200, 120) :
                        (val <= 16) ? RGB(255, 180, 80) :
                        (val <= 32) ? RGB(255, 140, 105) :
                        (val <= 64) ? RGB(255, 100, 100) :
                        (val <= 128) ? RGB(255, 255, 80) :
                        (val <= 256) ? RGB(255, 255, 0) :
                        (val <= 1024) ? RGB(255, 235, 0) : RGB(255, 215, 0);
                    
                    FillRect(hdc, &r, CreateSolidBrush(color));
                    Polyline(hdc, pt, 5);
                    SetBkColor(hdc, color);
                    TextOutW(hdc, 55 + j * 50 - _tcslen(sw) * 3, 40 + i * 50, sw, _tcslen(sw));
                }
            }
            std::string s = "Score: " + std::to_string(gaming::getGameScore());
            std::wstring stemp = std::wstring(s.begin(), s.end());
            LPCWSTR sw = stemp.c_str();
            SetBkColor(hdc, RGB(255, 255, 255));
            TextOutW(hdc, 35, 5, sw, _tcslen(sw));
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}