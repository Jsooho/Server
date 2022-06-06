#pragma once
#include "HVector.h"
enum collisionRet
{
	RECT_OUT = 0,
	RECT_IN,
	RECT_OVERLAP,
};
struct Rect2D
{
	HVector2 vMin;
	HVector2 vMax;
	HVector2 vMiddle;
	HVector2 size;
	bool operator == (const Rect2D& v)
	{
		if (fabs((vMin - v.vMin).Length()) < 0.0001f)
		{
			if (fabs((vMax - v.vMax).Length()) < 0.0001f)
			{
				return true;
			}
		}
		return false;
	}
	Rect2D() {};
	Rect2D(HVector2 vMin, HVector2 vMax)
	{
		this->vMin = vMin;
		this->vMax = vMax;
		vMiddle = (vMax + vMin) / 2.0f;
		size.x = vMax.x - vMin.x;
		size.y = vMax.y - vMin.y;
	}
	Rect2D(HVector2 v, float w, float h)
	{
		this->vMin = v;
		this->vMax = vMin + HVector2(w, h);
		vMiddle = (vMax + vMin) / 2.0f;
		this->size.x = w;
		this->size.y = h;
	}
};

struct Rect
{
	HVector2 vMin;
	HVector2 vMax;
	HVector2 vMiddle;
	HVector2 size;
	bool operator == (const Rect& v)
	{
		if (fabs((vMin - v.vMin).Length()) < 0.0001f)
		{
			if (fabs((vMax - v.vMax).Length()) < 0.0001f)
			{
				return true;
			}
		}
		return false;
	}
	Rect() {};
	Rect(HVector2 vMin, HVector2 vMax)
	{
		this->vMin = vMin;
		this->vMax = vMax;
		vMiddle = (vMax + vMin) / 2.0f;
		size.x = vMax.x - vMin.x;
		size.y = vMax.y - vMin.y;
	}
	Rect(HVector2 v, float w, float h)
	{
		this->vMin = v;
		this->vMax = vMin + HVector2(w, h);
		vMiddle = (vMax + vMin) / 2.0f;
		this->size.x = w;
		this->size.y = h;
	}
};
struct Box
{
	HVector3 vMin;
	HVector3 vMax;
	HVector3 vMiddle;
	HVector3 size;
	bool operator == (const Box& v)
	{
		if (fabs((vMin - v.vMin).Length()) < 0.0001f)
		{
			if (fabs((vMax - v.vMax).Length()) < 0.0001f)
			{
				return true;
			}
		}
		return false;
	}
	Box() {};
	Box(HVector3 vMin, HVector3 vMax)
	{
		this->vMin = vMin;
		this->vMax = vMax;
		vMiddle = (vMax + vMin) / 2.0f;
		size.x = vMax.x - vMin.x;
		size.y = vMax.y - vMin.y;
	}
	Box(HVector3 v, float w, float h, float q)
	{
		this->vMin = v;
		this->vMax = vMin + HVector3(w, h, q);
		vMiddle = (vMax + vMin) / 2.0f;
		this->size.x = w;
		this->size.y = h;
	}
};
class Collision
{
public:
	static collisionRet RectToRect(Rect, Rect);
	static bool   RectToPoint(Rect rt, int x, int y);
	static bool   RectToPoint(Rect rt, HVector2 v);
	static collisionRet ToRect(Rect rt1, Rect rt2);
	static Rect UnionRect(Rect rt1, Rect rt2);
	static bool IntersectRect(Rect rt1, Rect rt2, Rect* rt);

	static bool   BoxToPoint(Box rt, int x, int y, int z);
	static bool   BoxToPoint(Box rt, HVector3 v);
	static collisionRet    BoxToBox(Box, Box);
	static Box   UnionBox(Box rt1, Box rt2);
	static bool  IntersectBox(Box rt1, Box rt2, Box* rt);
};

