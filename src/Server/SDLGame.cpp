#include "SDLGame.h"

#include <time.h>
#include <iostream>

#include "SRandBasedGenerator.h"


unique_ptr<SDLGame> SDLGame::instance_;

SDLGame::SDLGame(int width, int height) :
		width_(width), height_(height) {
	initSDL();
	initResources();
	cfg = jute::parser::parse_file("resources/cfg/asteroids.cfg");
}

SDLGame::~SDLGame() {
	closeResources();
	closeSDL();
}

void SDLGame::initSDL() {
	int sdlInit_ret = SDL_Init(SDL_INIT_EVERYTHING);
	assert(sdlInit_ret == 0);
}

void SDLGame::closeSDL() {
	SDL_Quit();
}

void SDLGame::initResources() {
	random_ = new SRandBasedGenerator();
	random_->init();
}

void SDLGame::closeResources() {
	delete random_;
}

