#include "HInput.h"
POINT g_ptMouse;
bool HInput::Init()
{
    ZeroMemory(&m_dwKey, sizeof(DWORD)*256);
    return true;
}

bool HInput::Frame()
{
	//화면좌표계(전체스크린)로 넘어오는 함수
	GetCursorPos(&m_ptMouse);
	//클라이언트(윈도우)좌표계로 변환
	ScreenToClient(g_hWnd, &m_ptMouse);
	g_ptMouse = m_ptMouse;
	//마우스 버튼 VK_LBUTTON, VK_MBUTTON, VK_RBUTTON
	for (int iKey = 0; iKey < 256; iKey++)
	{
		SHORT sKey = GetAsyncKeyState(iKey);
		// 0000 0000 0000 0000
		// 1000 0000 0000 0000
		if (sKey & 0x8000)
		{
			if (m_dwKey[iKey] == KEY_FREE)
			{
				m_dwKey[iKey] = KEY_PUSH;
			}
			else
			{
				m_dwKey[iKey] = KEY_HOLD;
			}
		}
		else
		{
			if (m_dwKey[iKey] == KEY_PUSH ||
				m_dwKey[iKey] == KEY_HOLD)
			{
				m_dwKey[iKey] = KEY_UP;
			}
			else
			{
				m_dwKey[iKey] = KEY_FREE;
			}
		}
	}

	m_dwMouseState[0] = m_dwKey[VK_LBUTTON];
	m_dwMouseState[1] = m_dwKey[VK_RBUTTON];
	m_dwMouseState[2] = m_dwKey[VK_MBUTTON];

	return true;
}

bool HInput::Render()
{
    return true;
}

bool HInput::Release()
{
    return true;
}

DWORD HInput::GetKey(DWORD dwkey)
{

    return m_dwKey[dwkey];
}

HInput::HInput()
{

}

HInput::~HInput()
{
}
