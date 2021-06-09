#pragma once

#include "Component.h"
#include "AsteroidPool.h"
#include "Health.h"
#include "ScoreManager.h"

class GameCtrl: public Component {
public:
	GameCtrl(AsteroidPool* astPool, Health* health);
	virtual ~GameCtrl();
	void init() override;
	void update() override;

private:
	AsteroidPool* astPool;
	Health* health;
	ScoreManager* scoreManager_;
};

