#include "assert.h"
#include "FFNGVideo.h"
#include <android/log.h>

SDL_Surface* FFNGVideo::setVideoMode(int width, int height, int bpp, int flags) {
	// ignore bpp and flags
    __android_log_print(ANDROID_LOG_DEBUG, "FFNG", "FFNGVideo::setVideoMode %d %d", width, height);

	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jobject obj = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = JNI::getInstance()->getJavaCls();
		obj = JNI::getInstance()->getJavaObj();
		mid = javaEnv->GetMethodID(cls, "setWindowSize", "(II)V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "FFNGVideo::setVideoMode 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return NULL;
	}

    javaEnv->CallVoidMethod(obj, mid, width, height);

	return new SDL_Surface(width, height);
}

void FFNGVideo::setWindowCaption(const std::string &caption) {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jobject obj = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = JNI::getInstance()->getJavaCls();
		obj = JNI::getInstance()->getJavaObj();
		mid = javaEnv->GetMethodID(cls, "setWindowCaption", "(Ljava/lang/String;)V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "FFNGVideo::setWindowCaption 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

	jstring captionString = javaEnv->NewStringUTF(caption.c_str());
    javaEnv->CallVoidMethod(obj, mid, captionString);
	javaEnv->DeleteLocalRef(captionString);
}

void FFNGVideo::flip(SDL_Surface *screen) {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jobject obj = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = JNI::getInstance()->getJavaCls();
		obj = JNI::getInstance()->getJavaObj();
		mid = javaEnv->GetMethodID(cls, "renderThis", "(Lcz/ger/ffng/FFNGSurface;)V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "FFNGVideo::flip 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallVoidMethod(obj, mid, screen->getSurface());
}
