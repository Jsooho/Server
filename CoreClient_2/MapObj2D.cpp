#include "MapObj2D.h"


bool MapObj2D::Init()
{
    SetTile();
    return true;
}

bool MapObj2D::Frame()
{
    
    DxObject::Frame();
    return true;
}

bool MapObj2D::Render()
{
    DxObject::Render();
    return true;
}

bool MapObj2D::Release()
{
    return true;
}

bool MapObj2D::SetTile()
{
    RECT rt;
    //鸥老农扁
    rt.left = 0.0f;
    rt.top = 0.0f;
    rt.right = 45.0f;
    rt.bottom = 45.0f;
    //food农扁


    //outsidewall农扁
    //top
    m_Tile.m_Outsidewall[0].left = 0.0f;
    m_Tile.m_Outsidewall[0].top = 0.0f;
    m_Tile.m_Outsidewall[0].right = 495.0f;
    m_Tile.m_Outsidewall[0].bottom = 9.0f;
    //left
    m_Tile.m_Outsidewall[1].left = 0.0f;
    m_Tile.m_Outsidewall[1].top = 0.0f;
    m_Tile.m_Outsidewall[1].right = 11.0f;
    m_Tile.m_Outsidewall[1].bottom = 540.0f;
    //right
    m_Tile.m_Outsidewall[2].left = 484.0f;
    m_Tile.m_Outsidewall[2].top = 0.0f;
    m_Tile.m_Outsidewall[2].right = 495.0f;
    m_Tile.m_Outsidewall[2].bottom = 540.0f;
    //bottom
    m_Tile.m_Outsidewall[3].left = 0.0f;
    m_Tile.m_Outsidewall[3].top = 531.0f;
    m_Tile.m_Outsidewall[3].right = 495.0f;
    m_Tile.m_Outsidewall[3].bottom = 540.0f;

    RECT Totalmap;
    Totalmap.right = 495.0f;
    Totalmap.bottom = 540.0f;

    float w = rt.right - rt.left;
    float h = rt.bottom - rt.top;
    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 12; j++)
        {

            if (rt.right <= Totalmap.right && rt.bottom <= Totalmap.bottom)
            {
                
                m_Tile.m_rtTile[i][j] = rt;
                rt.left += w;
                rt.right += w;
                m_Tilelist.push_back(m_Tile);
            }
        }
        rt.top += h;
        rt.bottom += h;
        rt.left = 0.0f;
        rt.right = 45.0f;
    }
    
    return true;
}








MapObj2D::MapObj2D()
{
}

MapObj2D::~MapObj2D()
{
}

