#include "HInput.h"
POINT g_ptMouse;
bool HInput::Init()
{
    ZeroMemory(&m_dwKey, sizeof(DWORD)*256);
    return true;
}

bool HInput::Frame()
{
	//ȭ����ǥ��(��ü��ũ��)�� �Ѿ���� �Լ�
	GetCursorPos(&m_ptMouse);
	//Ŭ���̾�Ʈ(������)��ǥ��� ��ȯ
	ScreenToClient(g_hWnd, &m_ptMouse);
	g_ptMouse = m_ptMouse;
	//���콺 ��ư VK_LBUTTON, VK_MBUTTON, VK_RBUTTON
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
