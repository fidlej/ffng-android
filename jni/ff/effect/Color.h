#ifndef HEADER_COLOR_H
#define HEADER_COLOR_H

//FFNG #include "SDL.h"
#include "FFNGTypes.h"

/**
 * Constructor for SDL_Color.
 */
class Color/*FFNG : public SDL_Color*/ {
    public:
		Uint8 r,g,b,a;
		Color()
		{
			Color(0, 0, 0, 0);
		}
		Color(Uint32 color)
		{
			// TODO make sure it isn't abgr or argb
			/*
			r = (color & 0xff000000) >> 24;
			g = (color & 0x00ff0000) >> 16;
			b = (color & 0x0000ff00) >> 8;
			a = (color & 0x000000ff);
			*/
			a = (color & 0xff000000) >> 24;
			r = (color & 0x00ff0000) >> 16;
			g = (color & 0x0000ff00) >> 8;
			b = (color & 0x000000ff);
		}
        Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha=255)
        {
            this->r = red;
            this->g = green;
            this->b = blue;
            this->a = alpha;
        }
        Uint32 get32() const
        {
        	// TODO make sure it isn't abgr or argb
			//return (r << 24) | (g << 16) | (b << 8) | a;
        	return (a << 24) | (r << 16) | (g << 8) | b;
        }
};

#endif
