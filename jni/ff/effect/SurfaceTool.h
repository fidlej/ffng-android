#ifndef HEADER_SURFACETOOL_H
#define HEADER_SURFACETOOL_H

#include "Color.h"
//FFNG #include "SDL.h"
#include "FFNGSurface.h"

/**
 * Surface rutines.
 */
class SurfaceTool {
    public:
        static SDL_Surface *createEmpty(SDL_Surface *surface,
                int width=0, int height=0);
        static SDL_Surface *createTransparent(int w, int h,
                const Color &transparent);
        static SDL_Surface *createClone(SDL_Surface *surface);
        static void alphaFill(SDL_Surface *surface, SDL_Rect *dstrect,
                const Color &color);
};

#endif
