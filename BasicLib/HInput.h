#pragma once
#include "Std.h"

enum KeyState
{
	KEY_FREE = 0,
	KEY_UP,
	KEY_HOLD,
	KEY_PUSH,
};

class HInput
{
public:
	DWORD m_dwKey[256];
	POINT m_ptMouse;
	DWORD m_dwMouseState[3];

	static HInput& Get()
	{
		static HInput theSingle;
		return theSingle;
	}
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	DWORD GetKey(DWORD dwkey);
private:
	HInput();
public:
	virtual ~HInput();
};

