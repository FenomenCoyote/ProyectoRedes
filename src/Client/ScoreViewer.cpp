#include "ScoreViewer.h"

#include "Texture.h"
#include "Entity.h"
#include "SDL_macros.h"

ScoreViewer::ScoreViewer() :
		Component(ecs::ScoreViewer), //
		scoreManager_(nullptr) {
}

ScoreViewer::~ScoreViewer() {
}

void ScoreViewer::init() {
	scoreManager_ = GETCMP1_(ScoreManager);
}

void ScoreViewer::draw() {
	//Si el juego esta en marcha, se renderiza el score
	
	Texture score(game_->getRenderer(), "Score: " + to_string(scoreManager_->getScore()), (game_->getFontMngr()->getFont(Resources::ARIAL24)),
		{ COLOR(0x1111aaff) });
	score.render(game_->getWindowWidth() / 2 - score.getWidth() / 2, 10);

	//Si el juego NO esta en marcha, se renderiza el mensaje de si ha ganado o no
	if (!scoreManager_->isRunning()) {
		Texture* t;
		if (scoreManager_->isGameOver()) { 
			if(scoreManager_->hasWon()) t = game_->getTextureMngr()->getTexture(Resources::GameWin);
			else t = game_->getTextureMngr()->getTexture(Resources::GameLost);	
			t->render(game_->getWindowWidth() / 2 - t->getWidth() / 2, game_->getWindowWidth() / 2 - t->getHeight() / 2);
		}
	}
}
