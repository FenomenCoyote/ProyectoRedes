#include "ServerSDLGame.h"

#include <time.h>
#include <iostream>

#include "SRandBasedGenerator.h"


unique_ptr<ServerSDLGame> ServerSDLGame::instance_;

ServerSDLGame::ServerSDLGame(int width, int height) :
		width_(width), height_(height) {
	initSDL();
	initResources();
	cfg = jute::parser::parse_file("resources/cfg/asteroids.cfg");
}

ServerSDLGame::~ServerSDLGame() {
	closeResources();
	closeSDL();
}

void ServerSDLGame::initSDL() {
	int sdlInit_ret = SDL_Init(SDL_INIT_EVERYTHING);
	assert(sdlInit_ret == 0);
}

void ServerSDLGame::closeSDL() {
	SDL_Quit();
}

void ServerSDLGame::initResources() {
	random_ = new SRandBasedGenerator();
	random_->init();
}

void ServerSDLGame::closeResources() {
	delete random_;
}

