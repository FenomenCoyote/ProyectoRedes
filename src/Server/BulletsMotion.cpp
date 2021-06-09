#include "BulletsMotion.h"
#include "AsteroidPool.h"
#include "Entity.h"

BulletsMotion::BulletsMotion() : Component(ecs::BulletsMotion), bp(nullptr)
{
}

void BulletsMotion::init()
{
	bp = GETCMP1_(BulletsPool);
}

//llama a todos los update de las balas activas
void BulletsMotion::update()
{
	vector<Bullet*> bullets = bp->getPool();
	for (Bullet* b : bullets) {
		if (b->getInUse()) {
			b->update();
		}
	}
}