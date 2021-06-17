#pragma once
#include "Component.h"
#include "BulletsPool.h"
#include "Transform.h"

class Gun : public Component
{
public:
	Gun();
	Gun(BulletsPool* bp, int coolDown);
	void init() override;
	void shoot();
private:
	BulletsPool* bp_;
	Transform* tr_;
	Uint32 lastShot_;
	double coolDown_;
	double speedMultiplier;
};

