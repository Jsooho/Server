#include "HDxWrite.h"

bool HDxWrite::CreateFactory()
{
    HRESULT hr = S_OK;
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pd2dFactory);
    
    if (FAILED(hr))
    {
        return false;
    }

    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pdWriteFactory));
    
    if (FAILED(hr))
    {
        return false;
    }

    hr = m_pdWriteFactory->CreateTextFormat(L"±Ã¼­", 
        NULL, 
        DWRITE_FONT_WEIGHT_NORMAL, 
        DWRITE_FONT_STYLE_NORMAL, 
        DWRITE_FONT_STRETCH_NORMAL, 
        15, 
        L"ko-kr", 
        &m_pTextFormat);

    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

bool HDxWrite::CreateDeviceResources(IDXGISurface1* pSurface)
{
    HRESULT hr = S_OK;

    UINT udpi = GetDpiForWindow(g_hWnd);

    D2D1_RENDER_TARGET_PROPERTIES props;
    ZeroMemory(&props, sizeof(D2D1_RENDER_TARGET_PROPERTIES));

    props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
    props.pixelFormat.format = DXGI_FORMAT_UNKNOWN;
    props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
    props.dpiX = udpi;
    props.dpiY = udpi;
    props.usage = D2D1_RENDER_TARGET_USAGE_NONE;
    props.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

    hr = m_pd2dFactory->CreateDxgiSurfaceRenderTarget(pSurface, &props, &m_pd2dRenderTarger);

    if (FAILED(hr))
    {
        return false;
    }

    hr = m_pd2dRenderTarger->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &m_pTextBrush);

    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

bool HDxWrite::DrawText(RECT rt, std::wstring string, D2D1::ColorF color, IDWriteTextFormat* pTextFormat)
{
    if (m_pd2dRenderTarger)
    {
        m_pd2dRenderTarger->BeginDraw();
        //m_pd2dRenderTarger->SetTransform(D2D1::IdentityMatrix());
        D2D1_RECT_F rect = D2D1::RectF(rt.left, rt.top, rt.right, rt.bottom);
        m_pTextBrush->SetColor(color);
        if (pTextFormat == nullptr)
        {
            m_pd2dRenderTarger->DrawText(string.c_str(), string.size(), m_pTextFormat, rect, m_pTextBrush);
        }
        else
        {
            m_pd2dRenderTarger->DrawText(string.c_str(), string.size(), pTextFormat, rect, m_pTextBrush);
        }
        m_pd2dRenderTarger->EndDraw();
    }
    return true;
}

bool HDxWrite::Init()
{
    CreateFactory();
    return true;
}

bool HDxWrite::Frame()
{
    return true;
}

bool HDxWrite::Render()
{
    return true;
}

bool HDxWrite::Release()
{
    if (m_pd2dFactory) m_pd2dFactory->Release();
    if (m_pdWriteFactory) m_pdWriteFactory->Release();
    if (m_pd2dRenderTarger) m_pd2dRenderTarger->Release();
    if (m_pTextFormat) m_pTextFormat->Release();
    if (m_pTextBrush) m_pTextBrush->Release();

    m_pd2dFactory = nullptr;
    m_pdWriteFactory = nullptr;
    m_pd2dRenderTarger = nullptr;
    m_pTextFormat = nullptr;
    m_pTextBrush = nullptr;
    return true;
}
