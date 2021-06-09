#include "SDL_macros.h"
#include "Client.h"
#include <assert.h>

#include "InputHandler.h"

#include "SDLGame.h"

#include "SDL_macros.h"


using namespace std;

Client::Client() :
		game_(nullptr), //
		exit_(false) {
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

void Client::handleInput() {

	InputHandler *ih = InputHandler::instance();

	ih->update();

	if (ih->keyDownEvent()) {
		if (ih->isKeyDown(SDLK_ESCAPE)) {
			exit_ = true;
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

		if (ih->isKeyDown(SDLK_UP)) {
			//mensaje de ir hacia arriba
		}
		else if (ih->isKeyDown(SDLK_DOWN)) {
			//mensaje de ir hacia abajo
		}
		if (ih->isKeyDown(SDLK_LEFT)) {
			//mensaje de ir hacia izquierda
		}
		else if (ih->isKeyDown(SDLK_RIGHT)) {
			//mensaje de ir hacia derecha
		}
		if (ih->isKeyDown(SDLK_SPACE)) {
			//mensaje de disparar
		}
	}

}


void Client::render() {
	SDL_SetRenderDrawColor(game_->getRenderer(), COLOR(0x00AAAAFF));
	SDL_RenderClear(game_->getRenderer());

	//Render los objetos que tenga

	SDL_RenderPresent(game_->getRenderer());
}

