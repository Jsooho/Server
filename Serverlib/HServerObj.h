#pragma once
#include "HServerStd.h"
class HServerObj
{
public:
	CRITICAL_SECTION m_CS;
public:
	HServerObj();
	virtual ~HServerObj();
};

