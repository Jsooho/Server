#include "HSound.h"
#include "HInput.h"

void HSound::Set(FMOD::System* pSystem, int iIndex)
{
    m_pSystem = pSystem;
    m_iIndex = iIndex;
}

void HSound::Play(bool bLoop)
{
    bool Play = false;
    if (m_pChannel != nullptr)
    {
        m_pChannel->isPlaying(&Play);
    }
    if (Play == false)
    {
        //채널은 플레이 되는 사운드의 제어를 담당한다.
        FMOD_RESULT ret = FMOD_OK;
        ret = m_pSystem->playSound(m_pSound, nullptr, false, &m_pChannel);

        if (ret == FMOD_OK)
        {
            if (bLoop)
                m_pChannel->setMode(FMOD_LOOP_NORMAL);
            else
                m_pChannel->setMode(FMOD_LOOP_OFF);
        }
    }
}

void HSound::Stop()
{
    FMOD_RESULT ret = FMOD_OK;
    ret = m_pChannel->stop();

    if (ret != FMOD_OK)
    {
        return;
    }
}

void HSound::Paused()
{

}

void HSound::VolumeUp(float vol)
{
    if (m_pChannel != nullptr)
    {
        float fVol;
        m_pChannel->getVolume(&fVol);
        m_fVolum = fVol + vol;
        m_fVolum = max(0.0f, m_fVolum);
        m_fVolum = min(1.0f, m_fVolum);
        m_pChannel->setVolume(m_fVolum);
    }
}

void HSound::VolumeDown(float vol)
{
    if (m_pChannel != nullptr)
    {
        float fVol;
        m_pChannel->getVolume(&fVol);
        m_fVolum = fVol - vol;
        m_fVolum = max(0.0f, m_fVolum);
        m_fVolum = min(1.0f, m_fVolum);
        m_pChannel->setVolume(m_fVolum);
    }
}

bool HSound::Init()
{
    return true;
}

bool HSound::Frame()
{
    return true;
}

bool HSound::Render()
{
    return true;
}

bool HSound::Release()
{
    
    return true;
}

HSound::HSound()
{
}

HSound::~HSound()
{
}

bool HSoundMgr::CreateSystem()
{
    FMOD_RESULT ret;
    ret = FMOD::System_Create(&m_pSystem);
    ret = m_pSystem->init(32, FMOD_INIT_NORMAL, 0); //max채널은 보통 32개 사용

    if (ret != FMOD_OK)
    {
        return false;
    }

    return true;
}

HSound* HSoundMgr::LoadSound(string fileName)
{
   
    HSound* pSound = new HSound;
    FMOD_RESULT ret;

    ret = m_pSystem->createSound(fileName.c_str(), FMOD_DEFAULT, 0, &pSound->m_pSound);
    if (ret != FMOD_OK)
    {
        return nullptr;
    }
    m_SoundList.insert(make_pair(m_iIndex++, pSound));
    pSound->Set(m_pSystem, m_iIndex);

    return pSound;
}

HSound* HSoundMgr::GetData(int iIndex)
{
    std::map<int, HSound*>::iterator iter = m_SoundList.find(iIndex); 
    if (iter != m_SoundList.end())
    {
        return (*iter).second;
    }
    return nullptr;
}

bool HSoundMgr::Init()
{
    CreateSystem();
    return true;
}

bool HSoundMgr::Frame()
{
    //I_SoundMgr.GetData(0);
    m_pSystem->update();  //업데이트 호출을 안하면 정상적으로 돌아가지 않음
    return true;
}

bool HSoundMgr::Render()
{
    return true;
}

bool HSoundMgr::Release()
{
    for (auto data : m_SoundList)
    {
        data.second->Release();
        delete data.second;
    }
    m_SoundList.clear();

    m_pSystem->close();
    if (m_pSystem) m_pSystem->release();
    m_pSystem = nullptr;

    return true;
}

HSoundMgr::HSoundMgr()
{
    m_iIndex = 0;
}

HSoundMgr::~HSoundMgr()
{
}
