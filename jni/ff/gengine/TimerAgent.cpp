/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "TimerAgent.h"

#include "OptionAgent.h"

#include "FFNGApp.h"

//-----------------------------------------------------------------
    void
TimerAgent::own_init()
{
    m_timeinterval = OptionAgent::agent()->getAsInt("timeinterval", 100);
    m_lastTime = FFNGApp::getTicks();//FFNG SDL_GetTicks();
    m_nextTime = m_lastTime;
    m_deltaTime = 1;
    m_count = 0;
}
//-----------------------------------------------------------------
/**
 * Game is faster with pressed Shift.
 */
int
TimerAgent::getTimeInterval()
{
    int result = m_timeinterval;

    /* FFNG no faster game for android
    TODO button for this where appliable (replay?)
    if (SDL_GetModState() & KMOD_SHIFT) {
        result = m_timeinterval / 4;
    }
    */
    return result;
}
//-----------------------------------------------------------------
/**
 * Sleep fixed number miliseconds.
 */
    void
TimerAgent::own_update()
{
    m_count++;

    Uint32 now = FFNGApp::getTicks();//SDL_GetTicks();
    if (now < m_nextTime) {
        FFNGApp::delay/*FFNG SDL_Delay*/(m_nextTime - now);
    }

    now = FFNGApp::getTicks(); //FFNG SDL_GetTicks();
    //NOTE: every cycle have fixed time interval
    m_nextTime = now + getTimeInterval();

    m_deltaTime = now - m_lastTime;
    m_lastTime = now;
}
