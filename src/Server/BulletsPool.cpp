#include "BulletsPool.h"
#include "Bullet.h"

BulletsPool::BulletsPool(): Component(ecs::BulletsPool),
		pool(SDLGame::instance()->getCfg()["gameLogic"]["poolSize"]["bullets"].as_int(), [](Bullet *o) { return o->getInUse(); })
{
}

//Se coje una bala de la pool y se le establecen sus atributos correspondientemente
void BulletsPool::shoot(Vector2D pos, Vector2D vel, double w, double h)
{
	Bullet* b = pool.getObj();
	if (b != nullptr) {
		b->setInUse(true);
		b->setGame(game_);
		b->setPos(pos);
		b->setDir(vel);
		b->setWidth(w);
		b->setHeight(h);
		b->setRot(Vector2D(0, -1).angle(vel));
	}
}

void BulletsPool::disableAll() 
{
	vector<Bullet*> v = getPool();
	for (Bullet* a : v) {
		a->setInUse(false);
	}
}

void BulletsPool::onCollision(Bullet* b, Asteroid* a)
{
	b->setInUse(false);
	//El resto de calculos sobre el asteroide se hacen en el AsteroidPool
}

const std::vector<Bullet*>& BulletsPool::getPool()
{
	return pool.getPool();
}
