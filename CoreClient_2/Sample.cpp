#include "Sample.h"

bool Sample::Init()
{
	//I_SoundMgr.Init();

	m_Intro.m_pContext = m_pImmediateContext;
	m_Intro.Load();

	m_Zone.m_pContext = m_pImmediateContext;
	m_Zone.Load();

	scene::m_pScene = &m_Intro;

	
	return true;
}

bool Sample::Frame()
{
	if (HInput::Get().GetKey(VK_F1) == KEY_PUSH)
	{
		scene::m_pScene = &m_Zone;
		//m_Intro.m_Sound->Stop();
		//m_Zone.m_Sound->Play(true);
	}
	scene::m_pScene->Frame();
	return true;
}

bool Sample::Render()
{
	
	scene::m_pScene->Render();
	return true;
}
bool	Sample::Release()
{
	//I_SoundMgr.Release();
	m_Intro.Release();
	m_Zone.Release();
	return true;
}
Sample::Sample()
{

}
Sample::~Sample()
{}
GAMECORE(asdasd, 800, 600);