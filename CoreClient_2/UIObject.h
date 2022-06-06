#pragma once
#include"Object2D.h"
class UIObject : public Object2D
{
public:
	RECT m_rtoffset;
public:
	bool Frame() override;
	bool Render() override;
};

class ImageObj : public UIObject
{
public:
	float m_fAlpha = 0.0f;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

class StartUI : public UIObject
{

public:
	bool Init();
	bool Frame();
	bool Render();
};
