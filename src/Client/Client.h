#pragma once

#include <vector>

#include "SDLGame.h"

class Client {

public:
	Client();
	virtual ~Client();

	// from SDLGame
	void start();
	void stop();

private:
	void initGame();
	void closeGame();
	void handleInput();
	void render();

	SDLGame* game_;
	bool exit_;

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
