#include "Gun.h"
#include "InputHandler.h"
#include "Entity.h"
#include "BulletsPool.h"

Gun::Gun():
	Gun(nullptr, 250) {
}

Gun::Gun(BulletsPool* bp,int coolDown) : Component(ecs::Gun), 
		lastShot_(game_->getTime()),
		coolDown_(),
		bp_(bp),
		tr_(nullptr), 
		speedMultiplier()
{
}

void Gun::init()
{
	tr_ = GETCMP1_(Transform);
	coolDown_ = game_->getCfg()["ship"]["gun"]["cooldown"].as_double();
	speedMultiplier = game_->getCfg()["ship"]["gun"]["bullet"]["speed"].as_int();
}

void Gun::shoot()
{ 
	Uint32 t = game_->getTime();
	//Si puede disparar
	if (t - lastShot_ > coolDown_ * 1000) {
		bp_->shoot(tr_->getPos() + Vector2D(tr_->getW() / 2, tr_->getH() / 2) + Vector2D(0, -(tr_->getH() / 2 + 20.0)).rotate(tr_->getRot()), //Pos
			Vector2D(0, -1).rotate(tr_->getRot()) * speedMultiplier + tr_->getVel(), //Vel
			2, 20); //Width & Height
		lastShot_ = t;
	}
}
