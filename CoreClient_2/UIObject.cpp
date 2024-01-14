#include "UIObject.h"

bool UIObject::Frame()
{
    DxObject::Frame();
    return true;
}

bool UIObject::Render()
{
    DxObject::Render();
    return true;
}

bool ImageObj::Init()
{
    
    return true;
}

bool ImageObj::Frame()
{
    DxObject::Frame();
    m_ConstantList.Color = HVector4(m_fAlpha, m_fAlpha, m_fAlpha, 1.0f);
    m_pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_ConstantList, 0, 0);
    return true;
}

bool ImageObj::Render()
{
    DxObject::Render();
    return true;
}

bool ImageObj::Release()
{
    return true;
}

bool StartUI::Init()
{
    return true;
}

bool StartUI::Frame()
{
    DxObject::Frame();
    return true;
}

bool StartUI::Render()
{
    DxObject::Render();
    return true;
}
