package cz.ger.ffng;

import java.io.IOException;
import java.util.HashMap;
import java.util.Vector;

import com.badlogic.gdx.backends.android.AndroidFileHandle;

import android.content.res.AssetFileDescriptor;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.util.Log;

public class FFNGSound {
	private static HashMap<Integer,FFNGSound> playingChannels = new HashMap<Integer,FFNGSound>();
	//private static Vector<FFNGSound> stoppedChannels = new Vector<FFNGSound>();
	private MediaPlayer mp = new MediaPlayer();
	private boolean isPrepared = false;
	private final String file;
	private int channel = -1;
	
	public FFNGSound(String file) {
		this.file = file;
		try {
			AndroidFileHandle fh = (AndroidFileHandle)FFNGFiles.getFileHandle(file, FFNGFiles.INTERNAL);
			AssetFileDescriptor descriptor = FFNG.assets.openFd(fh.path());
			mp.setDataSource(descriptor.getFileDescriptor(), descriptor.getStartOffset(), descriptor.getLength());
			descriptor.close();
			mp.prepare();
			
			isPrepared = true;

			//stoppedChannels.add(this);
			
			Log("load");
		} catch(Exception ex) {
		}
	}
	
	private void Log(String action) {
		/*
		Log.d("FFNG", file + "(" + action + "): " +
					  (mp == null ? "NULL, " : "") +
					  "channel " + channel + ", " +
				      (playingChannels.containsValue(this) ? "playing(arr)" : "not playing(arr)") + ", " +
				      ((mp != null && mp.isPlaying()) ? "playing(mp)" : "not playing(mp)") + ", " +
				      (isPrepared ? "prepared" : "not prepared") + ", " +
				      playingChannels.size() + " playing");
				      //stoppedChannels.size() + " stopped");
       */
	}
	
	private static void LogStatic(String action) {
		/*
		Log.d("FFNG", "static(" + action + "): " +
				      playingChannels.size() + " playing");
				      //stoppedChannels.size() + " stopped");
       */
	}
	
	public static FFNGSound loadSound(String file) {
		return new FFNGSound(file);
	}
	
	public static FFNGSound getSound(int channel) {
		if (playingChannels.containsKey(channel))
			return playingChannels.get(channel);
		else
			return null;
	}
	
	protected static int getFreeChannel() {
		int ch = 0;
		while (playingChannels.containsKey(ch)) {ch++;}
		return ch;
	}
	
	public int playChannel(int channel, int loops) {
		// je-li kanal = -1, najdi nejblizsi kanal, zacni hrat
		// jinak
		//		pokud kanal hraje, zastav ho
		// zacni hrat
		
		Log("play begin " + channel);
		
		if (channel == -1) {
			channel = getFreeChannel();
		} else {
			if (isPlaying(channel))
				halt(channel);
		}
		//stoppedChannels.remove(this);
		playingChannels.put(channel, this);
		
		Log("play " + channel);
		
		if (mp.isPlaying()) {
			FFNGSound snd = new FFNGSound(file);
			snd.playChannel(channel, 0);
			return channel;
		}

		try {
			if (!isPrepared) {
				mp.prepare();
				isPrepared = true;
			}
			mp.start();
			this.channel = channel; 
			
			mp.setOnCompletionListener(new OnCompletionListener() {

				@Override
				public void onCompletion(MediaPlayer mp) {
					stop();
				}
				
			});
		} catch (IllegalStateException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		Log("play end " + channel);
		
		return channel;
	}
	
	public boolean isPlaying() {
		//Log("isplaying");
		if (mp == null) return false;
		return mp.isPlaying();
	}
	
	public static boolean isPlaying(int channel) {
		//LogStatic("isPlaying " + channel);
		return playingChannels.containsKey(channel) && getSound(channel).isPlaying();
	}
	
	public void play() {
		Log("play");
		if (isPrepared) {
			mp.start();
		}
	}
	
	public void pause() {
		Log("pause");
		if (isPlaying()) {
			mp.pause();
		}
	}
	
	public void stop() {
		Log("stop begin");
		mp.stop();
		isPrepared = false;
		playingChannels.remove(channel);
		//stoppedChannels.add(this);
		channel = -1;
		Log("stop end");
	}
	
	public static void halt(int channel) {
		LogStatic("halt " + channel);
		FFNGSound sound = getSound(channel);
		if (sound != null) {
			sound.stop();
		}
	}
	
	public void setVolume(float vol) {
		Log("setVolume");
		mp.setVolume(vol / 100f, vol / 100f);
	}
	
	public static void volume(int channel, float vol) {
		LogStatic("setVolume " + channel);
		FFNGSound sound = getSound(channel);
		if (sound != null) {
			sound.setVolume(vol);
		}
	}
	
	public static void pauseAll() {
		LogStatic("pause all channels");
		for (FFNGSound sound : playingChannels.values()) {
			if (sound != null) {
				sound.pause();
			}
		}
	}
	
	public static void playAll() {
		LogStatic("pause all channels");
		for (FFNGSound sound : playingChannels.values()) {
			if (sound != null) {
				sound.play();
			}
		}
	}
	
	public void dispose() {
		Log("dispose begin");
		playingChannels.remove(channel);
		//stoppedChannels.remove(this);
		
		mp.release();
		mp = null;
		
		channel = -1;
		Log("dispose end");
	}

}
