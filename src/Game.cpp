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
		asPool(nullptr), bsPool(nullptr),
		shipTr_p1(nullptr), shipTr_p2(nullptr), shipCtrl_p1(nullptr), shipCtrl_p2(nullptr)
{
	initGame();
}

Game::~Game() {
	closeGame();
}

void Game::initGame() {

	game_ = ServerSDLGame::init(_WINDOW_WIDTH_, _WINDOW_HEIGHT_);

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

void Game::netThread(){
	ClientMsg::InputMsg msg(ClientMsg::InputId::_AHEAD_);
	while(true){
		Socket* sd = (Socket*)1;

		socket.recv(msg, sd);

		int player = (*sd == *clientSocket_p1.get()) ? 0 : 1;

		if(msg.input == ClientMsg::InputId::_LOGOUT_)
			exit_ = (player == 0) ? PLAYER2_WON : PLAYER1_WON;
		else 
			setPlayerInput(msg.input, player);
	}
}

void Game::setPlayerInput(ClientMsg::InputId input, int player) 
{
	mGame.lock();
	switch (input)
	{
	case ClientMsg::InputId::_AHEAD_:
		if(player == 0)
			shipCtrl_p1->goAhead();
		else 
			shipCtrl_p2->goAhead();
		break;
	case ClientMsg::InputId::_LEFT_:
		if(player == 0)
			shipCtrl_p1->turnLeft();
		else 
			shipCtrl_p2->turnLeft();
		break;
	case ClientMsg::InputId::_RIGHT_:
		if(player == 0)
			shipCtrl_p1->turnRight();
		else 
			shipCtrl_p2->turnRight();
		break;
	case ClientMsg::InputId::_SHOOT_:
		if(player == 0)
			shipCtrl_p1->shoot();
		else 
			shipCtrl_p2->shoot();
		break;
	default:
		break;
	}
	mGame.unlock();
}


void Game::sendWorldState(){
	if(exit_ != PLAYING_)
		return;
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

	//net thread 1
	std::thread([this](){
		this->netThread();
	}).detach();

	//net thread 2
	std::thread([this](){
		this->netThread();
	}).detach();


	while (exit_ == PLAYING_) {
		Uint32 startTime = game_->getTime();

		mGame.lock();
		update();
		sendWorldState();
		mGame.unlock();
	
		Uint32 frameTime = game_->getTime() - startTime;
		if (frameTime < timeWait)
			SDL_Delay(timeWait - frameTime);
	}

	ServerMsg::ServerMsg msg;
	msg.type = ServerMsg::_ENDING_GAME;

	//Mandar info a los dos jugadores (de momento solo a un jugador)
	socket.send(msg, *clientSocket_p1.get());
	socket.send(msg, *clientSocket_p2.get());
}

void Game::playerDied(int player) 
{
	exit_ = (player == 0) ? PLAYER2_WON : PLAYER1_WON;
}

void Game::update() {
	entityManager_->update();
}

