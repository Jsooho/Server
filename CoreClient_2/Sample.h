#pragma once
#include "HCore.h"
#include "HDxWrite.h"
#include "HInput.h"
#include "Intro.h"
#include "Zone.h"
#include "MapObj2D.h"
class Sample : public HCore
{
public:
	MapObj2D	m_map;
	HDxWrite	m_write;
	Intro		m_Intro;
	Zone		m_Zone;
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
public:
	Sample();
	~Sample();
};

