package cz.ger.ffng;

import java.io.IOException;
import java.io.OutputStream;

import com.badlogic.gdx.backends.android.AndroidAudio;
import com.badlogic.gdx.backends.android.AndroidFiles;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Canvas;
import android.os.Bundle;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

public class FFNG extends Activity {
	private FFNGApp app;
	public static AndroidFiles files = null;
	public static AndroidAudio audio = null;
	public static AssetManager assets = null;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        assets = getAssets();
        files = new AndroidFiles(assets);
        audio = new AndroidAudio(this);
        
        FFNGFiles.createCache();
        
        // turn off the window's title bar
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, 
                       WindowManager.LayoutParams.FLAG_FULLSCREEN);

        app = new FFNGApp();
        
        View view = new FFNGApp.FFNGView(this, app);
        setContentView(view);
        app.setContext(view, this);
        
        app.start();
    }
    
    public FFNGApp getApp() {
    	return app;
    }
    
    @Override
    protected void onPause() {
    	super.onPause();
    	app.pauseRequest();
    	releaseWakeLock();
    }
    
    @Override
    protected void onResume() {
        super.onResume();
        app.resumeRequest();
        acquireWakeLock();
    }
    
    @Override
    protected void onStop() {
        super.onStop();
    }
    
    protected void onDestroy() {
    	super.onDestroy();
        app.disposeRequest();
    }
    
    @Override
    public void onSaveInstanceState(Bundle outState) {
    	
    } 
    
    private WakeLock wakeLock;
	private void acquireWakeLock() {
		final PowerManager pm = (PowerManager) this.getSystemService(Context.POWER_SERVICE);
		wakeLock = pm.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK | PowerManager.ON_AFTER_RELEASE, "FFNG");
		wakeLock.acquire();
	}

	private void releaseWakeLock() {
		if(wakeLock != null) {
			wakeLock.release();
		}
	}
    
	/** A signal handler in native code has been triggered. As our last gasp,
	 * launch the crash handler (in its own process), because when we return
	 * from this function the process will soon exit. */
	/*
	void nativeCrashed()
	{
		if (prefs != null) {
			try {
				System.err.println("saved game was:\n"+prefs.getString("savedGame",""));
			} catch(Exception e) {}
		}
		new RuntimeException("crashed here (native trace should follow after the Java trace)").printStackTrace();
		startActivity(new Intent(this, CrashHandler.class));
	}
	*/
}