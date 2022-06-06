#pragma once
#include "Object2D.h"
struct Tile
{
	RECT m_rtTile[12][11];
	RECT m_Food;
	RECT m_Outsidewall[4];
};
class MapObj2D : public Object2D
{
public:
	Tile m_Tile;
	vector<Tile> m_Tilelist;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	bool SetTile();
	
	
public:
	MapObj2D();
	virtual ~MapObj2D();
};

