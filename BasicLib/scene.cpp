#include "scene.h"
scene* scene::m_pScene = nullptr;
bool scene::Load()
{
	return true;
}

bool scene::Init()
{
	return true;
}

bool scene::Frame()
{
	for (auto obj : m_MapObj)
	{
		Object2D* pObj = obj.second;
		if (pObj != nullptr)
		{
			pObj->Frame();
		}
	}
	return true;
}

bool scene::Render()
{
	for (auto obj : m_MapObj)
	{
		Object2D* pObj = obj.second;
		if (pObj != nullptr)
		{
			pObj->Render();
		}
	}
	return true;
}

bool scene::Release()
{
	for (auto obj : m_MapObj)
	{
		Object2D* pObj = obj.second;
		if (pObj != nullptr)
		{
			pObj->Release();
		}
	}
	m_MapObj.clear();
	return true;
}

scene::scene()
{
}

scene::~scene()
{
}
