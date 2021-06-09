#include "AsteroidsMotion.h"
#include "AsteroidPool.h"
#include "Entity.h"

AsteroidsMotion::AsteroidsMotion(): Component(ecs::AsteroidsMotion), ap(nullptr)
{
}

void AsteroidsMotion::init()
{
	ap = GETCMP1_(AsteroidPool);
}

//llama a todos los update de los asteroides activos
void AsteroidsMotion::update()
{
	vector<Asteroid*> asteroids = ap->getPool();
	for (Asteroid* a : asteroids) {
		if (a->getInUse()) {
			a->update();
		}
	}
}
