#ifndef HEADER_KEYSTROKE_H
#define HEADER_KEYSTROKE_H

//FFNG #include "SDL.h"
#include "FFNGTypes.h"
#include "FFNGInputEvent.h"
#include <string>

/**
 * KeyStroke.
 */
class KeyStroke {
    private:
        //FFNG static const int STROKE_IGNORE = ~(KMOD_CTRL|KMOD_ALT);
        SDLKey m_sym;
        //FFNG int m_mod;
        //FFNG Uint16 m_unicode;
    private:
        //FFNG static int modStrip(int mod);
    public:
        KeyStroke(const SDL_keysym &keysym);
        KeyStroke(SDLKey sym, int mod);

        SDLKey getKey() const { return m_sym; }
        //FFNG Uint16 getUnicode() const { return m_unicode; }
        bool less(const KeyStroke &other) const;
        bool equals(const KeyStroke &other) const;
        std::string toString() const;
};

/**
 * KeyStroke comparation.
 */
struct stroke_less
{
    bool operator()(const KeyStroke &left, const KeyStroke &right) const
    {
        return left.less(right);
    }
};

#endif
