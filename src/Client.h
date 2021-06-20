#pragma once

#include <vector>

#include "SDLGame.h"
#include "Socket.h"
#include "ServerMsg.h"

class Client {

public:
	Client(const char* address, const char* port);
	virtual ~Client();

	void start();
	void stop();

	void netThread();

private:
	void initGame();
	void handleInput();
	void render();

	SDLGame* game_;
	bool exit_;

	Socket socket;

	bool inGame;

	vector<ServerMsg::ServerMsg::ObjectInfo> asteroids, bullets;
	ServerMsg::ServerMsg::ObjectInfo ship1, ship2;

	Texture* msgText;

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
