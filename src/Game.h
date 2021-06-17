#pragma once

#include <vector>

#include "Manager.h"
#include "ServerSDLGame.h"
#include "Socket.h"
#include "ClientMsg.h"

class AsteroidPool;
class BulletsPool;
class Transform;
class FighterCtrl;

class Game {

public:
	Game();
	virtual ~Game();

	// from ServerSDLGame
	void start(Socket* socket_, Socket* clientSocket_);
	void stop();

	void playerDied();
	void setPlayerInput(ClientMsg::InputId input);

private:
	void sendWorldState();
	void initGame();
	void closeGame();
	void update();

	ServerSDLGame* game_;
	EntityManager* entityManager_;
	bool exit_;

	//Socket 
	Socket* socket;

	//Info de sockets de los jugadores
	Socket* clientSocket;

	//Info del mundo para enviarla
	AsteroidPool* asPool;
	BulletsPool* bsPool;
	Transform* shipTr;
	FighterCtrl* shipCtrl;

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
