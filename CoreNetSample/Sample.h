#pragma once
#include"HCore.h"
#include"AsyncSelect.h"
class Sample : public HCore
{
public:
	AsyncSelect			m_Net;
	HWND				m_hEdit;
	HWND				m_hButton;
	HWND				m_hListBox;
	int					m_iChatCnt;
public:
	virtual bool Init()    override;
	virtual bool Frame()   override;
	virtual bool Render()  override;
	virtual bool Release() override;
	virtual LRESULT  MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Sample();
	~Sample();
};

