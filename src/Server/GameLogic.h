#pragma once

#include "Component.h"
#include "AsteroidPool.h"
#include "BulletsPool.h"
#include "Health.h"
#include "Transform.h"

class Game;

class GameLogic: public Component {
public:
	GameLogic(AsteroidPool* asPool, BulletsPool* bsPool, Transform* trCaza, Game* game);
	virtual ~GameLogic();
	void update() override;
private:
	AsteroidPool* asPool; 
	BulletsPool* bsPool;
	Transform* trCaza;
	Game* game;

	bool running;
};

