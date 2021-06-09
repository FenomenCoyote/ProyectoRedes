#pragma once

#include <vector>

#include "Manager.h"
#include "SDLGame.h"

class Server {

public:
	Server();
	virtual ~Server();

	// from SDLGame
	void start();
	void stop();

	void playerInputThread();

private:
	void waitUntilPlayerConnect();
	void constructWorldMsg();
	void initGame();
	void closeGame();
	void update();

	SDLGame* game_;
	EntityManager* entityManager_;
	bool exit_;

	//Socket 

	//Info de sockets de los jugadores

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
