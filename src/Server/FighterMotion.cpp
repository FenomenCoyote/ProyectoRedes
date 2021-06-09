#include "FighterMotion.h"
#include "Transform.h"
#include "Entity.h"

FighterMotion::FighterMotion(): Component(ecs::FighterMotion), 
		deceleration_(0.995),
		tr_(nullptr)
{
}

void FighterMotion::init()
{
	tr_ = GETCMP1_(Transform);
}

// Mueve al caza y hace que rebote si llega al borde de la pantalla
void FighterMotion::update()
{
	tr_->setVel(tr_->getVel() * deceleration_);
	Vector2D newPos = tr_->getPos() + tr_->getVel();
	if (newPos.getX() < 0 || newPos.getX() > game_->getWindowWidth() - tr_->getW()) tr_->setVelX(tr_->getVel().getX() * -1);
	if (newPos.getY() < 0 || newPos.getY() > game_->getWindowHeight() - tr_->getH()) tr_->setVelY(tr_->getVel().getY() * -1);
	tr_->setPos(newPos);
}
