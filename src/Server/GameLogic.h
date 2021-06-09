#pragma once

#include "Component.h"
#include "ScoreManager.h"
#include "AsteroidPool.h"
#include "BulletsPool.h"
#include "Health.h"
#include "Transform.h"

class GameLogic: public Component {
public:
	GameLogic(AsteroidPool* asPool, BulletsPool* bsPool, Health* health, Transform* trCaza);
	virtual ~GameLogic();
	void init() override;
	void update() override;
private:
	ScoreManager *scoreManager_;
	AsteroidPool* asPool; 
	BulletsPool* bsPool;
	Health* health;
	Transform* trCaza;
};

