#pragma once
#include "Vector2D.h"
#include "SDLGame.h"
class Asteroid
{
public:
	Asteroid();
	inline bool getInUse() const { return inUse; }
	inline void setInUse(bool b) { inUse = b; }

	inline void setGame(SDLGame* Game) { game = Game; }

	inline void setPos(Vector2D Pos) { pos = Pos; }
	inline Vector2D& getPos() { return pos; }

	inline void setDir(Vector2D Dir) { dir = Dir; }
	inline Vector2D& getDir() { return dir; }

	inline int getWidth() { return w; }
	inline int getHeight() { return h; }
	inline double getRot() { return rot; }

	inline void setSize(int s) {
		w = s;
		h = s;
	}

	inline void setGens(int g) { gens_ = g; }
	inline int getGens() { return gens_; }

	void update();

private:
	bool inUse;
	int gens_;
	SDLGame* game;
	Vector2D pos, dir;
	int w, h;
	double rot;
};

