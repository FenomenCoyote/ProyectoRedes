#pragma once

#include "Component.h"
#include "Transform.h"
#include "Gun.h"

class FighterCtrl: public Component {
public:
	FighterCtrl();
	virtual ~FighterCtrl();

	void init() override;

	void turnLeft();
	void turnRight();
	void goAhead();
	void shoot();

private:
	Transform *tr_;
	Gun *gun_;
	float rotSpeed_, speedLimit_, thrust_;
};

