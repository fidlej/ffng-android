/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "EffectMirror.h"

#include "SurfaceLock.h"
#include "PixelTool.h"

const char *EffectMirror::NAME = "mirror";
//-----------------------------------------------------------------
/**
 * Mirror effect. Draw left side inside.
 * The pixel in the middle will be used as a mask.
 * NOTE: mirror object should be drawn as the last.
 */
void
EffectMirror::blit(SDL_Surface *screen, SDL_Surface *surface, int x, int y)
{
	screen->blitMirror(surface, x, y, MIRROR_BORDER);

	/* FFNG
    SurfaceLock lock1(screen);
    SurfaceLock lock2(surface);

    Color mask = PixelTool::getColor(surface,
            surface->w / 2, surface->h / 2);

    for (int py = 0; py < surface->h; ++py) {
        for (int px = 0; px < surface->w; ++px) {
            Color pixel = PixelTool::getColor(surface, px, py);
            if (px > MIRROR_BORDER && PixelTool::colorEquals(pixel, mask)) {
                Color sample = PixelTool::getColor(screen,
                        x - px + MIRROR_BORDER, y + py);
                PixelTool::putColor(screen, x + px, y + py, sample);
            }
            else {
                if (pixel.a == 255) {
                    PixelTool::putColor(screen, x + px, y + py, pixel);
                }
            }
        }
    }
    */
}

