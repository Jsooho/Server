#pragma once
#include"HDevice.h"
#include "HInput.h"
#include "HTimer.h"
#include "HDxWrite.h"
#include "DxObject.h"
#include "HDxState.h"
class HCore : public HDevice
{
public:
	bool		m_bDebugText = false;
	HDxWrite	m_DxWrite;
	HTimer		m_Timer;
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
public:
	bool GameRun();
public:
	bool GameInit();
	bool GameFrame();
	bool GameRender();
	bool GameRelease();
};

