#pragma once
#include "Component.h"
#include "BulletsPool.h"

class BulletsMotion :
	public Component
{
public:
	BulletsMotion();
	void init() override;
	void update() override;
private:
	BulletsPool* bp;
};

