#include "FighterCtrl.h"
#include "Entity.h"
#include "Transform.h"

FighterCtrl::FighterCtrl() :
		Component(ecs::FighterCtrl), //
		tr_(nullptr), //
		gun_(nullptr),
		rotSpeed_(),
		speedLimit_(),
		thrust_()
{
}

FighterCtrl::~FighterCtrl() {
}

void FighterCtrl::init() {
	tr_ = GETCMP1_(Transform);
	gun_ = GETCMP1_(Gun);
	rotSpeed_ = game_->getCfg()["ship"]["rotSpeed"].as_int();
	speedLimit_ = game_->getCfg()["ship"]["maxSpeed"].as_int();
	thrust_ = game_->getCfg()["ship"]["thrust"].as_double();
}

void FighterCtrl::turnLeft() 
{
	tr_->setRot(tr_->getRot() - rotSpeed_);
}

void FighterCtrl::turnRight() 
{
	tr_->setRot(tr_->getRot() + rotSpeed_);
}

void FighterCtrl::goAhead() 
{
	Vector2D newVel = tr_->getVel() + Vector2D(0, -1).rotate(tr_->getRot()) * thrust_;
	//Se limita la velocidad maxima
	if (newVel.magnitude() > speedLimit_) newVel = newVel.normalize() * speedLimit_;
	tr_->setVel(newVel);
}

void FighterCtrl::shoot() 
{
	gun_->shoot();
}

