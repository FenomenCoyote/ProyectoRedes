#include "SDL_macros.h"
#include "Server.h"
#include <assert.h>

#include "GameLogic.h"

#include "Transform.h"
#include "Entity.h"

#include "SDLGame.h"

#include "SDL_macros.h"
#include "Gun.h"
#include "Health.h"
#include "FighterMotion.h"

#include "AsteroidPool.h"
#include "AsteroidsMotion.h"

#include "BulletsMotion.h"

#include "ServerMsg.h"

using namespace std;

constexpr float timeWait = 1000.0f/60.0f;

Server::Server(const char* address, const char* port) :
		game_(nullptr), //
		entityManager_(nullptr), //
		exit_(false), 
		socket(address, port),
		clientSocket(nullptr),
		asPool(nullptr), bsPool(nullptr),
		ship(nullptr), health(nullptr)
{
	initGame();
	socket.bind();
}

Server::~Server() {
	closeGame();
}

void Server::initGame() {

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
	ship = ship_->addComponent<Transform>(Vector2D(game_->getWindowWidth()/2, game_->getWindowHeight() / 2), Vector2D(), 50, 50, 0);
	ship_->addComponent<Gun>(bsPool, 250);
	health = ship_->addComponent<Health>();
	ship_->addComponent<FighterMotion>();
	
	//Se crea el gameManager
	Entity *gameManager = entityManager_->addEntity();
	gameManager->addComponent<GameLogic>(asPool, bsPool, health, ship);
}

void Server::closeGame() {
	delete entityManager_;
}

void Server::playerInputThread() {
	while(1){
		//recv from player
		//ClientInputMsg msg;
		//clientSocket->recv(msg);

		//If x the do y, 
		//etc 
	}
}


void Server::waitUntilPlayerConnect() {

	//ClientMsg msg;
	clientSocket = (Socket*)1;
    //socket.recv(msg, clientSocket);

}

void Server::sendWorldState(){
	//Crear info
	ServerMsg::WorldStateMSg msg(asPool, bsPool, ship, health);
	//Mandar info a los dos jugadores (de momento solo a un jugador)
	socket.send(msg, *clientSocket);
}

void Server::start() {
	exit_ = false;

	waitUntilPlayerConnect();

	//Make InputThread

	while (!exit_) {
		Uint32 startTime = game_->getTime();

		update();

		sendWorldState();
	
		Uint32 frameTime = game_->getTime() - startTime;
		if (frameTime < timeWait)
			SDL_Delay(timeWait - frameTime);
	}
}

void Server::stop() {
	exit_ = true;
}

void Server::update() {
	entityManager_->update();
}

