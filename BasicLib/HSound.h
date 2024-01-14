#pragma once
#include "Std.h"
#include "fmod.hpp"
#include "fmod_errors.h"

#pragma comment(lib, "fmod_vc.lib")

class HSound
{
public:
	int				m_iIndex;
	float			m_fVolum;
	FMOD::System*	m_pSystem = nullptr;
	FMOD::Sound*	m_pSound = nullptr;
	FMOD::Channel*	m_pChannel = nullptr;
	
public:
	void Set(FMOD::System* pSystem, int iIndex);
	void Play(bool bLoop = false);
	void Stop();
	void Paused();
	void VolumeUp(float vol);
	void VolumeDown(float vol);

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	HSound();
	virtual ~HSound();

};

class HSoundMgr : public Singleton<HSoundMgr>
{
public:
	friend class Singleton<HSoundMgr>;
	FMOD::System*		m_pSystem = nullptr;
	map<int, HSound*>	m_SoundList;
	int					m_iIndex;
public:
	bool CreateSystem();
	HSound* LoadSound(string fileName);
	HSound* GetData(int iIndex);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	HSoundMgr();
	~HSoundMgr();
};
#define I_SoundMgr HSoundMgr::Get()