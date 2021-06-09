#include "SDL_macros.h"
#include "Server.h"
#include <assert.h>

#include "GameCtrl.h"
#include "GameLogic.h"

#include "ScoreManager.h"

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

using namespace std;

constexpr float timeWait = 1000.0f/60.0f;

Server::Server() :
		game_(nullptr), //
		entityManager_(nullptr), //
		exit_(false) {
	initGame();
}

Server::~Server() {
	closeGame();
}

void Server::initGame() {

	game_ = SDLGame::init(_WINDOW_WIDTH_, _WINDOW_HEIGHT_);

	entityManager_ = new EntityManager(game_);

	//Se crea la pool de Asteroides
	Entity* asteroids = entityManager_->addEntity();
	AsteroidPool* asPool = asteroids->addComponent<AsteroidPool>();
	asteroids->addComponent<AsteroidsMotion>();

	//Se crea la pool de Bullets
	Entity* bullets = entityManager_->addEntity();
	BulletsPool* bsPool = bullets->addComponent<BulletsPool>();
	bullets->addComponent<BulletsMotion>();

	//Se crea el caza
	Entity* ship = entityManager_->addEntity();
	ship->addComponent<Transform>(Vector2D(game_->getWindowWidth()/2, game_->getWindowHeight() / 2), Vector2D(), 50, 50, 0);
	ship->addComponent<Gun>(bsPool, 250);
	Health* health = ship->addComponent<Health>();
	ship->addComponent<FighterMotion>();
	
	//Se crea el gameManager
	Entity *gameManager = entityManager_->addEntity();
	gameManager->addComponent<ScoreManager>();
	gameManager->addComponent<GameLogic>(asPool, bsPool, health, GETCMP2(ship, Transform));
	gameManager->addComponent<GameCtrl>(asPool, health);	
}

void Server::closeGame() {
	delete entityManager_;
}

void Server::playerInputThread() {
	while(1){
		//recv from player

		//If x the do y, 
		//etc 
	}
}


void Server::waitUntilPlayerConnect() {
	//recv 

	//store player info
}

void Server::constructWorldMsg(){
	
}

void Server::start() {
	exit_ = false;

	waitUntilPlayerConnect();

	while (!exit_) {
		Uint32 startTime = game_->getTime();

		update();

		//Mandar info a los dos jugadores (de momento solo a un jugador)

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

