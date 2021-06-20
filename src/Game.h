#pragma once

#include <vector>

#include "Manager.h"
#include "ServerSDLGame.h"
#include "Socket.h"
#include "ClientMsg.h"
#include <queue>
#include <memory>

class AsteroidPool;
class BulletsPool;
class Transform;
class FighterCtrl;

class Game {

public:
	Game(Socket socket_, std::unique_ptr<Socket>& clientSocket_p1_, std::unique_ptr<Socket>& clientSocket_p2_);
	virtual ~Game();

	void start();

	//Might be called from a logout from Server or by game logic
	void playerDied(int player);
	
	//Called from Server net threads
	void setPlayerInput(ClientMsg::InputId input, int player);
	
private:

	enum GameState : uint8_t{
		PLAYING_,
		PLAYER1_WON,
		PLAYER2_WON
	};

	void sendWorldState();
	void initGame();
	void closeGame();
	void update();
	void unloadQueue();

	ServerSDLGame* game_;
	EntityManager* entityManager_;
	Game::GameState exit_;

	//Socket 
	Socket socket;

	//Info de sockets de los jugadores
	std::unique_ptr<Socket>& clientSocket_p1;
	std::unique_ptr<Socket>& clientSocket_p2;

	//Cola de eventos de cada jugador
	queue<ClientMsg::InputId> inputP1, inputP2;

	//Info del mundo para enviarla
	AsteroidPool* asPool;
	BulletsPool* bsPool;
	Transform* shipTr_p1, *shipTr_p2;
	FighterCtrl* shipCtrl_p1, *shipCtrl_p2;

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
