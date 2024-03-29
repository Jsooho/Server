#include "Sample.h"
GAMECORE(sadas,800,600);

LRESULT  Sample::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 200:
		{
			char buffer[MAX_PATH] = { 0, };
			SendMessageA(m_hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)buffer);
			Packet tPacket(PACKET_CHAT_MSG);
			tPacket  << "��ȣ" <<  buffer;
			m_Net.SendMsg(m_Net.m_Sock, tPacket.m_uPacket);

			SendMessageA(m_hEdit, WM_SETTEXT, 0, (LPARAM)"");

		}break;
		}
	}break;
	}
	return m_Net.MsgProc(hWnd, msg, wParam, lParam);
}



bool Sample::Init()
{
	DWORD style = WS_CHILD | WS_VISIBLE | ES_MULTILINE;
	m_hEdit = CreateWindow(L"edit", NULL, style,
		0, g_rtClient.bottom - 50, 300, 50,
		m_hWnd, (HMENU)100, m_hInstance, NULL);
	style = WS_CHILD | WS_VISIBLE;
	m_hButton = CreateWindow(L"button", L"����", style,
		310, g_rtClient.bottom - 50, 50, 50,
		m_hWnd, (HMENU)200, m_hInstance, NULL);
	m_hListBox = CreateWindow(L"listbox", NULL, style,
		0, 0, 300, g_rtClient.bottom - 70,
		m_hWnd, (HMENU)300, m_hInstance, NULL);

	SendMessageA(m_hListBox, LB_ADDSTRING, 0, (LPARAM)"START CHAT!");

	m_Net.InitNetwork();
	m_Net.Connect(g_hWnd, SOCK_STREAM, 9190, "192.168.0.44");
	return true;

}

bool Sample::Frame()
{
	int Chatcnt = m_Net.m_Player.m_packetPool.size();
	if (Chatcnt > 0 && m_iChatCnt != Chatcnt)
	{
		m_iChatCnt = Chatcnt;
		SendMessage(m_hListBox, LB_RESETCONTENT, 0, 0);

		std::list<Packet>::iterator iter;
		if (m_Net.m_Player.m_packetPool.size() > 20)
		{
			m_Net.m_Player.m_packetPool.pop_front();
		}
		for (iter = m_Net.m_Player.m_packetPool.begin(); iter != m_Net.m_Player.m_packetPool.end(); iter++)
		{
			ChatMsg recvdata;
			ZeroMemory(&recvdata, sizeof(recvdata));
			(*iter)  >> recvdata.name >> recvdata.message;
			SendMessageA(m_hListBox, LB_ADDSTRING, 0, (LPARAM)recvdata.name);
			SendMessageA(m_hListBox, LB_ADDSTRING, 0, (LPARAM)recvdata.message);
			(*iter).Reset();
		}
	}
	return true;
}

bool Sample::Render()
{
	return true;
}

bool Sample::Release()
{
	m_Net.CloseNetwork();
	return true;
}
Sample::Sample()
{

}
Sample::~Sample()
{
}