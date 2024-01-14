#include "Intro.h"
#include "HInput.h"
#include "MapObj2D.h"
#include "Collision.h"
bool Intro::Load()
{

    /*m_Sound = I_SoundMgr.LoadSound("../../data/sound/autumnleaves.mp3.flac");
    m_Sound->Play(true);
    */

    ImageObj* obj = new ImageObj;
    obj->Init();
    obj->SetRectDraw({ 0,0, g_rtClient.right,g_rtClient.bottom });
    obj->SetPosition(HVector2(g_rtClient.right / 2.0f, g_rtClient.bottom / 2.0f));
    if (!obj->Create(m_pContext, L"../../data/resource/start.bmp",L"",L"MapShader.txt"))
    {
        return false;
    }
    m_MapObj.insert(std::make_pair(L"intro", obj));

    
}

bool Intro::Init()
{
    return true;
}

bool Intro::Frame()
{
   
    scene::Frame();
    return true;
}

bool Intro::Render()
{
   
    scene::Render();
    return true;
}

bool Intro::Release()
{
    
    return true;
}
