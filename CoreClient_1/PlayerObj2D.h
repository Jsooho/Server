#pragma once
#include "Object2D.h"
#include "HInput.h"
class PlayerObj2D : public Object2D
{
public:
	bool Frame() override;
	PlayerObj2D();
	~PlayerObj2D();
};

