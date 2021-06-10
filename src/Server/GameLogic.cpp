#include "GameLogic.h"
#include "Collisions.h"
#include "Resources.h"
#include "Entity.h"

GameLogic::GameLogic(AsteroidPool* asPool, BulletsPool* bsPool, Health* health, Transform* trCaza) :
		Component(ecs::GameLogic), //
		asPool(asPool), //
		bsPool(bsPool), //
		health(health), //
		trCaza(trCaza),
		running(true)
{
}

GameLogic::~GameLogic() {
}


//Logica del juego
void GameLogic::update() {
	//Si el juego esta en marcha
	if (running) {
		//Se chequea las colisiones de los asteroides
		for (auto& as : asPool->getPool()) {
			//Si el asteroide esta activo
			if (as->getInUse()) {
				//Si colisiona con el caza, se pausa el juego, se resta una vida y se pone al caza en la posicion original
				if (Collisions::collidesWithRotation(as->getPos(), as->getWidth(), as->getHeight(), as->getRot(),
					trCaza->getPos(), trCaza->getW(), trCaza->getH(), trCaza->getRot())) {
					asPool->disableAll();
					bsPool->disableAll();
					health->subtractHp();	
					trCaza->setPos(game_->getWindowWidth() / 2 - trCaza->getW() / 2, game_->getWindowHeight() / 2 - trCaza->getH() / 2);
					trCaza->setVel(0, 0);
					trCaza->setRot(0);			
					running = false;		
				}

				//Se chequea si el asteroide colisiona con una bala
				//Mirar si usar un while
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

