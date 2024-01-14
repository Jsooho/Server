#include "PlayerObj2D.h"
#include "HInput.h"



bool PlayerObj2D::Frame()
{
	if (HInput::Get().GetKey('W') == KEY_HOLD)
	{
		I_Sprite.Load(L"PacmanMove.txt");
		m_PlayerSprite = I_Sprite.GetPtr(L"rtUp");
		m_fChangeTime = m_PlayerSprite->m_fAnimTime / m_PlayerSprite->m_rtArray.size();
		m_fTimer += g_fSecPerFrame;
		if (m_fTimer >= m_fChangeTime)
		{
			m_iCurrentIndex++;
			if (m_iCurrentIndex >= m_PlayerSprite->m_rtArray.size())
			{
				m_iCurrentIndex = 0;
			}
			m_fTimer -= m_fChangeTime;
			SetRectSouce(m_PlayerSprite->m_rtArray[m_iCurrentIndex]);
			SetRectDraw({ 0,0,
			m_PlayerSprite->m_rtArray[m_iCurrentIndex].right,
			m_PlayerSprite->m_rtArray[m_iCurrentIndex].bottom });
		}
		HVector2 pos;
		// 시간의 동기화
		pos.y -= m_fSpeed * g_fSecPerFrame;
		AddPosition(pos);
	}
	if (HInput::Get().GetKey('A') == KEY_HOLD)
	{
		I_Sprite.Load(L"PacmanMove.txt");
		m_PlayerSprite = I_Sprite.GetPtr(L"rtLeft");
		m_fChangeTime = m_PlayerSprite->m_fAnimTime / m_PlayerSprite->m_rtArray.size();
		m_fTimer += g_fSecPerFrame;
		if (m_fTimer >= m_fChangeTime)
		{
			m_iCurrentIndex++;
			if (m_iCurrentIndex >= m_PlayerSprite->m_rtArray.size())
			{
				m_iCurrentIndex = 0;
			}
			m_fTimer -= m_fChangeTime;
			SetRectSouce(m_PlayerSprite->m_rtArray[m_iCurrentIndex]);
			SetRectDraw({ 0,0,
			m_PlayerSprite->m_rtArray[m_iCurrentIndex].right,
			m_PlayerSprite->m_rtArray[m_iCurrentIndex].bottom });
		}
		HVector2 pos;
		pos.x -= m_fSpeed * g_fSecPerFrame;
		AddPosition(pos);
	}
	if (HInput::Get().GetKey('S') == KEY_HOLD)
	{
		I_Sprite.Load(L"PacmanMove.txt");
		m_PlayerSprite = I_Sprite.GetPtr(L"rtDown");
		m_fChangeTime = m_PlayerSprite->m_fAnimTime / m_PlayerSprite->m_rtArray.size();
		m_fTimer += g_fSecPerFrame;
		if (m_fTimer >= m_fChangeTime)
		{
			m_iCurrentIndex++;
			if (m_iCurrentIndex >= m_PlayerSprite->m_rtArray.size())
			{
				m_iCurrentIndex = 0;
			}
			m_fTimer -= m_fChangeTime;
			SetRectSouce(m_PlayerSprite->m_rtArray[m_iCurrentIndex]);
			SetRectDraw({ 0,0,
			m_PlayerSprite->m_rtArray[m_iCurrentIndex].right,
			m_PlayerSprite->m_rtArray[m_iCurrentIndex].bottom });
		}
		HVector2 pos;
		pos.y += m_fSpeed * g_fSecPerFrame;
		AddPosition(pos);
		
	}
	if (HInput::Get().GetKey('D') == KEY_HOLD)
	{

		I_Sprite.Load(L"PacmanMove.txt");
		m_PlayerSprite = I_Sprite.GetPtr(L"rtRight");
		m_fChangeTime = m_PlayerSprite->m_fAnimTime / m_PlayerSprite->m_rtArray.size();
		m_fTimer += g_fSecPerFrame;
		if (m_fTimer >= m_fChangeTime)
		{
			m_iCurrentIndex++;
			if (m_iCurrentIndex >= m_PlayerSprite->m_rtArray.size())
			{
				m_iCurrentIndex = 0;
			}
			m_fTimer -= m_fChangeTime;
			SetRectSouce(m_PlayerSprite->m_rtArray[m_iCurrentIndex]);
			SetRectDraw({ 0,0,
			m_PlayerSprite->m_rtArray[m_iCurrentIndex].right,
			m_PlayerSprite->m_rtArray[m_iCurrentIndex].bottom });
		}

		HVector2 pos;
		pos.x += m_fSpeed * g_fSecPerFrame;
		AddPosition(pos);

	}
	return true;
}
PlayerObj2D::PlayerObj2D()
{
	m_fSpeed = 80.0f;
}
PlayerObj2D::~PlayerObj2D()
{

}

void PlayerObj2D::HitOverlap(BaseObject* pObj, DWORD dwState)
{
	if (dwState == CollisionType::Overlap)
	{
		pObj->m_bAlphaBlend = false;
	}
}



