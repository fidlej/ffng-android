#ifndef FFNGVIDEO_H_
#define FFNGVIDEO_H_

#include "FFNGSurface.h"

class FFNGVideo {
public:
	static SDL_Surface* setVideoMode(int width, int height, int bpp, int flags);
	static void setWindowCaption(const std::string &caption);
	static void flip(SDL_Surface *screen);
};

#endif /* FFNGVIDEO_H_ */
