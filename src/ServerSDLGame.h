#pragma once

#include <SDL.h>
#include "RandomNumberGenerator.h"
#include "jute.h"

#include <string>
#include <memory>
#include <assert.h>

using namespace std;

class ServerSDLGame {
public:
	virtual ~ServerSDLGame();

	ServerSDLGame(ServerSDLGame&) = delete;
	ServerSDLGame& operator=(ServerSDLGame&) = delete;

	inline static ServerSDLGame* init(int width, int height) {
		assert(instance_.get() == nullptr);
		instance_.reset(new ServerSDLGame(width, height));
		return instance_.get();
	}

	inline static ServerSDLGame* instance() {
		assert(instance_.get() != nullptr);
		return instance_.get();
	}

	inline int getWindowWidth() const {
		return width_;
	}

	inline int getWindowHeight() const {
		return height_;
	}

	inline RandomNumberGenerator* getRandGen() const {
		return random_;
	}

	inline Uint32 getTime() {
		return SDL_GetTicks();
	}

	inline jute::jValue& getCfg() { return cfg; }

private:
	ServerSDLGame(int width, int height);

	void initSDL(); // initialize SDL
	void closeSDL(); // close SDL
	void initResources(); // initialize the SDLResources object with the data at the top of this file
	void closeResources(); // close the SDLResources object (frees all memory)

protected:

	RandomNumberGenerator *random_;

	int width_; // window width
	int height_; // window height

	jute::jValue cfg;

	static unique_ptr<ServerSDLGame> instance_;

};

