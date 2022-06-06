#pragma once
#include "scene.h"
#include "HSound.h"
#include "MapObj2D.H"
#include "PlayerObj2D.h"
#include "ObjectNpc2D.h"
#include "UIObject.h"
class Zone : public scene
{
public:
	std::vector<ObjectNpc2D*>	m_NpcLlist;
	PlayerObj2D* m_pPlayer;
	
	HSound*						m_Sound;
	
	
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
	bool Load() override;
};

