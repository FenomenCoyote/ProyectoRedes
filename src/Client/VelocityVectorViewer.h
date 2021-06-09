#pragma once

#include "Component.h"
#include "Transform.h"

class VelocityVectorViewer: public Component {
public:
	VelocityVectorViewer();
	virtual ~VelocityVectorViewer();
	void init() override;
	void draw() override;
private:
	Transform *tr_;
};

