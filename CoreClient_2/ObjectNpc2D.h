#pragma once
#include"Object2D.h"
class ObjectNpc2D : public Object2D
{
public:
	bool Frame() override;
public:
	ObjectNpc2D();
	~ObjectNpc2D();
public:
	void	HitOverlap(BaseObject* pObj, DWORD dwState) override;
	void	SelectHitOverlap(BaseObject* pObj, DWORD dwState) override;
};

