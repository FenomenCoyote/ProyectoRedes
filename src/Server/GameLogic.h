#pragma once

#include "Component.h"
#include "AsteroidPool.h"
#include "BulletsPool.h"
#include "Health.h"
#include "Transform.h"

class GameLogic: public Component {
public:
	GameLogic(AsteroidPool* asPool, BulletsPool* bsPool, Health* health, Transform* trCaza);
	virtual ~GameLogic();
	void update() override;
private:
	AsteroidPool* asPool; 
	BulletsPool* bsPool;
	Health* health;
	Transform* trCaza;

	bool running;
};

