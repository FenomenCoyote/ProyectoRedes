#include "SDL_macros.h"
#include "Client.h"
#include <assert.h>

#include "InputHandler.h"

#include "SDLGame.h"

#include "SDL_macros.h"
#include "ClientMsg.h"
#include "ServerMsg.h"

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

constexpr float time_wait = 1000.f/60.0f;

mutex mClient;

Client::Client(const char* address, const char* port) :
		game_(nullptr), //
		exit_(false), 
		socket(address, port), 
		inGame(false),
		msgText(nullptr)
{
	initGame();
}

Client::~Client() {

}

void Client::initGame() {

	game_ = SDLGame::init("Asteroids PVP Arena", _WINDOW_WIDTH_, _WINDOW_HEIGHT_);

	game_->getAudioMngr()->playMusic(Resources::ImperialMarch);
	game_->getAudioMngr()->setMusicVolume(20);
	game_->getAudioMngr()->setChannelVolume(5); //Se deja la musica preparada para hacer un resume
	game_->getAudioMngr()->pauseMusic();

}

void Client::start() {
	exit_ = false;

	//net thread
	std::thread([this](){
		this->netThread();
	}).detach();

	msgText = game_->getTextureMngr()->getTexture(Resources::PressAnyKey);

	while (!exit_) {
		Uint32 startTime = game_->getTime();

		handleInput();
		render();

		Uint32 frameTime = game_->getTime() - startTime;
		if (frameTime < time_wait)
			SDL_Delay(time_wait - frameTime);
	}
}

void Client::stop() {
	exit_ = true;	
}

void Client::netThread() {
	while(1){
		ServerMsg::ServerMsg msg;
		socket.recv(msg);

		mClient.lock();
		if(inGame){
			if(msg.type == ServerMsg::_ENDING_GAME){
				inGame = false;
				game_->getAudioMngr()->pauseMusic();

				asteroids.clear();
				bullets.clear();

				//ships are send to oblivion, although not really needed
				ship1.posX = 5000;
				ship2.posY = 5000;

				msgText = msg.won ? game_->getTextureMngr()->getTexture(Resources::TextureId::GameWin) : 
									game_->getTextureMngr()->getTexture(Resources::TextureId::GameLost);
			} 
			else if(msg.type == ServerMsg::_WORLD_STATE){
				//swap the state
				asteroids = msg.asteroids;
				bullets = msg.bullets;
				ship1 = msg.ship1;
				ship2 = msg.ship2;
				if(msg.sound == ServerMsg::_ASTEROID_COLLISION_)
					game_->getAudioMngr()->playChannel(Resources::AudioId::Explosion, 0);
			}
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
		//Send ready msg if not in game
		if (!inGame && ih->isKeyDown(SDLK_RETURN)) {
			inGame = true;
			ClientMsg::InputMsg msg(ClientMsg::_READY_);
			socket.send(msg, socket);

			return;
		}

		if(!inGame)
			return;

		//In game inputs
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
			game_->getAudioMngr()->playChannel(Resources::AudioId::GunShot, 0);
		}
	}

}

void Client::render() {
	SDL_SetRenderDrawColor(game_->getRenderer(), COLOR(0x00AAAAFF));
	SDL_RenderClear(game_->getRenderer());

	//Render los objetos que tenga
	mClient.lock();
	if(inGame){
		//render asteroids
		for(const ServerMsg::ServerMsg::ObjectInfo& o : asteroids){
			SDL_Rect dest = RECT(o.posX, o.posY, o.width, o.height);
			game_->getTextureMngr()->getTexture(Resources::Asteroid)->render(dest, o.rot);
		}
		//render bullets
		for(const ServerMsg::ServerMsg::ObjectInfo& o : bullets){
			SDL_Rect dest = RECT(o.posX, o.posY, o.width, o.height);
			game_->getTextureMngr()->getTexture(Resources::WhiteRect)->render(dest, o.rot);
		}
		//render p1 ship
		SDL_Rect dest = RECT(ship1.posX, ship1.posY, ship1.width, ship1.height);
		SDL_Rect src = RECT(47, 90, 207, 250);
		game_->getTextureMngr()->getTexture(Resources::Airplanes)->render(dest, ship1.rot, src);

		//render p2 ship
		dest = RECT(ship2.posX, ship2.posY, ship2.width, ship2.height);
		src = RECT(305, 616, 207, 250);
		game_->getTextureMngr()->getTexture(Resources::Airplanes)->render(dest, ship2.rot, src);
	}
	else {
		//Render msg
		msgText->render(
				game_->getWindowWidth() / 2 - msgText->getWidth() / 2,
				game_->getWindowHeight() - msgText->getHeight() - 50);
	}
	mClient.unlock();

	SDL_RenderPresent(game_->getRenderer());
}

