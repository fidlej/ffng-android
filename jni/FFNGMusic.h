#ifndef FFNGMUSIC_H_
#define FFNGMUSIC_H_

#include "jni.h"

#define MIX_MAX_VOLUME 128

class Mix_Music {
	jobject music;

	jobject loadMusic(const char *file);
public:
	Mix_Music(const char *file);
	~Mix_Music();
	void play();
	void stop();
	void dispose();

	static void stopAll();
	static void setVolumeAll(int vol);
};

class Mix_Chunk {
	jobject sound;
	int channel;

	jobject loadSound(const char *file);
public:
	Mix_Chunk(const char *file);
	~Mix_Chunk();
	jobject getSoundObject() const { return sound; }
	void setChannel(int ch) { channel = ch; }
};

class FFNGMusic {
public:
	static void openAudio(int frequency);
	static void closeAudio();

	static Mix_Chunk* loadWAV(const char *file);
	static void halt(int channel);
	static bool isPlaying(int channel);
	static int playChannel(int channel, Mix_Chunk *sound, int loops);
	static void volume(int channel, float vol);
	static void free(Mix_Chunk *chunk);

	static Mix_Music* loadMUS(const char *file);
	static int playMusic(Mix_Music* music, int loops);
	static void volumeMusic(int vol);
	static bool playingMusic();
	static void haltMusic();
	static void freeMusic(Mix_Music *music);
	static void hookMusicFinished(void (*music_finished)(void));
};

#endif /* FFNGMUSIC_H_ */
