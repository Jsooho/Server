#include "Sample.h"

bool Sample::Init()
{
	
	m_player.Init();
	m_player.SetRectSouce({ 91,1,42,56 });
	//m_PlayerObj.SetRectSouce({ 46,63,69,79 });
	m_player.SetRectDraw({ 0,0, 42,56 });
	m_player.SetPosition(HVector2(400, 300));
	if (!m_player.Create(m_pImmediateContext))
	{
		return false;
	}

	for (int iNpc = 0; iNpc < 3; iNpc++)
	{
		ObjectNpc2D npc;
		npc.Init();
		if (iNpc % 2 == 0)
		{
			npc.SetRectSouce({ 46,63,69,79 });
			npc.SetRectDraw({ 0,0,69,79 }); // l,t,r,b
		}
		else
		{
			npc.SetRectSouce({ 1,63,42,76 });
			npc.SetRectDraw({ 0,0,42,76 });
		}
		npc.SetPosition(HVector2(50+iNpc*150, 50));
		if (!npc.Create(m_pImmediateContext))
		{
			return false;
		}
		m_NpcLlist.push_back(npc);
	}

	return true;
}

bool Sample::Frame()
{
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		RECT rt = m_NpcLlist[iObj].m_rtDraw;
		m_NpcLlist[iObj].UpdateRectDraw(rt);
		m_NpcLlist[iObj].Frame();
	}
	m_player.Frame();
	return true;
}

bool Sample::Render()
{
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj].Render();
	}
	m_player.Render();

	return true;
}
bool	Sample::Release()
{
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj].Release();
	}
	m_player.Release();

	if (m_AlphaBlend) m_AlphaBlend->Release();
	if (m_pTexture) m_pTexture->Release();
	if (m_pSRV) m_pSRV->Release();
	/*if (m_pTexture1) m_pTexture1->Release();
	if (m_pSRV1) m_pSRV1->Release();*/
	m_AlphaBlend = nullptr;
	m_pTexture = nullptr;
	m_pSRV = nullptr;
	/*m_pTexture1 = nullptr;
	m_pSRV1 = nullptr;*/
	//m_Net.CloseNetwork();
	return true;
}
Sample::Sample()
{

}
Sample::~Sample()
{}
GAMECORE(asdasd, 800, 600);