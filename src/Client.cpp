#include "SDL_macros.h"
#include "Client.h"
#include <assert.h>

#include "InputHandler.h"

#include "SDLGame.h"

#include "SDL_macros.h"
#include "ClientMsg.h"
#include "ServerMsg.h"

#include <thread>
#include <mutex>

using namespace std;

mutex mClient;

Client::Client(const char* address, const char* port, const char* nick) :
		game_(nullptr), //
		exit_(false), 
		socket(address, port), 
		inGame(false)
{
	initGame();
}

Client::~Client() {
	closeGame();
}

void Client::initGame() {

	game_ = SDLGame::init("Asteroids PVP Arena", _WINDOW_WIDTH_, _WINDOW_HEIGHT_);

	game_->getAudioMngr()->playMusic(Resources::ImperialMarch);
	game_->getAudioMngr()->setMusicVolume(20);
	game_->getAudioMngr()->setChannelVolume(5); //Se deja la musica preparada para hacer un resume
	game_->getAudioMngr()->pauseMusic();

}

void Client::closeGame() {

}

void Client::start() {
	exit_ = false;

	//net thread
	std::thread([this](){
		this->netThread();
	}).detach();

	while (!exit_) {
		Uint32 startTime = game_->getTime();

		handleInput();
		render();

		Uint32 frameTime = game_->getTime() - startTime;
		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
}

void Client::stop() {
	exit_ = true;
	
}

void Client::netThread() 
{
	while(1){
		ServerMsg::ServerMsg msg;
		socket.recv(msg);

		mClient.lock();
		if(inGame){
			if(msg.type == ServerMsg::_ENDING_GAME){
				inGame = false;
			} 
			else if(msg.type == ServerMsg::_WORLD_STATE){
				asteroids = msg.asteroids;
				bullets = msg.bullets;
				ship = msg.ship;
			} 
			else 
				assert(false);	
		}
		else {
			inGame = true;
		}
		mClient.unlock();
		
	}
}

void Client::handleInput() {

	InputHandler *ih = InputHandler::instance();

	ih->update();

	if (ih->keyDownEvent()) {
		if (ih->isKeyDown(SDLK_ESCAPE)) {
			exit_ = true;
			ClientMsg::InputMsg msg(ClientMsg::_LOGOUT_);
			socket.send(msg, socket);
		}

		if (ih->isKeyDown(SDLK_f)) {
			int flags = SDL_GetWindowFlags(game_->getWindow());
			if (flags & SDL_WINDOW_FULLSCREEN) {
				SDL_SetWindowFullscreen(game_->getWindow(), 0);
			} else {
				SDL_SetWindowFullscreen(game_->getWindow(),
						SDL_WINDOW_FULLSCREEN);
			}
		}

		if (ih->isKeyDown(SDLK_SPACE)) {
			inGame = true;
			ClientMsg::InputMsg msg(ClientMsg::_READY_);
			socket.send(msg, socket);
		}

		if(!inGame)
			return;

		if (ih->isKeyDown(SDLK_UP)) {
			ClientMsg::InputMsg msg(ClientMsg::InputId::_AHEAD_);
			socket.send(msg, socket);
		}
		if (ih->isKeyDown(SDLK_LEFT)) {
			ClientMsg::InputMsg msg(ClientMsg::InputId::_LEFT_);
			socket.send(msg, socket);
		}
		else if (ih->isKeyDown(SDLK_RIGHT)) {
			ClientMsg::InputMsg msg(ClientMsg::InputId::_RIGHT_);
			socket.send(msg, socket);
		}
		if (ih->isKeyDown(SDLK_SPACE)) {
			ClientMsg::InputMsg msg(ClientMsg::InputId::_SHOOT_);
			socket.send(msg, socket);
		}
	}

}


void Client::render() {
	SDL_SetRenderDrawColor(game_->getRenderer(), COLOR(0x00AAAAFF));
	SDL_RenderClear(game_->getRenderer());

	//Render los objetos que tenga
	mClient.lock();
	if(inGame){
		for(ServerMsg::ServerMsg::ObjectInfo& o : asteroids){
			SDL_Rect dest = RECT(o.posX, o.posY, o.width, o.height);
			game_->getTextureMngr()->getTexture(Resources::Asteroid)->render(dest, o.rot);
		}
		for(ServerMsg::ServerMsg::ObjectInfo& o : bullets){
			SDL_Rect dest = RECT(o.posX, o.posY, o.width, o.height);
			game_->getTextureMngr()->getTexture(Resources::WhiteRect)->render(dest, o.rot);
		}
		SDL_Rect dest = RECT(ship.posX, ship.posY, ship.width, ship.height);
		game_->getTextureMngr()->getTexture(Resources::Airplanes)->render(dest, ship.rot);
	}
	else {
		Texture *hitanykey = game_->getTextureMngr()->getTexture(
				Resources::PressAnyKey);
		hitanykey->render(
				game_->getWindowWidth() / 2 - hitanykey->getWidth() / 2,
				game_->getWindowHeight() - hitanykey->getHeight() - 50);
	}
	mClient.unlock();

	SDL_RenderPresent(game_->getRenderer());
}

