#ifndef HEADER_PIXELTOOL_H
#define HEADER_PIXELTOOL_H

//#include "SDL.h"
#include "Color.h"
#include "FFNGSurface.h"

/**
 * Pixel rutines.
 */
class PixelTool {
    public:
        static bool colorEquals(const Color &color1,
                const Color &color2);
        static Uint32 convertColor(/*FFNGSDL_PixelFormat *format,*/
                const Color &color);

        static Color getColor(SDL_Surface *surface, int x, int y);
        static void putColor(SDL_Surface *surface, int x, int y,
                const Color &color);

        static Uint32 getPixel(SDL_Surface *surface, int x, int y);
        static void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

        static Uint32 unpackPixel(Uint8 bpp, Uint8 *p);
        static void packPixel(Uint8 bpp, Uint8 *p, Uint32 pixel);
};

#endif
