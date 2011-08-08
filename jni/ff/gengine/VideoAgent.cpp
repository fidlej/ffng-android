/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "VideoAgent.h"

#include "Log.h"
#include "Path.h"
#include "ImgException.h"
#include "SDLException.h"
#include "LogicException.h"
#include "AgentPack.h"
#include "SimpleMsg.h"
#include "StringMsg.h"
#include "UnknownMsgException.h"
#include "OptionAgent.h"
#include "SysVideo.h"

//FFNG #include "SDL_image.h"
#include "FFNGVideo.h"
#include "FFNGApp.h"
#include <stdlib.h> // atexit()

//-----------------------------------------------------------------
/**
 * Init SDL and grafic window.
 * Register watcher for "fullscren" and "screen_*" options.
 * @throws SDLException if there is no usuable video mode
 */
    void
VideoAgent::own_init()
{
    m_screen = NULL;
    m_fullscreen = false;
    /* FFNG no need to (de)initialize video on android
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw SDLException(ExInfo("Init"));
    }
    atexit(SDL_Quit);
    */

    //FFNG no icon on android, at least this way... setIcon(Path::dataReadPath("images/icon.png"));

    registerWatcher("fullscreen");
    initVideoMode();
}
//-----------------------------------------------------------------
/**
 * Draw all drawer from list.
 * First will be drawed first.
 */
    void
VideoAgent::own_update()
{
    drawOn(m_screen);
    FFNGVideo::flip/*FFNG SDL_Flip*/(m_screen);
}
//-----------------------------------------------------------------
/**
 * Shutdown SDL.
 */
    void
VideoAgent::own_shutdown()
{
    //FFNG no quit for android... SDL_Quit();
}

//-----------------------------------------------------------------
/**
 * Load and set icon.
 * @throws ImgException
 */
/* FFNG no icon code for android
    void
VideoAgent::setIcon(const Path &file)
{
    SDL_Surface *icon = IMG_Load(file.getNative().c_str());
    if (NULL == icon) {
        throw ImgException(ExInfo("Load")
                .addInfo("file", file.getNative()));
    }

    SDL_WM_SetIcon(icon, NULL);
    SDL_FreeSurface(icon);
}
*/

//-----------------------------------------------------------------
/**
 * Init video mode along options.
 * Change window only when necessary.
 *
 * @throws SDLException when video mode cannot be made,
 * the old video mode remain usable
 */
    void
VideoAgent::initVideoMode()
{
    OptionAgent *options = OptionAgent::agent();
    int screen_width = options->getAsInt("screen_width", 640);
    int screen_height = options->getAsInt("screen_height", 480);

    SysVideo::setCaption(options->getParam("caption", "A game"));
    if (NULL == m_screen
            || m_screen->w != screen_width
            || m_screen->h != screen_height)
    {
    	/*FFNG*/FFNGApp::setGameState(options->getAsInt("game_state", FFNGApp::GAMESTATE_LOADING));
        changeVideoMode(screen_width, screen_height);
    }
}
//-----------------------------------------------------------------
/**
 * Init new video mode.
 * NOTE: m_screen pointer will change
 */
    void
VideoAgent::changeVideoMode(int screen_width, int screen_height)
{
    OptionAgent *options = OptionAgent::agent();
    int screen_bpp = options->getAsInt("screen_bpp", 32);
    int videoFlags = getVideoFlags();
    m_fullscreen = options->getAsBool("fullscreen", false);
    /* FFNG ignore video flags for android
    if (m_fullscreen) {
        videoFlags |= SDL_FULLSCREEN;
    }
    */

    //TODO: check VideoModeOK and available ListModes
    /* FFNG */ if(m_screen) { delete m_screen; m_screen = NULL; }
    SDL_Surface *newScreen =
        FFNGVideo::setVideoMode/*FFNG SDL_SetVideoMode*/(screen_width, screen_height, screen_bpp, videoFlags);
    /* FFNG no try without fullscreen for android
    if (NULL == newScreen && (videoFlags & SDL_FULLSCREEN)) {
        LOG_WARNING(ExInfo("unable to use fullscreen resolution, trying windowed")
                .addInfo("width", screen_width)
                .addInfo("height", screen_height)
                .addInfo("bpp", screen_bpp));

        videoFlags = videoFlags & ~SDL_FULLSCREEN;
        newScreen = SDL_SetVideoMode(screen_width, screen_height, screen_bpp,
                videoFlags);
    }
    */

    if (newScreen) {
        m_screen = newScreen;
        //NOTE: must be two times to change MouseState
        /* FFNG no mouse wapr for android
        SDL_WarpMouse(screen_width / 2, screen_height / 2);
        SDL_WarpMouse(screen_width / 2, screen_height / 2);
        */
    }
    else {
        throw SDLException(ExInfo("SetVideoMode")
                .addInfo("width", screen_width)
                .addInfo("height", screen_height)
                .addInfo("bpp", screen_bpp));
    }
}
//-----------------------------------------------------------------
/**
 * Obtain video information about best video mode.
 * @return best video flags
 */
    int
VideoAgent::getVideoFlags()
{
    int videoFlags  = 0;
    /* FFNG ignore all video flags for android
    videoFlags |= SDL_HWPALETTE;
    videoFlags |= SDL_ANYFORMAT;
    videoFlags |= SDL_SWSURFACE;
    */

    return videoFlags;
}
//-----------------------------------------------------------------
/**
 *  Toggle fullscreen.
 */
    void
VideoAgent::toggleFullScreen()
{
    /* FFNG no fullscreen toggling for android
    int success = SDL_WM_ToggleFullScreen(m_screen);
    if (success) {
        m_fullscreen = !m_fullscreen;
    }
    else {
        //NOTE: some platforms need reinit video
        changeVideoMode(m_screen->w, m_screen->h);
    }
    */
}
//-----------------------------------------------------------------
/**
 * Handle incoming message.
 * Messages:
 * - fullscreen ... toggle fullscreen
 *
 * @throws UnknownMsgException
 */
    void
VideoAgent::receiveSimple(const SimpleMsg *msg)
{
    if (msg->equalsName("fullscreen")) {
        OptionAgent *options = OptionAgent::agent();
        bool toggle = !(options->getAsBool("fullscreen"));
        options->setPersistent("fullscreen", toggle);
    }
    else {
        throw UnknownMsgException(msg);
    }
}
//-----------------------------------------------------------------
/**
 * Handle incoming message.
 * Messages:
 * - param_changed(fullscreen) ... handle fullscreen
 *
 * @throws UnknownMsgException
 */
    void
VideoAgent::receiveString(const StringMsg *msg)
{
    if (msg->equalsName("param_changed")) {
        std::string param = msg->getValue();
        if ("fullscreen" == param) {
            bool fs = OptionAgent::agent()->getAsBool("fullscreen");
            if (fs != m_fullscreen) {
                toggleFullScreen();
            }
        }
        else {
            throw UnknownMsgException(msg);
        }
    }
    else {
        throw UnknownMsgException(msg);
    }
}

