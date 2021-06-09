#pragma once

#include "Component.h"

class ScoreManager: public Component {
public:
	ScoreManager();
	virtual ~ScoreManager();

	int getScore() {
		return score_;
	}

	void setScore(int Score) {
		score_ = Score;
	}

	void addScore(int plus) { score_ += plus; }

	bool isRunning() const {
		return running_;
	}

	void setRunning(bool running) {
		running_ = running;
	}

	void setWin(bool w) { win = w; }
	bool hasWon() { return win; }

	bool isGameOver() {
		return gameOver_;
	}

	void setGameOver(bool g) {
		gameOver_ = g;
	}
private:
	bool running_;
	bool gameOver_;
	bool win;
	int score_;
};
