#include "AsteroidPool.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "Server.h"
#include "SRandBasedGenerator.h"

AsteroidPool::AsteroidPool(): Component(ecs::AsteroidPool), 
		inUse(),
		pool(SDLGame::instance()->getCfg()["gameLogic"]["poolSize"]["asteroids"].as_int(), [](Asteroid* o) { return o->getInUse(); })
{
}

//Genera n asteroides randomizados llamando a getObj de la pool
void AsteroidPool::generateAsteroids(int n)
{
	for (int i = 0; i < n; ++i) {
		Asteroid* a = pool.getObj();
		if (a != nullptr) {
			a->setInUse(true);
			a->setGame(game_);
			++inUse;
			randomizeAsteroid(a);
		}
	}
}

//Desactiva todos los asteroides
void AsteroidPool::disableAll()
{
	vector<Asteroid*> v = getPool();
	for (Asteroid* a : v) {
		a->setInUse(false);
	}
	inUse = 0;
}

//Se llama cuando un asteroide a colisiona con la bala b
void AsteroidPool::onCollision(Asteroid* a, Bullet* b)
{
	a->setInUse(false);
	// game_->getAudioMngr()->playChannel(Resources::Explosion, 0);
	// Mandar mensaje de evento
	int gens = a->getGens();
	--inUse;
	//Se generan dos asteroides nuevos si quedan generaciones
	if (gens > 0) {
		--gens;

		Asteroid* a1 = pool.getObj();
		Vector2D dir1 = a->getDir().rotate(45);
		Vector2D p1 = a->getPos() + dir1.normalize();
		a1->setInUse(true);
		a1->setGame(game_);
		a1->setGens(gens);
		a1->setPos(p1);
		a1->setDir(dir1);
		a1->setSize(10 + 3 * gens);

		Asteroid* a2 = pool.getObj();
		Vector2D dir2 = a->getDir().rotate(-45);
		Vector2D p2 = a->getPos() + dir2.normalize();
		a2->setInUse(true);
		a2->setGame(game_);
		a2->setGens(gens);
		a2->setPos(p2);
		a2->setDir(dir2);	
		a2->setSize(10 + 3 * gens);

		inUse += 2;
	}
}

int AsteroidPool::getNumOfAsteroid()
{
	return inUse;
}

const std::vector<Asteroid*>& AsteroidPool::getPool()
{
	return pool.getPool();
}

void AsteroidPool::randomizeAsteroid(Asteroid* a)
{
	RandomNumberGenerator* rng = game_->getRandGen();

	Vector2D pos;
	int zone = rng->nextInt(0, 3);
	if (zone == 0) { //Arriba
		pos.setY(0);
		pos.setX(rng->nextInt(0, game_->getWindowWidth()));
	}
	else if (zone == 1) { //Abajo
		pos.setY(game_->getWindowHeight());
		pos.setX(rng->nextInt(0, game_->getWindowWidth()));
	}
	else if (zone == 2) { //Izq
		pos.setX(0);
		pos.setY(rng->nextInt(0, game_->getWindowHeight()));
	}
	else { //Der
		pos.setX(game_->getWindowWidth());
		pos.setY(rng->nextInt(0, game_->getWindowHeight()));
	}

	//Calculo del centro de la pantalla con cierto error
	int offset = game_->getCfg()["gameLogic"]["centerOffset"].as_int();
	//Posici�n del centro
	Vector2D c(game_->getWindowWidth()/2 + rng->nextInt(-offset, offset), game_->getWindowHeight()/2 + rng->nextInt(-offset, offset));

	//Direccion hacia el centro
	Vector2D dir = (c - pos).normalize() * (rng->nextInt(1, 10) * game_->getCfg()["asteroid"]["speed"].as_double());

	//Calculo del numero de generaciones
	int gen = rng->nextInt(game_->getCfg()["asteroid"]["minGens"].as_int(), game_->getCfg()["asteroid"]["maxGens"].as_int());

	a->setPos(pos);
	a->setDir(dir);
	a->setSize(game_->getCfg()["asteroid"]["minSize"].as_int() + game_->getCfg()["asteroid"]["sizePerGen"].as_int() * gen);
	a->setGens(gen);
}
