#pragma once

#include <vector>

#include "Manager.h"
#include "SDLGame.h"
#include "Socket.h"

class AsteroidPool;
class BulletsPool;
class Transform;
class Health;

class Server {

public:
	Server(const char* address, const char* port);
	virtual ~Server();

	// from SDLGame
	void start();
	void stop();

	void playerInputThread();

private:
	void waitUntilPlayerConnect();
	void sendWorldState();
	void initGame();
	void closeGame();
	void update();

	SDLGame* game_;
	EntityManager* entityManager_;
	bool exit_;

	//Socket 
	Socket socket;

	//Info de sockets de los jugadores
	Socket* clientSocket;

	//Info del mundo para enviarla

	AsteroidPool* asPool;
	BulletsPool* bsPool;
	Transform* ship;
	Health* health;

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
