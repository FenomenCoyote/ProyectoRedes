#pragma once

#include "Component.h"
#include "AsteroidPool.h"
#include "BulletsPool.h"

#include "Transform.h"

class Game;

class GameLogic: public Component {
public:
	GameLogic(AsteroidPool* asPool, BulletsPool* bsPool, Transform* trCaza_p1, Transform* trCaza_p2 , Game* game);
	virtual ~GameLogic();
	void update() override;
private:
	AsteroidPool* asPool; 
	BulletsPool* bsPool;
	Transform* trCaza_p1;
	Transform* trCaza_p2;
	Game* game;

	bool running;
};

