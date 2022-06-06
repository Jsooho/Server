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

	// 화면좌표 위치를 중점으로 NDC 변환
	virtual void	Convert(HVector2 center, float fWidth, float fHeight,
		std::vector<PS_VECTOR>& retList);
	// 화면좌표계를 NDC 변환
	virtual void	Convert(std::vector<PS_VECTOR>& list,
		std::vector<PS_VECTOR>& retList);
	virtual void	ConvertIndex(HVector2 center, float fWidth, float fHeight,
		std::vector<PS_VECTOR>& retList);
	// 화면좌표계를 NDC 변환
	virtual void	ConvertIndex(std::vector<PS_VECTOR>& list,
		std::vector<PS_VECTOR>& retList);
public:
	virtual bool		SetVertexData() override;
	virtual bool		SetIndexData() override;
};

