#ifndef FFNGINPUTEVENT_H_
#define FFNGINPUTEVENT_H_

#include <string>
#include "FFNGTypes.h"

#define KMOD_NONE 0		// key modificator (ctrl, alt, shift) - LevelInput::LevelInput

#define SDL_BUTTON_LEFT		1
#define SDL_BUTTON_MIDDLE	2
#define SDL_BUTTON_RIGHT	3

struct SDL_MouseButtonEvent {
	int x,y;
	Uint8 button;
};

#define SDL_BUTTON(x) (1<<x)

typedef enum {
	SDLK_UP = 1,
	SDLK_DOWN,
	SDLK_LEFT,
	SDLK_RIGHT,
	SDLK_SPACE,
	SDLK_F2,
	SDLK_F3,
	SDLK_F10,
	SDLK_BACKSPACE,
	SDLK_MINUS,
	SDLK_PLUS,
	SDLK_ESCAPE,

	// direct control events for the fishes
	SDLK_i,
	SDLK_k,
	SDLK_j,
	SDLK_l,
	SDLK_w,
	SDLK_s,
	SDLK_a,
	SDLK_d,

	SDLK_LAST
} SDLKey;

struct SDL_keysym {
	//Uint8 scancode;			/**< hardware specific scancode */
	SDLKey sym;			/**< SDL virtual keysym */
	//SDLMod mod;			/**< current key modifiers */
	//Uint16 unicode;			/**< translated character */
};

struct SDL_Event {
	enum {
		QUIT = 1,
		KEYDOWN = 2,
		KEYUP = 3,
		MOUSEBUTTONDOWN = 4
	};
	int type;
	SDL_keysym key;

	// mouse
	SDL_MouseButtonEvent button;
};

class FFNGInputEvent {
	static Uint8 pressed[SDLK_LAST];
public:
	static Uint8 getTouch(int *x, int *y);
	static std::string getKeyName(SDLKey sym);
	static bool pollEvent(SDL_Event *event);
	static Uint8* getKeyState(int *numkeys);
};

#endif /* FFNGINPUTEVENT_H_ */
