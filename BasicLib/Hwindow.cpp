#include "Hwindow.h"
RECT g_rtClient;
HWND g_hWnd;
Hwindow* g_pWindow = nullptr;
LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    g_pWindow->MsgProc(hWnd, message, wParam, lParam);
    switch (message)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);// WM_QUIT
    }break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam); // 요거는 꼭 넣어주기!! 확인
    }
    return 0; 
}
LRESULT  Hwindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return 0;
}
bool Hwindow::InitWindows(HINSTANCE hInstance, int nCmdShow, const WCHAR* strWindowTitle, int iX, int iY)
{
    m_hInstance = hInstance;
    WNDCLASSW wc;
    ZeroMemory(&wc, sizeof(WNDCLASSW));
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hInstance = hInstance;
    //운영체제 등록 이름
    wc.lpszClassName = L"SampleWindow";
    //os, exe 메세지를 받고, 처리하는 장소를 지정 하는 함수!!!
    wc.lpfnWndProc = WndProc;
    //레지스터에 등록을 해야 사용할수 있기 때문에 밑에 함수에 들어갈꺼 찾아보고 정의하기!!
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    if (!RegisterClass(&wc))
    {
        return FALSE;
    }
    RECT rt = { 0,0, iX , iY };
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);
    //CreateWindow 버전마다 인자값 다름 확인!!//사용할 함수 잘보고 사용
    m_hWnd = CreateWindow(
        L"SampleWindow",
        strWindowTitle,
        WS_OVERLAPPEDWINDOW, //style = 거의 고정해서 사용, 다른종류 사용해서 확인해보기!!
        0,
        0,
        rt.right - rt.left,
        rt.bottom - rt.top,
        NULL,
        NULL,
        m_hInstance,
        NULL
    );
    if (m_hWnd == NULL)
    {
        return FALSE;
    }
    g_hWnd = m_hWnd;
    GetClientRect(m_hWnd, &m_rtClient);
    GetWindowRect(m_hWnd, &m_rtWindow);
    g_rtClient = m_rtClient;

    ShowWindow(m_hWnd, nCmdShow); //쇼윈도우 함수 두번째 인자 종류 확인!

    return true;
}
//3번: 메세지프로시저 처리!!
//메세지 큐가 끝나면 게임런이 돌아간다 = (이거에 대한 이해!!)
bool Hwindow::WinRun()
{
    MSG msg;
    GameInit();
    while (m_bGameRun)
    {
        if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            m_bGameRun = GameRun();
        }
    }
    GameRelease();
    return true;
}

bool Hwindow::GameInit()
{
    return true;
}

bool Hwindow::GameRun()
{
    return true;
}

bool Hwindow::GameRelease()
{
    return true;
}
Hwindow::Hwindow()
{
    g_pWindow = this;
}

