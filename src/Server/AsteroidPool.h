#pragma once
#include "Component.h"
#include "Asteroid.h"
#include "ObjectPool.h"
class Bullet;

class AsteroidPool : public Component
{
public:
	AsteroidPool();
	void generateAsteroids(int n);
	void disableAll();
	void onCollision(Asteroid* a, Bullet* b);
	int getNumOfAsteroid();
	const std::vector<Asteroid*>& getPool();
private:
	void randomizeAsteroid(Asteroid* as);
	ObjectPool<Asteroid> pool;
	unsigned int inUse;
};

