/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Path.h"

#include "Log.h"
#include "OptionAgent.h"
#include "Dialog.h"
#include "FsPath.h"
#include "PathException.h"

#include <stdio.h>
#include "FFNGFiles.h"
#include <android/log.h>

//-----------------------------------------------------------------
    Path::Path(const std::string &file, const FileType &type)
: m_path(file),
  m_type(type)
{
    /* empty */
}
//-----------------------------------------------------------------
/**
 * Try return user data path,
 * otherwise return system data path.
 * NOTE: OptionAgent must be initialized before this
 *
 * @param file path to file
 * @param writeable whether we want write to the file
 * @return path to user or system file
 */
    Path
Path::dataPath(const std::string &file, bool writeable)
{
    Path datapath = dataUserPath(file);

    __android_log_print(ANDROID_LOG_DEBUG, "FFNG", "Path::dataPath 1 %s (%s)", file.c_str(), writeable ? "w":"r");
    if (!datapath.exists())  {
    	bool try_write = false;
        if (writeable) {
            try {
                LOG_INFO(ExInfo("creating path")
                        .addInfo("path", datapath.getNative()));
                /*FFNG FsPath::createPath(datapath.getPosixName());*/
                FFNGFiles::createPath(datapath.getPosixName());

                try_write = datapath.testWrite();
            }
            catch (PathException &e) {
                LOG_WARNING(e.info());
            }
        }

        if (!try_write) {
            datapath = dataSystemPath(file);
        }
    }

    //__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "Path::dataPath 2 %s (%s)", datapath.getPosixName().c_str(), (datapath.getType() == Path::EXTERNAL) ? "external":"internal");
    return datapath;
}
//-----------------------------------------------------------------
    Path
Path::dataReadPath(const std::string &file)
{
    return dataPath(file, false);
}
//-----------------------------------------------------------------
    Path
Path::dataWritePath(const std::string &file)
{
    return dataPath(file, true);
}
//-----------------------------------------------------------------
/**
 * Return path to system file.
 * Path does not need to exist.
 */
    Path
Path::dataSystemPath(const std::string &file)
{
    std::string systemdir = OptionAgent::agent()->getParam("systemdir");
    return constructPath(systemdir, file, INTERNAL);
}
//-----------------------------------------------------------------
/**
 * Return path to user file.
 * Path does not need to exist.
 */
    Path
Path::dataUserPath(const std::string &file)
{
    std::string userdir = OptionAgent::agent()->getParam("userdir");
    return constructPath(userdir, file, EXTERNAL);
}
//-----------------------------------------------------------------
/**
 * Create path to the given file in the given directory.
 * Tries to use path to a localized resource if it exists.
 */
    Path
Path::constructPath(const std::string &dir, const std::string &file, const FileType &type)
{
    std::string localized = localizePath(file);
    Path localizedPath = Path(FsPath::join(dir, localized), type);

    if (localized == file) {
        return localizedPath;
    } else if (localizedPath.exists()) {
        LOG_INFO(ExInfo("localized")
                .addInfo("path", localizedPath.getNative()));
        return localizedPath;
    } else {
        return Path(FsPath::join(dir, file), type);
    }
}
//-----------------------------------------------------------------
/**
 * Return path to a localized resource or the original path.
 * The localized path has format: <name>__<lang>.<extension>
 *
 * @param original original path
 * @return localized path if it is meaningful or the original path
 */
    std::string
Path::localizePath(const std::string &original)
{
    std::string::size_type dotPos = original.rfind('.');
    if (dotPos == std::string::npos) {
        return original;
    }

    std::string lang = OptionAgent::agent()->getParam("lang");
    if (Dialog::DEFAULT_LANG == lang || lang.empty()) {
        return original;
    }

    std::string appendix = "__" + lang;

    std::string::size_type dirPos = original.rfind('/');
    if (dirPos != std::string::npos && dotPos < dirPos) {
        return original;
    }

    std::string path = original;
    path.insert(dotPos, appendix);
    return path;
}

//-----------------------------------------------------------------
std::string
Path::getNative() const
{
    return FsPath::getNative(m_path);
}
//-----------------------------------------------------------------
bool
Path::exists() const
{
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "Path::exists 1 %s type=%d", m_path.c_str(), m_type);
	bool result = FFNGFiles::exists(m_path, m_type);
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "Path::exists 2 %s", result ? "exists" : "!exists");
	return result;
//    return FsPath::exists(m_path);
}
//-----------------------------------------------------------------
std::string
Path::read() const
{
	__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "Path::read 1 %s (%s)", m_path.c_str(), m_type == INTERNAL ? "internal" : "external");
	return FFNGFiles::read(m_path, m_type);
}
//-----------------------------------------------------------------
bool
Path::write(const std::string &data) const
{
	__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "Path::write 1 %s (%s)", m_path.c_str(), m_type == INTERNAL ? "internal" : "external");
	return FFNGFiles::write(m_path, data);
}
//-----------------------------------------------------------------
bool
Path::testWrite() const
{
	__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "Path::testWrite 1 %s (%s)", m_path.c_str(), m_type == INTERNAL ? "internal" : "external");
	return true;
}
