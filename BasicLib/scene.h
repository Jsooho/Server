#pragma once
#include "Std.h"
#include "Object2D.h"
class scene
{
public:
	ID3D11DeviceContext* m_pContext;
public:
	static scene* m_pScene;
	map<wstring, Object2D*> m_MapObj;
	map<wstring, Object2D*> m_UIObj;
public:
	virtual bool Load();
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
public:
	scene();
	virtual ~scene();

};

