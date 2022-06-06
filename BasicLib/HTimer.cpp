#include "HTimer.h"
float g_fSecPerFrame = 0.0f;
float g_fGameTimer = 0.0f;
bool HTimer::Init()
{
    QueryPerformanceCounter(&m_Frame); // 이전 틱

    return true;
}

bool HTimer::Frame()
{
    QueryPerformanceCounter(&m_Current);//현재 틱
    m_fSecondPerFrame = static_cast<float>(m_Current.QuadPart - m_Frame.QuadPart) / static_cast<float>(m_Frequency.QuadPart); //속도측정
    g_fSecPerFrame = m_fSecondPerFrame;

    static float fpsTime = 0.0f;
    m_iTmpCounter++;
    fpsTime += m_fSecondPerFrame;
    if (fpsTime >= 1.0f)
    {
        m_iFPS = m_iTmpCounter;
        m_iTmpCounter = 0;
        fpsTime -= 1.0f;
    }
    
    m_fGameTimer += m_fSecondPerFrame;
    g_fGameTimer = m_fGameTimer;
    m_Frame = m_Current;
    return true;
}

bool HTimer::Render()
{
    _stprintf_s(m_szTimerString,
        L"FPS=%d:Timer=%10.4f",
        m_iFPS,
        m_fGameTimer);
    return true;
}

bool HTimer::Release()
{
    return true;
}

HTimer::HTimer()
{
    m_iTmpCounter = 0;
    m_iFPS = 0;
    m_fSecondPerFrame = 0.0f;
    m_fGameTimer = 0.0f;
    QueryPerformanceFrequency(&m_Frequency); //주파수에 따라 1초당 진행되는 틱 수 계산
}

HTimer::~HTimer()
{
}
