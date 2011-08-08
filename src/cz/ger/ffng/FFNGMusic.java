package cz.ger.ffng;

import java.io.IOException;
import java.util.Vector;

import android.content.res.AssetFileDescriptor;
import android.media.MediaPlayer;
import android.util.Log;

import com.badlogic.gdx.backends.android.AndroidFileHandle;

public class FFNGMusic {
	private static Vector<FFNGMusic> channels = new Vector<FFNGMusic>();
	private static float defaultVolume = 1f;
	private MediaPlayer mp = new MediaPlayer();
	private boolean isPrepared = false;
	
	public FFNGMusic(String file) {
		try {
			AndroidFileHandle fh = (AndroidFileHandle)FFNGFiles.getFileHandle(file, FFNGFiles.INTERNAL);
			AssetFileDescriptor descriptor = FFNG.assets.openFd(fh.path());
			mp.setDataSource(descriptor.getFileDescriptor(), descriptor.getStartOffset(), descriptor.getLength());
			descriptor.close();
			mp.prepare();
			mp.setLooping(true);
			mp.setVolume(defaultVolume, defaultVolume);
			
			isPrepared = true;
			
			channels.add(this);
			
			//Log.d("FFNG", "MUSIC load " + file);
		} catch(Exception ex) {
		}
	}
	
	public static FFNGMusic loadMusic(String file) {
		return new FFNGMusic(file);
	}
	
	public void play() {
		//Log.d("FFNG", "MUSIC play");
		if (mp == null || mp.isPlaying()) return;

		try {
			if (!isPrepared) mp.prepare();
			mp.start();
		} catch (IllegalStateException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void stop() {
		//Log.d("FFNG", "MUSIC stop");
		if (mp.isPlaying()) {
			mp.stop();
			isPrepared = false;
		}
	}
	
	public void pause() {
		//Log.d("FFNG", "MUSIC pause");
		if (mp.isPlaying()) {
			mp.pause();
		}
	}
	
	public void dispose() {
		//Log.d("FFNG", "MUSIC dispose");
		if (mp != null) {
			if (mp.isPlaying()) mp.stop();
			mp.release();
			mp = null;
		}
		channels.remove(this);
	}
	
	public void setVolume(int vol) {
		//Log.d("FFNG", "MUSIC setVolume");
		mp.setVolume((float)vol / 100f, (float)vol / 100f);
	}
	
	public static void stopAll() {
		//Log.d("FFNG", "MUSIC stopAll");
		for (FFNGMusic m : channels) {
			m.stop();
		}
	}
	
	public static void pauseAll() {
		//Log.d("FFNG", "MUSIC pauseAll");
		for (FFNGMusic m : channels) {
			m.pause();
		}
	}
	
	public static void playAll() {
		//Log.d("FFNG", "MUSIC playAll");
		for (FFNGMusic m : channels) {
			m.play();
		}
	}
	
	public static void setVolumeAll(int vol) {
		//Log.d("FFNG", "MUSIC setVolumeAll");
		defaultVolume = vol / 100f;
		for (FFNGMusic m : channels) {
			m.setVolume(vol);
		}
	}
}
