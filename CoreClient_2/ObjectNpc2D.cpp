#include "ObjectNpc2D.h"

bool ObjectNpc2D::Frame()
{
    HVector2 pos;

	if (m_vPos.x > g_rtClient.right - 50.0f)
	{
		m_vDirection.x *= -1.0f;
		m_vPos.x = g_rtClient.right - 50.0f;
	}
	if (m_vPos.x < g_rtClient.left + 50.0f)
	{
		m_vDirection.x *= -1.0f;
		m_vPos.x = g_rtClient.left + 50.0f;
	}
	if (m_vPos.y > g_rtClient.bottom - 50.0f)
	{
		m_vDirection.y *= -1.0f;
		m_vPos.y = g_rtClient.bottom - 50.0f;
	}
	if (m_vPos.y < g_rtClient.top + 50.0f)
	{
		m_vDirection.y *= -1.0f;
		m_vPos.y = g_rtClient.top + 50.0f;
	}
	pos = m_vDirection * (m_fSpeed * g_fSecPerFrame);
    AddPosition(pos);

    return true;
}
ObjectNpc2D::ObjectNpc2D()
{
	m_fSpeed = 20.0f + (rand() % 300); // ¼Ó·Â
	m_vDirection.x = (rand() % 2 == 0) ? 1.0f : -1.0f;
	m_vDirection.y = (rand() % 2 == 0) ? 1.0f : -1.0f;
}
ObjectNpc2D::~ObjectNpc2D()
{

}

void ObjectNpc2D::HitOverlap(BaseObject* pObj, DWORD dwState)
{
    if (dwState == CollisionType::Overlap)
    {
        pObj->m_bAlphaBlend = !pObj->m_bAlphaBlend;
    }
}

void ObjectNpc2D::SelectHitOverlap(BaseObject* pObj, DWORD dwState)
{
	if (m_dwSelectState & S_HOVER)
	{
		int K = 0;
	}
	if (m_dwSelectState & S_FOCUS)
	{
		int K = 0;
	}
	if (m_dwSelectState & S_ACTIVE)
	{
		int K = 0;
	}
	if (m_dwSelectState & S_SELECTED)
	{
		m_bSelect = true;
		m_bDead = true;
	}
}
