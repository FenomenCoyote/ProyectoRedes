#include "SDL_macros.h"
#include "Game.h"
#include <assert.h>

#include "GameLogic.h"

#include "Transform.h"
#include "Entity.h"

#include "SDLGame.h"

#include "SDL_macros.h"
#include "Gun.h"
#include "Health.h"
#include "FighterMotion.h"
#include "FighterCtrl.h"

#include "AsteroidPool.h"
#include "AsteroidsMotion.h"

#include "BulletsMotion.h"

#include "ServerMsg.h"

#include "Client/ClientMsg.h"

using namespace std;

constexpr float timeWait = 1000.0f/60.0f;

Game::Game() :
		game_(nullptr), //
		entityManager_(nullptr), //
		exit_(false), 
		socket(nullptr),
		clientSocket(nullptr),
		asPool(nullptr), bsPool(nullptr),
		shipTr(nullptr), shipCtrl(nullptr)
{
	initGame();
	socket.bind();
}

Game::~Game() {
	closeGame();
}

void Game::initGame() {

	game_ = SDLGame::init(_WINDOW_WIDTH_, _WINDOW_HEIGHT_);

	entityManager_ = new EntityManager(game_);

	//Se crea la pool de Asteroides
	Entity* asteroids = entityManager_->addEntity();
	asPool = asteroids->addComponent<AsteroidPool>();
	asteroids->addComponent<AsteroidsMotion>();

	//Se crea la pool de Bullets
	Entity* bullets = entityManager_->addEntity();
	bsPool = bullets->addComponent<BulletsPool>();
	bullets->addComponent<BulletsMotion>();

	//Se crea el caza
	Entity* ship_ = entityManager_->addEntity();
	shipTr = ship_->addComponent<Transform>(Vector2D(game_->getWindowWidth()/2, game_->getWindowHeight() / 2), Vector2D(), 50, 50, 0);
	ship_->addComponent<Gun>(bsPool, 250);
	shipCtrl = ship_->addComponent<FighterCtrl>();
	ship_->addComponent<FighterMotion>();
	
	//Se crea el gameManager
	Entity *gameManager = entityManager_->addEntity();
	gameManager->addComponent<GameLogic>(asPool, bsPool, shipTr);
}

void Game::closeGame() {
	delete entityManager_;
}

void Game::setPlayerInput(ClientMsg::InputId input) 
{
	//Pillar mutex
	switch (input)
	{
	case ClientMsg::InputId::_AHEAD_:
		shipCtrl->goAhead();
		break;
	case ClientMsg::InputId::_LEFT_:
		shipCtrl->turnLeft();
		break;
	case ClientMsg::InputId::_RIGHT_:
		shipCtrl->turnRight();
		break;
	case ClientMsg::InputId::_SHOOT_:
		shipCtrl->shoot();
		break;
	default:
		break;
	}

	//Liberar mutex
}


void Game::sendWorldState(){
	if(clientSocket == nullptr)
		return;
		
	//Crear info
	if(exit_){
		ServerMsg::ServerMsg msg(nullptr, nullptr, nullptr, this);

		//Mandar info a los dos jugadores (de momento solo a un jugador)
		socket.send(msg, *clientSocket);
	}
	else {
		ServerMsg::ServerMsg msg(asPool, bsPool, shipTr, this);
		if(asPool->anyColision())
			msg.setSound(ServerMsg::SoundId::_ASTEROID_COLLISION_);

		//Mandar info a los dos jugadores (de momento solo a un jugador)
		socket.send(msg, *clientSocket);
	}
}

void Game::start(Socket* socket_, Socket* clientSocket_) {
	exit_ = false;

	socket = socket_;
	clientSocket = clientSocket_;

	while (!exit_) {
		Uint32 startTime = game_->getTime();

		//Pillar mutex
		update();
		sendWorldState();
		//Liberar mutex
	
		Uint32 frameTime = game_->getTime() - startTime;
		if (frameTime < timeWait)
			SDL_Delay(timeWait - frameTime);

	}
}

void Game::stop() {
	exit_ = true;
}

void Game::playerDied() 
{
	exit_ = true;
}

void Game::update() {
	entityManager_->update();
}
