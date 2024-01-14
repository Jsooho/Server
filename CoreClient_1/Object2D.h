#pragma once
#include"DxObject.h"
class Object2D : public DxObject
{
public:
	RECT m_rtSource;
	RECT m_rtDraw;
public:
	virtual void		SetRectSouce(RECT rt);
	virtual void		SetRectDraw(RECT rt);
	virtual void		AddPosition(HVector2 vPos);
	virtual void		SetPosition(HVector2 vPos);
	virtual void		UpdateRectDraw(RECT rt);

	// ȭ����ǥ ��ġ�� �������� NDC ��ȯ
	virtual void	Convert(HVector2 center, float fWidth, float fHeight,
		std::vector<SimpleVertex>& retList);
	// ȭ����ǥ�踦 NDC ��ȯ
	virtual void	Convert(std::vector<SimpleVertex>& list,
		std::vector<SimpleVertex>& retList);
public:
	virtual bool		SetVertexData() override;
};

