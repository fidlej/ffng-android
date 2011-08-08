#ifndef HEADER_WIPARA_H
#define HEADER_WIPARA_H

class Font;

#include "VBox.h"

//FFNG #include "SDL.h"
#include "Color.h"
#include <string>

/**
 * Multi line paragraph.
 */
class WiPara : public VBox {
    public:
        WiPara(const std::string &text, const Font &font,
                const Color &color);
};

#endif
