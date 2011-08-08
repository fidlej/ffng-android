#include <assert.h>
#include "FFNGSurface.h"
#include <android/log.h>

SDL_Surface::SDL_Surface()
: w(0)
, h(0)
, colorkey(0)
, surface(NULL)
{
	// TODO shouldn't exists in the end
	__android_log_print(ANDROID_LOG_ERROR, "FFNG", "SDL_Surface::SDL_Surface");
}

SDL_Surface::SDL_Surface(int width, int height, Uint32 colorkey_, bool transparent)
: w(width)
, h(height)
, colorkey(colorkey_)
, surface(NULL)
{
	// TODO create Bitmap and fill it with color key if transparent
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSurface");
		mid = javaEnv->GetStaticMethodID(cls, "newSurface", "(II)Lcz/ger/ffng/FFNGSurface;");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::SDL_Surface 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    surface = javaEnv->CallStaticObjectMethod(cls, mid, width, height);
}

SDL_Surface::SDL_Surface(SDL_Surface *source)
: surface(NULL)
, w(source->w)
, h(source->h)
{
	// TODO w, h, colorkey
	SDL_Surface(source->w, source->h, source->colorkey);
	SDL_Rect srcRect(0, 0, source->w, source->h);
	SDL_Rect dstRect(0, 0, 0, 0);	// width and height are irrelevant
	FFNGSurface::blitSurface(source, &srcRect, this, &dstRect);
}

SDL_Surface::SDL_Surface(const char *path)
: surface(NULL)
{
	// TODO w, h, colorkey
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSurface");
		mid = javaEnv->GetStaticMethodID(cls, "newSurface", "(Ljava/lang/String;)Lcz/ger/ffng/FFNGSurface;");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::SDL_Surface 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

	jstring pathString = javaEnv->NewStringUTF(path);

	surface = javaEnv->CallStaticObjectMethod(cls, mid, pathString);

	javaEnv->DeleteLocalRef(pathString);

	w = getWidth();
	h = getHeight();
}

SDL_Surface::SDL_Surface(const std::string &path)
: surface(NULL)
, w(0)
, h(0)
{
	// TODO w, h, colorkey
	SDL_Surface(path.c_str());
}

SDL_Surface::SDL_Surface(jobject font, const char *text, int frontColor, int bgColor, int outlineWidth)
: surface(NULL)
{
	// TODO w, h, colorkey
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSurface");
		mid = javaEnv->GetStaticMethodID(cls, "newSurface", "(Lcz/ger/ffng/FFNGFont;Ljava/lang/String;III)Lcz/ger/ffng/FFNGSurface;");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::SDL_Surface 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

	jstring textString = javaEnv->NewStringUTF(text);

	surface = javaEnv->CallStaticObjectMethod(cls, mid, font, textString, frontColor, bgColor, outlineWidth);

	javaEnv->DeleteLocalRef(textString);

	w = getWidth();
	h = getHeight();
}

SDL_Surface::~SDL_Surface() {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSurface");
		mid = javaEnv->GetMethodID(cls, "dispose", "()V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::~SDL_Surface 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallVoidMethod(surface, mid);
    javaEnv->DeleteLocalRef(surface);
}

jobject SDL_Surface::getSurface() const {
	return surface;
}

void SDL_Surface::blit(int dstx, int dsty, SDL_Surface *source, int srcx, int srcy, int srcw, int srch) {
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::blit 0 %p %d %d %p %d %d %d %d", surface, dstx, dsty, source, srcx, srcy, srcw, srch);
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSurface");
		mid = javaEnv->GetMethodID(cls, "blit", "(IILcz/ger/ffng/FFNGSurface;IIII)V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::blit 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallVoidMethod(surface, mid, dstx, dsty, source->getSurface(), srcx, srcy, srcw, srch);
}

void SDL_Surface::blitMasked(int dstx, int dsty, const SDL_Surface *mask, Uint32 color, const SDL_Surface *layer) {
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::blitMasked 0 %d %d %p %d %p", dstx, dsty, mask, color, layer);
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSurface");
		mid = javaEnv->GetMethodID(cls, "blitMasked", "(IILcz/ger/ffng/FFNGSurface;ILcz/ger/ffng/FFNGSurface;)V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::blitMasked 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallVoidMethod(surface, mid, dstx, dsty, mask->getSurface(), color, layer->getSurface());
}

void SDL_Surface::blitWavy(const SDL_Surface *source, int x, int y, float amp, float periode, float speed)
{
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSurface");
		mid = javaEnv->GetMethodID(cls, "blitWavy", "(Lcz/ger/ffng/FFNGSurface;IIFFF)V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::blitWavy 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallVoidMethod(surface, mid, source->getSurface(), x, y, amp, periode, speed);
}

void SDL_Surface::blitDisintegrate(const SDL_Surface *source, int x, int y, int disint) {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSurface");
		mid = javaEnv->GetMethodID(cls, "blitDisintegrate", "(Lcz/ger/ffng/FFNGSurface;III)V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::blitDisintegrate 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallVoidMethod(surface, mid, source->getSurface(), x, y, disint);
}

void SDL_Surface::blitMirror(const SDL_Surface *source, int x, int y, int border) {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSurface");
		mid = javaEnv->GetMethodID(cls, "blitMirror", "(Lcz/ger/ffng/FFNGSurface;III)V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::blitMirror 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallVoidMethod(surface, mid, source->getSurface(), x, y, border);
}

void SDL_Surface::blitReverse(const SDL_Surface *source, int x, int y) {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSurface");
		mid = javaEnv->GetMethodID(cls, "blitReverse", "(Lcz/ger/ffng/FFNGSurface;II)V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::blitReverse 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallVoidMethod(surface, mid, source->getSurface(), x, y);
}

void SDL_Surface::blitZX(const SDL_Surface *source, int x, int y, int zx, int countHeight, int stripeHeight)
{
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSurface");
		mid = javaEnv->GetMethodID(cls, "blitZX", "(Lcz/ger/ffng/FFNGSurface;IIIII)V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::blitZX 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallVoidMethod(surface, mid, source->getSurface(), x, y, zx, countHeight, stripeHeight);
}

void SDL_Surface::line(int x1, int y1, int x2, int y2, Uint32 colorRGBA) {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSurface");
		mid = javaEnv->GetMethodID(cls, "line", "(IIIII)V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::line 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallVoidMethod(surface, mid, x1, y1, x2, y2, colorRGBA);
}

void SDL_Surface::fillRect(const SDL_Rect *dstRect, Uint32 pixel) {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSurface");
		mid = javaEnv->GetMethodID(cls, "fillRect", "(IIIII)V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::fillRect 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

	int x_ = 0,
		y_ = 0,
		w_ = w,
		h_ = h;
	if (dstRect) {
		x_ = dstRect->x;
		y_ = dstRect->y;
		w_ = dstRect->w;
		h_ = dstRect->h;
	}

    javaEnv->CallVoidMethod(surface, mid, x_, y_, w_, h_, pixel);
}

void SDL_Surface::filledCircleColor(int x, int y, int radius, Uint32 colorRGBA) {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSurface");
		mid = javaEnv->GetMethodID(cls, "filledCircle", "(IIII)V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::filledCircleColor 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallVoidMethod(surface, mid, x, y, radius, colorRGBA);
}

Uint32 SDL_Surface::getPixel(int x, int y) {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSurface");
		mid = javaEnv->GetMethodID(cls, "getPixel", "(II)I");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::getPixel 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return 0;
	}

    return javaEnv->CallIntMethod(surface, mid, x, y);
}

void SDL_Surface::putPixel(int x, int y, Uint32 colorRGBA) {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSurface");
		mid = javaEnv->GetMethodID(cls, "putPixel", "(III)V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::putPixel 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallVoidMethod(surface, mid, x, y, colorRGBA);
}

int SDL_Surface::getWidth() const {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSurface");
		mid = javaEnv->GetMethodID(cls, "getWidth", "()I");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::getWidth 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return 0;
	}

    return javaEnv->CallIntMethod(surface, mid);
}

int SDL_Surface::getHeight() const {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSurface");
		mid = javaEnv->GetMethodID(cls, "getHeight", "()I");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "SDL_Surface::getHeight 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return 0;
	}

    return javaEnv->CallIntMethod(surface, mid);
}

SDL_Rect::SDL_Rect() {
	SDL_Rect(0, 0, 0, 0);
}

SDL_Rect::SDL_Rect(int x_, int y_, int w_, int h_)
: x(x_)
, y(y_)
, w(w_)
, h(h_)
{
}

SDL_Surface* FFNGSurface::imgLoad(const char *file) {
	return new SDL_Surface(file);
}

void FFNGSurface::freeSurface(SDL_Surface *surface) {
	if (surface) {
		delete surface;
		surface = NULL;
	}
}

void FFNGSurface::blitSurface(SDL_Surface *srcSurface, const SDL_Rect *srcRect, SDL_Surface *dstSurface, const SDL_Rect *dstRect) {
	int dstx, dsty, srcx, srcy, srcw, srch;

	if (dstRect) {
		dstx = dstRect->x;
		dsty = dstRect->y;
	} else {
		dstx = 0;
		dsty = 0;
	}

	if (srcRect) {
		srcx = srcRect->x;
		srcy = srcRect->y;
		srcw = srcRect->w;
		srch = srcRect->h;
	} else {
		srcx = 0;
		srcy = 0;
		srcw = srcSurface->w;
		srch = srcSurface->h;
	}

	dstSurface->blit(dstx, dsty, srcSurface, srcx, srcy, srcw, srch);
}

Uint32 FFNGSurface::getPixel(SDL_Surface *surface, int x, int y) {
	return surface->getPixel(x, y);
}

void FFNGSurface::putPixel(SDL_Surface *surface, int x, int y, Uint32 colorRGBA) {
	surface->putPixel(x, y, colorRGBA);
}

SDL_Surface* FFNGSurface::createSurface(int width, int height, int colorkey) {
	SDL_Surface *surface = new SDL_Surface(width, height, colorkey);
	return surface;
}

SDL_Surface* FFNGSurface::clone(SDL_Surface *source) {
	return new SDL_Surface(source);
}

void FFNGSurface::fillRect(SDL_Surface *surface, const SDL_Rect *dstRect, Uint32 pixel) {
	surface->fillRect(dstRect, pixel);
}

void FFNGSurface::filledCircleColor(SDL_Surface *screen, int x, int y, int radius, Uint32 colorRGBA) {
	screen->filledCircleColor(x, y, radius, colorRGBA);
}

void FFNGSurface::lineColor(SDL_Surface *screen, int x1, int y1, int x2, int y2, Uint32 colorRGBA) {
	screen->line(x1, y1, x2, y2, colorRGBA);
}
