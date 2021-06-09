#include "FighterCtrl.h"
#include "InputHandler.h"
#include "Entity.h"
#include "Transform.h"

FighterCtrl::FighterCtrl() :
		FighterCtrl(SDLK_LEFT, SDLK_RIGHT, SDLK_UP) {
}

FighterCtrl::FighterCtrl(SDL_Keycode left, SDL_Keycode right, SDL_Keycode ahead) :
		Component(ecs::FighterCtrl), //
		left_(left), //
		right_(right), //
		ahead_(ahead),
		tr_(nullptr), //
		rotSpeed_(),
		speedLimit_(),
		thrust_()
{
}

FighterCtrl::~FighterCtrl() {
}

void FighterCtrl::init() {
	tr_ = GETCMP1_(Transform);
	rotSpeed_ = game_->getCfg()["ship"]["rotSpeed"].as_int();
	speedLimit_ = game_->getCfg()["ship"]["maxSpeed"].as_int();
	thrust_ = game_->getCfg()["ship"]["thrust"].as_double();
}

void FighterCtrl::update() {

	InputHandler *ih = InputHandler::instance();

	if (ih->keyDownEvent()) {
		if (ih->isKeyDown(left_)) {
			tr_->setRot(tr_->getRot() - rotSpeed_);
		} 
		if (ih->isKeyDown(right_)) {
			tr_->setRot(tr_->getRot() + rotSpeed_);
		}  
		if (ih->isKeyDown(ahead_)) {
			Vector2D newVel = tr_->getVel() + Vector2D(0, -1).rotate(tr_->getRot()) * thrust_;
			//Se limita la velocidad maxima
			if (newVel.magnitude() > speedLimit_) newVel = newVel.normalize() * speedLimit_;
			tr_->setVel(newVel);
		}
	}
}

