#pragma once

#include "RandomNumberGenerator.h"

/*
 *
 */
class SRandBasedGenerator: public RandomNumberGenerator {
public:
	SRandBasedGenerator();
	SRandBasedGenerator(unsigned seed);
	virtual ~SRandBasedGenerator();
	virtual void init();
	virtual int nextInt();
	virtual int nextInt(int low, int high);
private:
	unsigned seed_;
};

