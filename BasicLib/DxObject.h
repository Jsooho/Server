#pragma once
#include "TextureMgr.h"
#include "ShaderMgr.h"
struct CB_DATA
{
	HVector4 Color;
	HVector4 Timer;
};
struct PS_VECTOR
{
	HVector2 v;
	HVector2 t;
};

enum CollisionType
{
	Block = 0,
	Overlap,
	Ignore,
};
enum SelectType
{
	Select_Block =0,
	Select_Overlap,
	Select_Ignore,
};
enum SelectState
{
	S_DEFAULT =	0,  // 커서가 위에 없을 때(T_FOCUS상태에서 다른 곳을 T_ACTIVE하면 전환된다.)
	S_HOVER = 1,    // 커서가 위에 있을 때
	S_FOCUS = 2,    // T_ACTIVE상태에서 왼쪽 버튼을 다른 곳에서 놓았을 때(취소)
	S_ACTIVE = 4,   // 마우스 왼쪽 버튼 누르고 있을 때
	S_SELECTED = 8, // T_ACTIVE 상태에서 왼쪽버튼을 놓았을 때
};

class BaseObject
{
public:
	bool		m_bDead;
	wstring		m_csName;
	int			m_iSelectID;
	int			m_iCollisionID;
	float		m_fSpeed;
	HVector2	m_vPos;
	HVector2	m_vDirection;
	float		m_fWidth;
	float		m_fHeight;
	Rect		m_rtCollision;
	DWORD		m_dwCollisonType;
	DWORD		m_dwSelectType;
	DWORD		m_dwSelectState;
	DWORD		m_dwPreSelectState;
	bool		m_bSelect;
	bool		m_bAlphaBlend = true;
public:
	virtual void	HitOverlap(BaseObject* pObj, DWORD dwState);
	virtual void	SelectHitOverlap(BaseObject* pObj, DWORD dwState);
	virtual void	SetCollisionType(DWORD dwCollisionType, DWORD dwSelectType)
	{
		m_dwCollisonType = dwCollisionType;
		m_dwSelectType = dwSelectType;
	}
public:

	BaseObject()
	{
		m_bDead = false;
		m_bSelect = false;
		m_dwSelectState = S_DEFAULT;
		m_iCollisionID = -1;
		m_iSelectID = -1;
		m_vDirection.x = 0.0f;
		m_vDirection.y = 0.0f;
		m_dwCollisonType = CollisionType::Ignore;
		m_dwSelectType = SelectType::Select_Ignore;
	}
};

class DxObject : public BaseObject
{
public: //Texture
	Texture*					m_Texture = nullptr;
	Texture*					m_maskTexture = nullptr;
	D3D11_TEXTURE2D_DESC		m_TextureDesc;
	ID3D11BlendState*			m_AlphaBlend;
	ID3D11BlendState*			m_AlphaBlendDisable;
public:
	Shader*						m_VsShader = nullptr;
	Shader*						m_PsShader = nullptr;
public:
	float		m_fSpeed;
	HVector2	m_vPos;
	float		m_fWidth;
	float		m_fHeight;
public:
	int							m_iNumIndex;
	vector<PS_VECTOR>			m_VertexList;
	vector<DWORD>				m_IndexList;
	ID3D11Buffer*				m_pConstantBuffer;
	ID3D11Buffer*				m_IndexBuffer;
	ID3D11Buffer*				m_pVertexBuffer;
	CB_DATA						m_ConstantList;
	ID3D11InputLayout*			m_pVertexLayout;
	
	ID3D11DeviceContext*		m_pContext;
	
public:
	virtual bool CreateVertexShader(wstring filename);
	virtual bool CreatePixelShader(wstring filename);
	virtual bool CreateVertexLayout();
	virtual bool LoadTexture(std::wstring texturename, std::wstring maskname);
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
	virtual bool CreateConstantBuffer();
public:
	virtual bool SetIndexData();
	virtual bool SetVertexData();
	virtual bool SetConstantData();
	virtual void SetContext(ID3D11DeviceContext* m_pContext);
	virtual bool Create(ID3D11DeviceContext* pContext, wstring Colorname, wstring maskname, wstring filename);
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
public:
	DxObject();
	~DxObject();
};

