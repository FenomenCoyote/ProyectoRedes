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
#include "FighterCtrl.h"

#include "AsteroidPool.h"
#include "AsteroidsMotion.h"

#include "BulletsMotion.h"

#include "ServerMsg.h"

#include "Client/ClientMsg.h"

using namespace std;

constexpr float timeWait = 1000.0f/60.0f;

Server::Server(const char* address, const char* port) :
		game_(nullptr), //
		entityManager_(nullptr), //
		exit_(false), start_(false),
		socket(address, port),
		clientSocket(nullptr),
		asPool(nullptr), bsPool(nullptr),
		shipTr(nullptr), shipCtrl(nullptr), 
		health(nullptr)
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
	shipTr = ship_->addComponent<Transform>(Vector2D(game_->getWindowWidth()/2, game_->getWindowHeight() / 2), Vector2D(), 50, 50, 0);
	ship_->addComponent<Gun>(bsPool, 250);
	health = ship_->addComponent<Health>();
	shipCtrl = ship_->addComponent<FighterCtrl>();
	ship_->addComponent<FighterMotion>();
	
	//Se crea el gameManager
	Entity *gameManager = entityManager_->addEntity();
	gameManager->addComponent<GameLogic>(asPool, bsPool, health, shipTr);
}

void Server::closeGame() {
	delete entityManager_;
}

void Server::playerInputThread() {
	while(1){

		ClientMsg::Msg msg;
		clientSocket->recv(msg);

		//pillar mutex

		switch (ClientMsg::ClientMsgId)
		{
		case ClientMsg::_INPUT_:
			switch (ClientMsg::InputId)
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
				assert(false);
				break;
			}
			break;
		case ClientMsg::_READY_:
			start_ = true;
			break;
		case ClientMsg::_LOGOUT_:
			exit_ = true;
			break;
		default:
			assert(false);
			break;
		}
		//Soltar mutex
	}
}


void Server::waitUntilPlayerConnect() {

	//ClientMsg msg;
	ClientMsg::Msg msg;
	clientSocket = (Socket*)1;
    socket.recv(msg, clientSocket);

	assert(msg.type != ClientMsg::_LOGIN_)
}

void Server::sendWorldState(){
	if(clientSocket == nullptr)
		return;
	//Crear info
	ServerMsg::WorldStateMsg msg(asPool, bsPool, shipTr, health);
	if(asPool->anyColision())
		msg.setSound(ServerMsg::SoundId::_ASTEROID_COLLISION_);

	//Mandar info a los dos jugadores (de momento solo a un jugador)
	socket.send(msg, *clientSocket);
}

void Server::start() {
	exit_ = false;

	waitUntilPlayerConnect();

	//Make InputThread

	//pillar mutex
	while(!start_){
		//Soltar mutex
		SDL_Delay(timeWait);
		//Pillax mutex
	}
	//Soltar mutex

	ServerMsg::StartingGameMsg msg;
	socket.send(msg, clientSocket);

	while (!exit_) {
		//Soltar mutex
		Uint32 startTime = game_->getTime();

		update();

		sendWorldState();
	
		Uint32 frameTime = game_->getTime() - startTime;
		if (frameTime < timeWait)
			SDL_Delay(timeWait - frameTime);

		//Pillar mutex
	}

	ServerMsg::EndingGameMsg msg;
	socket.send(msg, clientSocket);
}

void Server::stop() {
	exit_ = true;
}

void Server::update() {
	entityManager_->update();
}

