#include "PlayerObj2D.h"

bool PlayerObj2D::Frame()
{
	if (HInput::Get().GetKey('W') == KEY_HOLD)
	{
		HVector2 pos;
		// 시간의 동기화
		pos.y -= m_fSpeed * g_fSecPerFrame;
		AddPosition(pos);
	}
	if (HInput::Get().GetKey('A') == KEY_HOLD)
	{
		HVector2 pos;
		pos.x -= m_fSpeed * g_fSecPerFrame;
		AddPosition(pos);
	}
	if (HInput::Get().GetKey('S') == KEY_HOLD)
	{
		HVector2 pos;
		pos.y += m_fSpeed * g_fSecPerFrame;
		AddPosition(pos);
	}
	if (HInput::Get().GetKey('D') == KEY_HOLD)
	{
		HVector2 pos;
		pos.x += m_fSpeed * g_fSecPerFrame;
		AddPosition(pos);
	}
	return true;
}
PlayerObj2D::PlayerObj2D()
{
	m_fSpeed = 300.0f;
}
PlayerObj2D::~PlayerObj2D()
{

}