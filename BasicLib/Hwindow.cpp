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
        return DefWindowProc(hWnd, message, wParam, lParam); // ��Ŵ� �� �־��ֱ�!! Ȯ��
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
    //�ü�� ��� �̸�
    wc.lpszClassName = L"SampleWindow";
    //os, exe �޼����� �ް�, ó���ϴ� ��Ҹ� ���� �ϴ� �Լ�!!!
    wc.lpfnWndProc = WndProc;
    //�������Ϳ� ����� �ؾ� ����Ҽ� �ֱ� ������ �ؿ� �Լ��� ���� ã�ƺ��� �����ϱ�!!
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    if (!RegisterClass(&wc))
    {
        return FALSE;
    }
    RECT rt = { 0,0, iX , iY };
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);
    //CreateWindow �������� ���ڰ� �ٸ� Ȯ��!!//����� �Լ� �ߺ��� ���
    m_hWnd = CreateWindow(
        L"SampleWindow",
        strWindowTitle,
        WS_OVERLAPPEDWINDOW, //style = ���� �����ؼ� ���, �ٸ����� ����ؼ� Ȯ���غ���!!
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

    ShowWindow(m_hWnd, nCmdShow); //�������� �Լ� �ι�° ���� ���� Ȯ��!

    return true;
}
//3��: �޼������ν��� ó��!!
//�޼��� ť�� ������ ���ӷ��� ���ư��� = (�̰ſ� ���� ����!!)
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

