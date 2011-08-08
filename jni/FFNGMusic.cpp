#include <stdio.h>
#include <assert.h>
#include "FFNGMusic.h"

void FFNGMusic::openAudio(int frequency) {
	// could stay empty
}

void FFNGMusic::closeAudio() {
	// could stay empty
}

Mix_Chunk* FFNGMusic::loadWAV(const char *file) {
	return new Mix_Chunk(file);
}

void FFNGMusic::halt(int channel) {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSound");
		mid = javaEnv->GetStaticMethodID(cls, "halt", "(I)V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "FFNGMusic::halt 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

	javaEnv->CallStaticVoidMethod(cls, mid, channel);

}

bool FFNGMusic::isPlaying(int channel) {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSound");
		mid = javaEnv->GetStaticMethodID(cls, "isPlaying", "(I)Z");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "FFNGMusic::isPlaying 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return false;
	}

	return javaEnv->CallStaticBooleanMethod(cls, mid, channel);
}

int FFNGMusic::playChannel(int channel, Mix_Chunk *sound, int loops) {
	// TODO don't forget the loops
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSound");
		mid = javaEnv->GetMethodID(cls, "playChannel", "(II)I");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "FFNGMusic::playChannel 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return -1;
	}

	int ch = javaEnv->CallIntMethod(sound->getSoundObject(), mid, channel, loops);
    sound->setChannel(ch);
    return ch;
}

void FFNGMusic::volume(int channel, float vol) {
	// TODO set channel volume, -1 for all
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSound");
		mid = javaEnv->GetStaticMethodID(cls, "volume", "(IF)V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "FFNGMusic::volume 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

	javaEnv->CallStaticVoidMethod(cls, mid, channel, vol);
}

void FFNGMusic::free(Mix_Chunk *chunk) {
	if (chunk) {
		delete chunk;
		chunk = NULL;
	}
}

Mix_Music* FFNGMusic::loadMUS(const char *file) {
	return new Mix_Music(file);
}

int FFNGMusic::playMusic(Mix_Music* music, int loops) {
	// TODO loops, return value (channel?)
	music->play();
	return 0;
}

void FFNGMusic::volumeMusic(int vol) {
	Mix_Music::setVolumeAll(vol);
}

bool FFNGMusic::playingMusic() {
	return false;
}

void FFNGMusic::haltMusic() {
	Mix_Music::stopAll();
}

void FFNGMusic::freeMusic(Mix_Music *music) {
	music->dispose();
	if (music) {
		delete music;
		music = NULL;
	}
}

void FFNGMusic::hookMusicFinished(void (*music_finished)(void)) {
	// TODO store music_finished and call it when music is stop playing (SDLSoundAgent::own_update)
}


Mix_Music::Mix_Music(const char *file) {
	music = loadMusic(file);
}

Mix_Music::~Mix_Music() {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGMusic");
		mid = javaEnv->GetMethodID(cls, "dispose", "()V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "Mix_Music::~Mix_Music 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallVoidMethod(music, mid);

	javaEnv->DeleteLocalRef(music);
}

jobject Mix_Music::loadMusic(const char *file) {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGMusic");
		mid = javaEnv->GetStaticMethodID(cls, "loadMusic", "(Ljava/lang/String;)Lcz/ger/ffng/FFNGMusic;");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "Mix_Music::loadMusic 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return false;
	}

	jstring fileString = javaEnv->NewStringUTF(file);

    jobject result = javaEnv->CallStaticObjectMethod(cls, mid, fileString);

    javaEnv->DeleteLocalRef(fileString);

    return result;
}

void Mix_Music::play() {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGMusic");
		mid = javaEnv->GetMethodID(cls, "play", "()V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "Mix_Music::play 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallVoidMethod(music, mid);
}

void Mix_Music::stop() {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGMusic");
		mid = javaEnv->GetMethodID(cls, "stop", "()V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "Mix_Music::stop 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallVoidMethod(music, mid);
}

void Mix_Music::dispose() {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGMusic");
		mid = javaEnv->GetMethodID(cls, "dispose", "()V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "Mix_Music::dispose 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallVoidMethod(music, mid);
}

void Mix_Music::stopAll() {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGMusic");
		mid = javaEnv->GetStaticMethodID(cls, "stopAll", "()V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "Mix_Music::stopAll 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallStaticVoidMethod(cls, mid);
}

void Mix_Music::setVolumeAll(int vol) {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGMusic");
		mid = javaEnv->GetStaticMethodID(cls, "setVolumeAll", "(I)V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "Mix_Music::setvolumeAll 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallStaticVoidMethod(cls, mid, vol);
}

Mix_Chunk::Mix_Chunk(const char *file)
: channel(-1)
{
	sound = loadSound(file);
}

Mix_Chunk::~Mix_Chunk() {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSound");
		mid = javaEnv->GetMethodID(cls, "dispose", "()V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "Mix_Chunk::~Mix_Chunk 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return;
	}

    javaEnv->CallVoidMethod(sound, mid);

	javaEnv->DeleteLocalRef(sound);
}

jobject Mix_Chunk::loadSound(const char *file) {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGSound");
		mid = javaEnv->GetStaticMethodID(cls, "loadSound", "(Ljava/lang/String;)Lcz/ger/ffng/FFNGSound;");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "Mix_Chunk::loadSound 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return false;
	}

	jstring fileString = javaEnv->NewStringUTF(file);

    jobject result = javaEnv->CallStaticObjectMethod(cls, mid, fileString);

    javaEnv->DeleteLocalRef(fileString);

    return result;
}
