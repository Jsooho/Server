#pragma once
#include "Thread.h"
class Accepter : public Thread
{

public:
	bool Run() override;
public:
	Accepter();
	Accepter(LPVOID value);
};

