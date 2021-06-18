#include "GameLogic.h"
#include "Collisions.h"
#include "Entity.h"
#include "Game.h"

GameLogic::GameLogic(AsteroidPool* asPool, BulletsPool* bsPool, Transform* trCaza_p1, Transform* trCaza_p2, Game* game) :
		Component(ecs::GameLogic), //
		asPool(asPool), //
		bsPool(bsPool), //
		trCaza_p1(trCaza_p1),
		trCaza_p2(trCaza_p2),
		game(game),
		running(true)
{
}

GameLogic::~GameLogic() {
}


//Logica del juego
void GameLogic::update() {
	//Si el juego esta en marcha
	if (running) {
		//Se chequea la colision de las balas con los dos cazas
		for(auto& b : bsPool->getPool()){
			if (b->getInUse() && Collisions::collidesWithRotation(b->getPos(), b->getWidth(), b->getHeight(), b->getRot(),
				trCaza_p1->getPos(), trCaza_p1->getW(), trCaza_p1->getH(), trCaza_p1->getRot())) {
				asPool->disableAll();
				bsPool->disableAll();	
				running = false;
				game->playerDied(0);
			}
			else if (b->getInUse() && Collisions::collidesWithRotation(b->getPos(), b->getWidth(), b->getHeight(), b->getRot(),
				trCaza_p2->getPos(), trCaza_p2->getW(), trCaza_p2->getH(), trCaza_p2->getRot())) {
				asPool->disableAll();
				bsPool->disableAll();	
				running = false;
				game->playerDied(1);
			}
		}

		//Se chequea las colisiones de los asteroides
		for (auto& as : asPool->getPool()) {
			//Si el asteroide esta activo
			if (as->getInUse()) {
				//Si colisiona con el caza 1
				if (Collisions::collidesWithRotation(as->getPos(), as->getWidth(), as->getHeight(), as->getRot(),
					trCaza_p1->getPos(), trCaza_p1->getW(), trCaza_p1->getH(), trCaza_p1->getRot())) {
					asPool->disableAll();
					bsPool->disableAll();
					running = false;
					game->playerDied(0);
				}

				//Si colisiona con el caza 2
				if (Collisions::collidesWithRotation(as->getPos(), as->getWidth(), as->getHeight(), as->getRot(),
					trCaza_p2->getPos(), trCaza_p2->getW(), trCaza_p2->getH(), trCaza_p2->getRot())) {
					asPool->disableAll();
					bsPool->disableAll();	
					running = false;
					game->playerDied(1);
				}

				//Se chequea si el asteroide colisiona con una bala
				for (auto& b : bsPool->getPool()) {
					//Si la bala esta activa y el asteroide colisiona con ella
					if (b->getInUse() && Collisions::collidesWithRotation(as->getPos(), as->getWidth(), as->getHeight(), as->getRot(),
						b->getPos(), b->getWidth(), b->getHeight(), b->getRot())) {
						//Se llama a los onCollision de ambas entidades
						asPool->onCollision(as, b);
						bsPool->onCollision(b, as);
					}
				}
			}
		}
	}
}

