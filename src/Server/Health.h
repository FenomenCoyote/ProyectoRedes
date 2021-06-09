#pragma once
#include "Component.h"
class Health : public Component
{
public:
	Health();
	void init() override;
	void subtractHp();
	void resetHp();
	inline int getHp() { return hp_; }
private:
	int hp_;
};

