#include "Resources.h"
#include "SDL_macros.h"

vector<Resources::FontInfo> Resources::fonts_ {
//
		{ ARIAL16, "resources/fonts/ARIAL.ttf", 16 }, //
		{ ARIAL24, "resources/fonts/ARIAL.ttf", 24 } //
};

vector<Resources::ImageInfo> Resources::images_ {
//
		{ Blank, "resources/images/blank.png" }, //
		{ KeyBoardIcon, "resources/images/keyboard.png" }, //
		{ MouseIcon, "resources/images/mouse.png" }, //
		{ AIIcon, "resources/images/ai.png" }, //
		{ SpaceShips, "resources/images/spaceships.png" }, //
		{ Airplanes, "resources/images/airplanes.png" }, //
		{ Star, "resources/images/star.png" }, //
		{ Heart, "resources/images/heart.png" }, //
		{ Asteroid, "resources/images/asteroid.png" }, //
		{ WhiteRect, "resources/images/whiterect.png" } //
};

vector<Resources::TextMsgInfo> Resources::messages_ {
//
		{ HelloWorld, "Hello World", { COLOR(0xaaffffff) }, ARIAL16 }, //
		{ PressAnyKey, "Press Return Key ...", { COLOR(0xaaffbbff) }, ARIAL24 }, //
		{ GameWin, "Game Over, you won!", { COLOR(0x22ff66ff) }, ARIAL24 }, //
		{ GameLost, "Game Over, you lost...", { COLOR(0xff6622ff) }, ARIAL24 } //
};

vector<Resources::MusicInfo> Resources::musics_ {
//	
		{ ImperialMarch, "resources/sound/imperial_march.wav" } //
};

vector<Resources::SoundInfo> Resources::sounds_ {
//
		{ GunShot, "resources/sound/GunShot.wav"},
		{ Explosion, "resources/sound/explosion.wav"},
};

