#pragma once

#include "Component.h"
#include "Transform.h"

class FighterCtrl: public Component {
public:
	FighterCtrl();
	FighterCtrl(SDL_Keycode left, SDL_Keycode right, SDL_Keycode ahead);
	virtual ~FighterCtrl();

	inline void setCtrlKeys(SDL_Keycode left, SDL_Keycode right,
			SDL_Keycode ahead) {
		left_ = left;
		right_ = right;
		ahead_ = ahead;
	}

	void init() override;
	void update() override;

private:
	SDL_Keycode left_;
	SDL_Keycode right_;
	SDL_Keycode ahead_;
	Transform *tr_;
	float rotSpeed_, speedLimit_, thrust_;
};

