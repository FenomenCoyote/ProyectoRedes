#include "Asteroid.h"

Asteroid::Asteroid() :
		inUse(false),
		gens_(),
		game(nullptr),
		pos(),
		dir(),
		w(), h(),
		rot()
{
}

//Mueve al asteroide y si llega a un borde, sale por el otro lado
void Asteroid::update()
{
	Vector2D newPos = pos + dir;
	//Si se sale por la izda
	if (newPos.getX() + w < 0) newPos.setX(game->getWindowWidth());
	//Si se sale por la dcha
	else if (newPos.getX() > game->getWindowWidth()) newPos.setX(-w);
	//Si se sale por arriba
	if (newPos.getY() + h < 0) newPos.setY(game->getWindowHeight());
	//Si se sale por abajo
	else if (newPos.getY() > game->getWindowHeight()) newPos.setY(-h);

	rot += 0.5;

	pos = newPos;
}
