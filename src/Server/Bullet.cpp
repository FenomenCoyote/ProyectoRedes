#include "Bullet.h"

Bullet::Bullet(): inUse(false),
		game(),
		pos(), dir(),
		w(), h(),
		rot()
{
}

void Bullet::update()
{
	pos = pos + dir;
	//Si se sale de la pantalla
	if (pos.getX() < 0 || pos.getY() < 0 || pos.getX() > game->getWindowWidth() || pos.getY() > game->getWindowHeight()) inUse = false;
}

