#pragma once
//기본적인  사항 설정
#include "HThread.h"
class HNetwork : public HThread
{
public:
	SOCKET m_Sock;
public:
	bool Init(int iport);
	virtual bool Run() override;
public:
	HNetwork();
	virtual ~HNetwork();
};

