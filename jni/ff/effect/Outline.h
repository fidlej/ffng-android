#ifndef HEADER_OUTLINE_H
#define HEADER_OUTLINE_H

#include "Color.h"
//FFNG #include "SDL.h"
#include "FFNGSurface.h"
#include "FFNGTypes.h"

/**
 * Outline around picture.
 * Used to outline rendered text.
 */
class Outline {
    private:
        int m_width;
        Color m_color;
        Uint32 m_pixel;
    private:
        SDL_Surface *cloneSurface(SDL_Surface *surface);
        void precomputePixel(/*FFNG SDL_PixelFormat *format*/);
        void drawOneLayer(SDL_Surface *surface, Uint32 bgKey);
        void drawAlongCopy(SDL_Surface *surface, Uint32 bgKey,
                SDL_Surface *copy);
        void fillNeighbourhood(SDL_Surface *surface, Uint32 bgKey,
                int x, int y);
    public:
        Outline(const Color &color, int width);
        void drawOnColorKey(SDL_Surface *surface);
        void drawOn(SDL_Surface *surface, Uint32 bgKey);
};

#endif
