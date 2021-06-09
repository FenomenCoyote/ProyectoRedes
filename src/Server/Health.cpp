#include "Health.h"
#include "SDL_macros.h"

Health::Health(): Component(ecs::Health), hp_()
{
}

void Health::init()
{
	hp_ = game_->getCfg()["ship"]["health"].as_int();
}

void Health::subtractHp()
{
	--hp_;
}

void Health::resetHp()
{
	hp_ = game_->getCfg()["ship"]["health"].as_int();
}
