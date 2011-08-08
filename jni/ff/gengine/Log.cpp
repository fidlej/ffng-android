/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Log.h"

#include "ExInfo.h"

#include <stdlib.h>
#include <stdio.h>
#include <android/log.h>

//int Log::ms_logLevel = LEVEL_INFO;
int Log::ms_logLevel = LEVEL_DEBUG;
//-----------------------------------------------------------------
/**
 * Print log info.
 * @param level priority level
 * @param file source file
 * @param line line number in source file
 * @param info detail info
 */
void 
Log::log(int level, const char *file, int line,
        const ExInfo &info) throw()
{
    if (true || level <= ms_logLevel) {
        const char *levelName;
        android_LogPriority andLevelName;
        switch (level) {
            case LEVEL_DEBUG:
                levelName = "DEBUG";
                andLevelName = ANDROID_LOG_DEBUG;
                break;
            case LEVEL_INFO:
                levelName = "INFO";
                andLevelName = ANDROID_LOG_INFO;
                break;
            case LEVEL_WARNING:
                levelName = "WARNING";
                andLevelName = ANDROID_LOG_WARN;
                break;
            case LEVEL_ERROR:
                levelName = "ERROR";
                andLevelName = ANDROID_LOG_ERROR;
                break;
            default:
                levelName = "UNKNOWN";
                andLevelName = ANDROID_LOG_UNKNOWN;
                break;
        }

        fprintf(stderr, "%s:%d: %s %s\n", file, line, levelName, info.what());
        __android_log_print(andLevelName, "FFNG", "%s:%d: %s %s\n", file, line, levelName, info.what());
    }
}
