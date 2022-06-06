#include "Sample.h"
GAMECORE(asdsd,800,600);

HRESULT Sample::CreateVertexBuffer()
{

    PS_VECTOR v[3];

    v[0].pos.x = -1.0f; v[0].pos.y = 1.0f; v[0].pos.z = 0.0f;  v[0].color.x = 1.0f; v[0].color.y = 0.0f; v[0].color.z = 0.0f; v[0].color.w = 1.0f;
    v[1].pos.x = 1.0f; v[1].pos.y = 1.0f; v[1].pos.z = 0.0f;   v[1].color.x = 0.0f; v[1].color.y = 1.0f; v[1].color.z = 0.0f; v[1].color.w = 1.0f;
    v[2].pos.x = -1.0f; v[2].pos.y = -1.0f; v[2].pos.z = 0.0f; v[2].color.x = 0.0f; v[2].color.y = 0.0f; v[2].color.z = 1.0f; v[2].color.w = 1.0f;

    for (int i = 0; i < _countof(v); i++)
    {
        m_Vertexlist.push_back(v[i]);
    }

    HRESULT hr = S_OK;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(PS_VECTOR) * m_Vertexlist.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA sd;
    ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
    sd.pSysMem = &m_Vertexlist.at(0);
    hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    if (FAILED(hr))
    {
        return hr;
    }

    return hr;
}

HRESULT Sample::LoadShader()
{
    HRESULT hr = S_OK;
    ID3DBlob* error = nullptr;

    hr = D3DCompileFromFile(L"VertexShader.txt", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &m_VSBlob, nullptr);
    if (FAILED(hr))
    {
        return hr;
    }
    hr = m_pd3dDevice->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), NULL, &m_pVS);
    if (FAILED(hr))
    {
        return hr;
    }
    ID3DBlob* PSblob;
    hr = D3DCompileFromFile(L"PixelShader.txt", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &PSblob, nullptr);
    if (FAILED(hr))
    {
        MessageBoxA(NULL,
            (char*)error->GetBufferPointer(),
            "error", MB_OK);
        return hr;
    }
    hr = m_pd3dDevice->CreatePixelShader(PSblob->GetBufferPointer(), PSblob->GetBufferSize(), NULL, &m_pPS);
    if (FAILED(hr)) return hr;
    PSblob->Release();

    return hr;
}

HRESULT Sample::CreateVertexLayout()
{
    HRESULT hr = S_OK;
    D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    int num = sizeof(vertexDesc) / sizeof(vertexDesc[0]);
    hr = m_pd3dDevice->CreateInputLayout(vertexDesc, num, m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), &m_VertexLayout);
    if (FAILED(hr))
    {
        return hr;
    }
    return hr;
}

bool Sample::Init()
{

    CreateVertexBuffer();
    LoadShader();
    CreateVertexLayout();
    return true;
}
bool Sample::Frame()
{
    return true;
}
bool Sample::Render()
{
    m_pImmediateContext->IASetInputLayout(m_VertexLayout);
    m_pImmediateContext->VSSetShader(m_pVS, NULL, 0);
    m_pImmediateContext->PSSetShader(m_pPS, NULL, 0);
    UINT pStrides = sizeof(PS_VECTOR);
    UINT pOffsets = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &pStrides, &pOffsets);
    m_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pImmediateContext->Draw(m_Vertexlist.size(), 0);
    return true;
}
bool Sample::Release()
{
    m_VertexBuffer->Release();
    m_VertexLayout->Release();
    m_pVS->Release();
    m_pPS->Release();
    return true;
}
