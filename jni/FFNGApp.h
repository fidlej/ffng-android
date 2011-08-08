#ifndef FFNGAPP_H_
#define FFNGAPP_H_

#include "FFNGTypes.h"

class FFNGApp {
public:
	enum {
		GAMESTATE_LOADING = 1,
		GAMESTATE_WORLDMAP = 2,
		GAMESTATE_LEVEL = 3,
		GAMESTATE_OPTIONS = 4,
		GAMESTATE_MOVIE = 5,
		GAMESTATE_DEMO = 6,
		GAMESTATE_POSTER = 7
	};
	static int getTicks();
	static void delay(Uint32 ms);
	static void setGameState(int newState);
	static int getGameState();
	static void switchFishes(bool isBig);
	static void saveEffect();
	static bool pauseAndDisposeChance();
};

#endif /* FFNGAPP_H_ */
