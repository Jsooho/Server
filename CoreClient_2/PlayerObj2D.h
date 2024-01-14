#pragma once
#include "Object2D.h"
#include "Sprite.h"
#include "Collision.h"
class PlayerObj2D : public Object2D
{
public:
	int			m_iCurrentIndex = 0;
	float		m_fChangeTime;
	Sprite*		m_PlayerSprite;
	float		m_fTimer = 0.0f;
	bool Frame() override;
	
public:
	PlayerObj2D();
	~PlayerObj2D();
public:
	void	HitOverlap(BaseObject* pObj, DWORD dwState) override;
	
};

