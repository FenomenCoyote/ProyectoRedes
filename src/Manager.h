#pragma once

#include "ServerSDLGame.h"

class Entity;

class EntityManager {
public:
	EntityManager(ServerSDLGame* game);
	virtual ~EntityManager();

	void update();
	Entity* addEntity();


private:
	ServerSDLGame* game_;
	std::vector<std::unique_ptr<Entity>> entities;
};
