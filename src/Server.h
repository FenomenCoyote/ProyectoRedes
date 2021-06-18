#pragma once

#include <vector>
#include <memory>

#include "Socket.h"
#include "Game.h"

class Server {

public:
	Server(const char* address, const char* port);
	virtual ~Server();

	void start();

private:

	//Socket 
	Socket socket;

	Game* game;

	std::vector<std::unique_ptr<Socket>> clients;

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
