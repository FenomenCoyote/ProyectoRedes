#pragma once
#include "Component.h"
#include "AsteroidPool.h"
class AsteroidsMotion :
	public Component
{
public:
	AsteroidsMotion();
	void init() override;
	void update() override;
private:
	AsteroidPool* ap;
};

