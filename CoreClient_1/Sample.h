#pragma once
#include "HCore.h"
#include "PlayerObj2D.h"
#include "ObjectNpc2D.h"

class Sample : public HCore
{
public:
	ID3D11ShaderResourceView*	m_pSRV;
	ID3D11Resource*				m_pTexture;
	ID3D11ShaderResourceView*	m_pSRV1;
	ID3D11Resource*				m_pTexture1;
	ID3D11BlendState*			m_AlphaBlend;
public:
	std::vector<ObjectNpc2D> m_NpcLlist;
	std::vector<DxObject>		m_ObjectList;
	PlayerObj2D					m_player;
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
public:
	Sample();
	~Sample();
};

