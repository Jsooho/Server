#include "Zone.h"
#include "HInput.h"

bool Zone::Init()
{
	
    return true;
}

bool Zone::Frame()
{
	/*if (HInput::Get().GetKey(VK_NUMPAD8) == KEY_HOLD)
	{
		m_Sound->VolumeUp(g_fSecPerFrame);
	}
	if (HInput::Get().GetKey(VK_NUMPAD2) == KEY_HOLD)
	{
		m_Sound->VolumeDown(g_fSecPerFrame);
	}*/

    //m_Sound->Frame();

	
	
	scene::Render();
	m_pPlayer->Frame();
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		//RECT rt = m_NpcLlist[iObj].m_rtDraw;
		//m_NpcLlist[iObj].UpdateRectDraw(rt);
		m_NpcLlist[iObj]->Frame();
	}
    return true;
}

bool Zone::Render()
{
	
	
	
	scene::Render();
	m_pPlayer->Render();
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		if (m_NpcLlist[iObj]->m_bDead == false)
		{
			m_NpcLlist[iObj]->Render();
		}
	}
    return true;
}

bool Zone::Release()
{

	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->Release();
		delete m_NpcLlist[iObj];
	}
	m_pPlayer->Release();
	m_NpcLlist.clear();

    return true;
}

bool Zone::Load()
{
	ImageObj* obj = new ImageObj;
	obj->Init();
	obj->SetRectDraw({ 0,0, g_rtClient.right,g_rtClient.bottom });
	obj->SetPosition(HVector2(g_rtClient.right / 2.0f, g_rtClient.bottom / 2.0f));
	if (!obj->Create(m_pContext, L"../../data/resource/Pacman_map5.bmp",L"",L"MapShader.txt"))
	{
		return false;
	}
	m_MapObj.insert(std::make_pair(L"bk", obj));

    //타일 세팅 및 생성
	MapObj2D* map = new MapObj2D;
	map->Init();
	

	//플레이어 캐릭터 생성
	m_pPlayer = new PlayerObj2D;
	m_pPlayer->Init();
	m_pPlayer->SetRectSouce({ 856,6,48,54 });
	m_pPlayer->SetRectDraw({ 0,0, 45,45 });
	m_pPlayer->SetPosition(HVector2(40, 575));
    if (!m_pPlayer->Create(m_pContext, L"../../data/resource/Pacman.png", L"../../data/resource/Pacman.png", L"Shader.txt"))
    {
        return false;
    }

	ObjectNpc2D* npc = new ObjectNpc2D;
	npc->Init();
	npc->SetRectSouce({ 4,5,43,43 });
	npc->SetRectDraw({ 0,0,45,45 });
	npc->SetPosition( HVector2(g_rtClient.right/2.0f,g_rtClient.bottom/2.0f));
	
	if (!npc->Create(m_pContext, L"../../data/resource/Pacman.png", L"../../data/resource/Pacman.png", L"Shader.txt"))
	{
		return false;
	}
	m_NpcLlist.push_back(npc);
	
    return true;
}
