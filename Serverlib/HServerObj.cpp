#include "HServerObj.h"

HServerObj::HServerObj()
{
	InitializeCriticalSection(&m_CS);
}

HServerObj::~HServerObj()
{
	DeleteCriticalSection(&m_CS);
}
