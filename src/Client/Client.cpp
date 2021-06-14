#include "SDL_macros.h"
#include "Client.h"
#include <assert.h>

#include "InputHandler.h"

#include "SDLGame.h"

#include "SDL_macros.h"
#include "ClientMsg.h"
#include "ServerMsg.h"


using namespace std;

Client::Client(const char* address, const char* port, const char* nick) :
		game_(nullptr), //
		exit_(false), 
		socket(address, port), 
		inGame(false) {
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

	ClientMsg::LoginMsg msg;
	socket.send(msg, socket);
}

void Client::closeGame() {

}

void Client::start() {
	exit_ = false;

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
		//Pillar mutex
		if(inGame){
			//Soltar mutex
			ServerMsg::Msg msg;
			socket.recv(msg);

			//Pillar mutex
			if(msg.type == ServerMsg::EndingGameMsg){
				inGame = false;
			} 
			else if(msg.type == ServerMsg::WorldStateMsg){
				msg = static_cast<ServerMsg::WorldStateMsg>(msg);
				asteroids = msg.


			} 
			else 
				assert(false);

			//Liberar mutex
		}
		else {
			//Soltar mutex
			ServerMsg::StartingGameMsg msg;
			socket.recv(msg);

			//Pillar mutex
			inGame = true;
			//Soltar mutex
		}
	}
}

void Client::handleInput() {

	InputHandler *ih = InputHandler::instance();

	ih->update();

	if (ih->keyDownEvent()) {
		if (ih->isKeyDown(SDLK_ESCAPE)) {
			exit_ = true;
			ClientMsg::LogoutMsg msg;
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

		if(!inGame)
			return;

		if (ih->isKeyDown(SDLK_SPACE) && !inGame) {
			inGame = true;
			ClientMsg::ReadyMsg msg;
			socket.send(msg, socket);
		}
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
	if(inGame){

	}
	else {

	}

	SDL_RenderPresent(game_->getRenderer());
}

