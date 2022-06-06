#pragma once
#include "scene.h"
#include "HSound.h"
#include "ShaderMgr.h"
#include "TextureMgr.h"
#include "MapObj2D.h"
#include "PlayerObj2D.h"
#include "UIObject.h"
class Intro : public scene
{
public:
	HSound*						m_Sound;
	Texture*					m_pChangeColorTex[10];
	Texture*					m_MapTex;
	Texture* m_UITex;
	Shader* m_VShader;
	Shader* m_PShader;
	PlayerObj2D					m_player;
	MapObj2D					m_Map;
	
public:
	bool Load() override;
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
};

