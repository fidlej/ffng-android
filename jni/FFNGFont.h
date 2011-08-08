#ifndef FFNGFONT_H_
#define FFNGFONT_H_

#include "Color.h"
#include "FFNGSurface.h"

class TTF_Font {
private:
	jobject typeface;
	int height;
public:
	TTF_Font(const char *file, int height_); //OpenFont
	~TTF_Font(); // CloseFont
	int fontHeight();
	static int init();
	static void quit();
	void sizeUTF8(const char *text, int *width, int *height);
	SDL_Surface* renderUTF8_Shaded(const char *text, Color fontColor, Color bgColor, int outlineWidth=0);
private:
	int getWidth(const char *text);
	int getHeight(const char *text);
};

#endif /* FFNGFONT_H_ */
