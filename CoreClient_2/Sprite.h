#pragma once
#include"Object2D.h"

class Sprite : public Object2D
{
public:
	vector<RECT>	m_rtArray;
	float			m_fAnimTime;
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
public:
	void SetRectSouceArray(RECT rt);
	bool Load(std::wstring filename);
public:
	Sprite();
	virtual ~Sprite();
};

class SpriteMgr : public BaseMgr<Sprite, SpriteMgr>
{
public:
	friend class Singleton<SpriteMgr>;
public:
	Sprite* Load(std::wstring filename) override;
private:
	SpriteMgr();
public:
	virtual ~SpriteMgr();
};
#define I_Sprite SpriteMgr::Get()