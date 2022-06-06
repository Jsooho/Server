#include "Hwindow.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN: //키보드 메시지 사용
		if (wParam == 'F')
		{
			SetWindowText(hWnd, L"good"); // 윈도우 이름바꾸기
		}break;
	case WM_KEYUP:
		if (wParam == 'F')
		{
			SetWindowTextW(hWnd, L"wtf");
		}
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
bool Hwindow::InitWindow(HINSTANCE hInstance, int nCmdShow, const WCHAR* strwindowtitle)
{
    m_hInstance = hInstance;
    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEXW));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);//커서
    wcex.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));//바탕화면색
    wcex.lpszClassName = L"JSH";//이름
    if (!RegisterClassExW(&wcex))
    {
        return false;
    }
	
    m_hWnd = CreateWindowExW(
        0,
        L"JSH",
        strwindowtitle,
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        1024,
        768,
        NULL,
        NULL,
        m_hInstance,
        NULL
    );
    if (m_hWnd == NULL)
    {
        return false;
    }

    return true;
}
