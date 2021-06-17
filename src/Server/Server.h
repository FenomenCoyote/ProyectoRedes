#pragma once

#include <vector>

#include "../Socket.h"
#include "Game.h"

class Server {

public:
	Server(const char* address, const char* port);
	virtual ~Server();

	// from SDLGame
	void start();

	void inputThread();

private:

	//Socket 
	Socket socket;

	//Info de sockets de los jugadores
	Socket* clientSocket;

	Game* game;

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
