#include "ScoreManager.h"

ScoreManager::ScoreManager() :
		Component(ecs::ScoreManager), 
		score_(0),
		running_(false), 
		gameOver_(false),
		win(false)
{
}

ScoreManager::~ScoreManager() {
}

