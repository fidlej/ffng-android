#ifndef HEADER_WILABEL_H
#define HEADER_WILABEL_H

class Font;

#include "WiPicture.h"

//FFNG #include "SDL.h"
#include "Color.h"
#include <string>

/**
 * Constant text label.
 */
class WiLabel : public WiPicture {
    public:
        WiLabel(const std::string &text, const Font &font,
                const Color &color);
};

#endif
