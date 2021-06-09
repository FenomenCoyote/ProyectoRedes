#pragma once
#include "Vector2D.h"
#include "SDLGame.h"
class Bullet
{
public:
	Bullet();
	bool getInUse() { return inUse; }
	void setInUse(bool b) { inUse = b; }

	inline void setGame(SDLGame* Game) { game = Game; }

	inline void setPos(Vector2D Pos) { pos = Pos; }
	inline Vector2D& getPos() { return pos; }

	inline void setDir(Vector2D Dir) { dir = Dir; }
	inline void setRot(double Rot) { rot = Rot; }

	inline void setWidth(int width) {w = width;}
	inline void setHeight(int heigth) { h = heigth; }

	inline int getWidth() { return w; }
	inline int getHeight() { return h; }
	inline double getRot() { return rot; }

	void update();
private:
	bool inUse;
	SDLGame* game;
	Vector2D pos, dir;
	int w, h;
	double rot;
};
