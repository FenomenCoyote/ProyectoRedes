#pragma once

#include <SDL.h>

#include "ecs.h"
#include "ServerSDLGame.h"

class Entity;


class Component {
protected:
	Entity* entity_;
	ServerSDLGame* game_;
	ecs::CmpId id_;
public:
	Component(ecs::CmpId id);
	virtual ~Component();

	void setEntity(Entity* entity) {
		entity_ = entity;
	}

	void setGame(ServerSDLGame* game) {
		game_ = game;
	}

	void setId(ecs::CmpId id) {
		id_ = id;
	}

	ecs::CmpId getId() const {
		return id_;
	}

	virtual void init() {
	}
	virtual void update() {
	}
	virtual void draw() {
	}

};

