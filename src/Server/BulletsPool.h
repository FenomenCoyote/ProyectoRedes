#pragma once
#include "Component.h"
#include "ObjectPool.h"
#include "Bullet.h"
class Asteroid;

class BulletsPool : public Component
{
public:
	BulletsPool();
	void shoot(Vector2D pos, Vector2D vel, double w, double h);
	void disableAll();
	void onCollision(Bullet* b, Asteroid* a);
	const std::vector<Bullet*>& getPool();
private:
	ObjectPool<Bullet> pool;
};