#include "GameCtrl.h"
#include "Entity.h"
#include "InputHandler.h"

GameCtrl::GameCtrl(AsteroidPool* astPool, Health* health) : Component(ecs::GameCtrl),
		astPool(astPool), 
		health(health), 
		scoreManager_(nullptr)
{
}

GameCtrl::~GameCtrl() {
}

void GameCtrl::init() {
	scoreManager_ = GETCMP1_(ScoreManager);
}

//Inicia/resetea el juego cuando esta parado
void GameCtrl::update() {

	if (InputHandler::instance()->keyDownEvent()) {
		if (!scoreManager_->isRunning()) {
			scoreManager_->setRunning(true);
			game_->getAudioMngr()->resumeMusic();
			astPool->generateAsteroids(game_->getCfg()["gameLogic"]["asteroidsToGenerate"].as_int());
			// rest the score if the game is over
			if (scoreManager_->isGameOver()) {
				scoreManager_->setScore(0);
				health->resetHp();
				game_->getAudioMngr()->playMusic(Resources::ImperialMarch);
			}
			scoreManager_->setGameOver(false);
		}
	}
}
