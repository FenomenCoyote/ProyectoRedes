#include "SDL_macros.h"
#include "Game.h"
#include <assert.h>

#include "GameLogic.h"

#include "Transform.h"
#include "Entity.h"

#include "ServerSDLGame.h"

#include "SDL_macros.h"
#include "Gun.h"

#include "FighterMotion.h"
#include "FighterCtrl.h"

#include "AsteroidPool.h"
#include "AsteroidsMotion.h"

#include "BulletsMotion.h"

#include "ServerMsg.h"

#include "ClientMsg.h"

#include <mutex>
#include <thread>

using namespace std;

mutex mGame;

constexpr float timeWait = 1000.0f/60.0f;

Game::Game(Socket socket_, std::unique_ptr<Socket>& clientSocket_p1_, std::unique_ptr<Socket>& clientSocket_p2_) :
		game_(nullptr), //
		entityManager_(nullptr), //
		exit_(GameState::PLAYING_), 
		socket(socket_),
		clientSocket_p1(clientSocket_p1_), clientSocket_p2(clientSocket_p2_),
		inputP1(), inputP2(),
		asPool(nullptr), bsPool(nullptr),
		shipTr_p1(nullptr), shipTr_p2(nullptr), shipCtrl_p1(nullptr), shipCtrl_p2(nullptr)
{
	initGame();
}

Game::~Game() {
	closeGame();
}

void Game::initGame() {

	game_ = ServerSDLGame::instance();

	entityManager_ = new EntityManager(game_);

	//Se crea la pool de Asteroides
	Entity* asteroids = entityManager_->addEntity();
	asPool = asteroids->addComponent<AsteroidPool>();
	asteroids->addComponent<AsteroidsMotion>();

	//Se crea la pool de Bullets
	Entity* bullets = entityManager_->addEntity();
	bsPool = bullets->addComponent<BulletsPool>();
	bullets->addComponent<BulletsMotion>();

	//Se crea el caza p1
	Entity* ship_ = entityManager_->addEntity();
	shipTr_p1 = ship_->addComponent<Transform>(Vector2D(game_->getWindowWidth()/2 - 50, game_->getWindowHeight() / 2), Vector2D(), 50, 50, 0);
	ship_->addComponent<Gun>(bsPool, 250);
	shipCtrl_p1 = ship_->addComponent<FighterCtrl>();
	ship_->addComponent<FighterMotion>();

	//Se crea el caza p2
	ship_ = entityManager_->addEntity();
	shipTr_p2 = ship_->addComponent<Transform>(Vector2D(game_->getWindowWidth()/2 + 50, game_->getWindowHeight() / 2), Vector2D(), 50, 50, 0);
	ship_->addComponent<Gun>(bsPool, 250);
	shipCtrl_p2 = ship_->addComponent<FighterCtrl>();
	ship_->addComponent<FighterMotion>();
	
	//Se crea el gameManager
	Entity *gameManager = entityManager_->addEntity();
	gameManager->addComponent<GameLogic>(asPool, bsPool, shipTr_p1, shipTr_p2, this);
}

void Game::closeGame() {
	delete entityManager_;
}

void Game::setPlayerInput(ClientMsg::InputId input, int player) {
	mGame.lock();
	if(player == 0)
		inputP1.push(input);
	else 
		inputP2.push(input);
	mGame.unlock();
}


void Game::sendWorldState(){
	ServerMsg::ServerMsg msg(asPool, bsPool, shipTr_p1, shipTr_p2);
	msg.type = ServerMsg::_WORLD_STATE;
	if(asPool->anyColision())
		msg.setSound(ServerMsg::SoundId::_ASTEROID_COLLISION_);

	//Mandar info a los dos jugadores (de momento solo a un jugador)
	socket.send(msg, *clientSocket_p1.get());
	socket.send(msg, *clientSocket_p2.get());
}

void Game::start() {
	exit_ = PLAYING_;

	asPool->generateAsteroids(game_->getCfg()["gameLogic"]["asteroidsToGenerate"].as_int());

	while (exit_ == PLAYING_) {
		Uint32 startTime = game_->getTime();

		mGame.lock();
		unloadQueue();
		mGame.unlock();

		update();
		sendWorldState();
	
		Uint32 frameTime = game_->getTime() - startTime;
		if (frameTime < timeWait)
			SDL_Delay(timeWait - frameTime);
	}

	ServerMsg::ServerMsg msgp1;
	msgp1.type = ServerMsg::_ENDING_GAME;
	ServerMsg::ServerMsg msgp2;
	msgp2.type = ServerMsg::_ENDING_GAME;

	cout << "ending game, sending msg ..." << endl;

	if(exit_ == GameState::PLAYER1_WON){
		msgp1.won = true;
		msgp2.won = false;
	}
	else if(exit_ == GameState::PLAYER2_WON){
		msgp1.won = false;
		msgp2.won = true;
	}

	socket.send(msgp1, *clientSocket_p1.get());
	socket.send(msgp2, *clientSocket_p2.get());
}

void Game::unloadQueue(){
	ClientMsg::InputId input;
	while(!inputP1.empty()){
		input = inputP1.front(); inputP1.pop();
		switch (input) {
			case ClientMsg::InputId::_AHEAD_:
				shipCtrl_p1->goAhead();
				break;
			case ClientMsg::InputId::_LEFT_:
				shipCtrl_p1->turnLeft();
				break;
			case ClientMsg::InputId::_RIGHT_:
				shipCtrl_p1->turnRight();
				break;
			case ClientMsg::InputId::_SHOOT_:
				shipCtrl_p1->shoot();
				break;
			default:
				break;
		}
	}
	while(!inputP2.empty()){
		input = inputP2.front(); inputP2.pop();
		switch (input) {
			case ClientMsg::InputId::_AHEAD_:
				shipCtrl_p2->goAhead();
				break;
			case ClientMsg::InputId::_LEFT_:
				shipCtrl_p2->turnLeft();
				break;
			case ClientMsg::InputId::_RIGHT_:
				shipCtrl_p2->turnRight();
				break;
			case ClientMsg::InputId::_SHOOT_:
				shipCtrl_p2->shoot();
				break;
			default:
				break;
		}
	}
}

void Game::playerDied(int player) 
{
	exit_ = (player == 0) ? PLAYER2_WON : PLAYER1_WON;
}

void Game::update() {
	entityManager_->update();
}

