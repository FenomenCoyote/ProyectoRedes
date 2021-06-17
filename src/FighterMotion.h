#pragma once
#include "Component.h"
#include "Transform.h"

class FighterMotion : public Component
{
public:
	FighterMotion();
	void init() override;
	void update() override;
private:
	double deceleration_;
	Transform* tr_;
};

