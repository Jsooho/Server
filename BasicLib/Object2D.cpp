#include "Object2D.h"

void Object2D::SetRectSouce(RECT rt)
{
	m_rtSource = rt;
}

void Object2D::SetRectDraw(RECT rt)
{
	m_rtDraw = rt;
	m_fWidth = rt.right;
	m_fHeight = rt.bottom;
}

void Object2D::UpdateRectDraw(RECT rt)
{
	m_fWidth = rt.right;
	m_fHeight = rt.bottom;
}
void		Object2D::AddPosition(HVector2 vPos)
{
	// 현재위치
	m_rtCollision = Rect(m_vPos, m_fWidth, m_fHeight);
	m_vPos += vPos;
	ConvertIndex(m_vPos, m_fWidth, m_fHeight, m_VertexList);
	m_pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);
}
void		Object2D::SetPosition(HVector2 vPos)
{
	// 현재위치
	m_vPos = vPos;
}
void	Object2D::Convert(HVector2 center, float fWidth, float fHeight, std::vector<PS_VECTOR>& retList)
{
	std::vector<PS_VECTOR> list(6);
	float halfWidth = fWidth / 2.0f;
	float halfHeight = fHeight / 2.0f;
	list[0].v = { center.x - halfWidth, center.y - halfHeight };
	list[1].v = { center.x + halfWidth, center.y - halfHeight };
	list[2].v = { center.x - halfWidth, center.y + halfHeight };
	list[3].v = list[2].v;
	list[4].v = list[1].v;
	list[5].v = { center.x + halfWidth, center.y + halfHeight };

	Convert(list, retList);

}
void Object2D::Convert(std::vector<PS_VECTOR>& list, std::vector<PS_VECTOR>& retList)
{
	retList.resize(list.size());
	for (int i = 0; i < list.size(); i++)
	{
		// 0 ~ 800 -> 0 ~ 1 -> -1 ~ +1
		retList[i].v.x = list[i].v.x / g_rtClient.right;
		retList[i].v.y = list[i].v.y / g_rtClient.bottom;
		// 0 ~ 1 -> -1 ~ +1 :::: -1 ~ +1 -> 0 ~ 1
		// x = x * 2 + -1;  ::::  x= x * 0.5f + 0.5f;
		retList[i].v.x = retList[i].v.x * 2.0f - 1.0f;
		retList[i].v.y = -1.0f * (retList[i].v.y * 2.0f - 1.0f);
	}
	// 91,1, 42, 56 => 0 ~ 1
	float u = m_rtSource.left / (float)m_TextureDesc.Width;
	float v = m_rtSource.top / (float)m_TextureDesc.Height;
	float w = m_rtSource.right / (float)m_TextureDesc.Width;
	float h = m_rtSource.bottom / (float)m_TextureDesc.Height;
	retList[0].t.x = u; retList[0].t.y = v; // v0
	retList[1].t.x = u + w; retList[1].t.y = v; // v1
	retList[2].t.x = u; retList[2].t.y = v + h;
	retList[3].t = retList[2].t;
	retList[4].t = retList[1].t;
	retList[5].t.x = u + w; retList[5].t.y = v + h;

}
void Object2D::ConvertIndex(HVector2 center, float fWidth, float fHeight, std::vector<PS_VECTOR>& retList)
{
	vector<PS_VECTOR> list(4);

	float halfWidth = fWidth / 2.0f;
	float halfHeight = fHeight / 2.0f;

	list[0].v = { center.x - halfWidth, center.y - halfHeight };
	list[1].v = { center.x + halfWidth, center.y - halfHeight };
	list[2].v = { center.x - halfWidth, center.y + halfHeight };
	list[3].v = { center.x + halfWidth, center.y + halfHeight };

	ConvertIndex(list, retList);
	
}
void Object2D::ConvertIndex(std::vector<PS_VECTOR>& list, std::vector<PS_VECTOR>& retList)
{
	retList.resize(list.size());
	for (int i = 0; i < list.size(); i++)
	{
		// 0 ~ 800 -> 0 ~ 1 -> -1 ~ +1
		retList[i].v.x = list[i].v.x / g_rtClient.right;
		retList[i].v.y = list[i].v.y / g_rtClient.bottom;
		// 0 ~ 1 -> -1 ~ +1 :::: -1 ~ +1 -> 0 ~ 1
		// x = x * 2 + -1;  ::::  x= x * 0.5f + 0.5f;
		retList[i].v.x = retList[i].v.x * 2.0f - 1.0f;
		retList[i].v.y = -1.0f * (retList[i].v.y * 2.0f - 1.0f);
	}
	// 91,1, 42, 56 => 0 ~ 1

	if (m_rtSource.left == 0 && m_rtSource.right == 0 &&
		m_rtSource.top == 0 && m_rtSource.bottom == 0)
	{
		retList[0].t.x = 0.0f; retList[0].t.y = 0.0f; // v0
		retList[1].t.x = 1.0f; retList[1].t.y = 0.0f; // v1
		retList[2].t.x = 0.0f; retList[2].t.y = 1.0f;
		retList[3].t.x = 1.0f; retList[3].t.y = 1.0f;
	}
	else
	{
		float u = m_rtSource.left / (float)m_TextureDesc.Width;
		float v = m_rtSource.top / (float)m_TextureDesc.Height;
		float w = m_rtSource.right / (float)m_TextureDesc.Width;
		float h = m_rtSource.bottom / (float)m_TextureDesc.Height;
		retList[0].t.x = u; retList[0].t.y = v; // v0
		retList[1].t.x = u + w; retList[1].t.y = v; // v1
		retList[2].t.x = u; retList[2].t.y = v + h;
		retList[3].t.x = u + w; retList[3].t.y = v + h;
	}
}
bool    Object2D::SetVertexData()
{
	ConvertIndex(m_vPos, m_fWidth, m_fHeight, m_VertexList);
	return true;
}

bool Object2D::SetIndexData()
{
	m_IndexList.push_back(0); m_IndexList.push_back(1); m_IndexList.push_back(2);
	m_IndexList.push_back(2); m_IndexList.push_back(1); m_IndexList.push_back(3);
	return true;
}
