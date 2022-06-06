#include "Sprite.h"

bool Sprite::Init()
{
    return true;
}

bool Sprite::Frame()
{
    return true;
}

bool Sprite::Render()
{
    return true;
}

bool Sprite::Release()
{
    return true;
}

void Sprite::SetRectSouceArray(RECT rt)
{
    m_rtArray.push_back(rt);
}

bool Sprite::Load(wstring filename)
{
    return true;
}

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

Sprite* SpriteMgr::Load(std::wstring filename)
{
	TCHAR pBuffer[256] = { 0 };
	TCHAR pTemp[256] = { 0 };

	int iNumSprite = 0;
	float fAnimTimer;
	FILE* fp_src;
	_wfopen_s(&fp_src, filename.c_str(), _T("rt"));
	if (fp_src == NULL) return nullptr;

	_fgetts(pBuffer, _countof(pBuffer), fp_src);
	_stscanf_s(pBuffer, _T("%s%d"), pTemp, (unsigned int)_countof(pTemp),
		&iNumSprite);

	for (int iCnt = 0; iCnt < iNumSprite; iCnt++)
	{
		TCHAR name[256] = { 0 };
		Sprite* pSprite = new Sprite;
		int iNumFrame = 0;
		_fgetts(pBuffer, _countof(pBuffer), fp_src);
		_stscanf_s(pBuffer, _T("%s %d"), name, (unsigned int)_countof(name),
			&iNumFrame,
			&pSprite->m_fAnimTime);

		RECT rt;
		for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
		{
			_fgetts(pBuffer, _countof(pBuffer), fp_src);
			_stscanf_s(pBuffer, _T("%s %d %d %d %d"), pTemp, (unsigned int)_countof(pTemp),
				&rt.left, &rt.top, &rt.right, &rt.bottom);
			pSprite->m_rtArray.push_back(rt);
		}

		pSprite->m_csName = name;
		m_Datalist.insert(make_pair(pSprite->m_csName, pSprite));
		m_iIndex++;
	}
	fclose(fp_src);
    return nullptr;
}
SpriteMgr::SpriteMgr()
{

}
SpriteMgr::~SpriteMgr()
{
}
