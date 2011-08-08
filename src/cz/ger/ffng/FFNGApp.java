package cz.ger.ffng;

import java.util.LinkedList;

import cz.ger.ffng.Controls.TouchArea;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Bitmap.Config;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnKeyListener;
import android.view.View.OnTouchListener;

public class FFNGApp extends Thread {
	private static final int GAMESTATE_LOADING = 1;
	private static final int GAMESTATE_WORLDMAP = 2;
	private static final int GAMESTATE_LEVEL = 3;
	private static final int GAMESTATE_OPTIONS = 4;
	private static final int GAMESTATE_MOVIE = 5;
	private static final int GAMESTATE_DEMO = 6;
	private static final int GAMESTATE_POSTER = 7;
	private int gameState = GAMESTATE_LOADING;
	
	private boolean pauseRequired = false;
	private boolean disposeRequired = false;
	
	private View view = null;
	private Activity activity = null;
	
	private Bitmap screen = null;
	private Canvas canvas;
	
	private int surfaceWidth = 0, surfaceHeight = 0;
	
	private int windowWidth = -1, windowHeight = -1;
	private float windowX = 0, windowY = 0;
	private float windowScale = 1f; 
	private String windowCaption = "";
	private static final int windowCaptionDelayFrames = 50; // frames
	private int windowCaptionFrameCountdown = 0;
	private float captionX = 10, captionY = 40;
	private Paint captionPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
	private Rect captionBounds = new Rect();
	
	private LinkedList<FFNGInputEvent> eventQueue = new LinkedList<FFNGInputEvent>();

	private Controls controls = new Controls();
	private ActiveFishControl activeFish = new ActiveFishControl();
	
	FFNGApp() {
		screen = Bitmap.createBitmap(900, 900, Config.ARGB_8888);
		canvas = new Canvas(screen);
		
		captionPaint.setARGB(255, 255, 255, 255);
		captionPaint.setTextSize(30);
		
		// bottom left
		controls.addTouchArea(controls.new TouchArea(new Rect(48, 480-96, 48+96, 480), FFNGView.SDLK_DOWN, "gui/down.png"));
		controls.addTouchArea(controls.new TouchArea(new Rect(0, 480-2*96, 96, 480-96), FFNGView.SDLK_LEFT, "gui/left.png"));
		controls.addTouchArea(controls.new TouchArea(new Rect(96, 480-2*96, 2*96, 480-96), FFNGView.SDLK_RIGHT, "gui/right.png"));
		controls.addTouchArea(controls.new TouchArea(new Rect(48, 480-3*96, 48+96, 480-2*96), FFNGView.SDLK_UP, "gui/up.png"));
		
		// bottom right
		controls.addTouchArea(controls.new TouchArea(new Rect(800-96, 480-96, 800, 480), FFNGView.SDLK_SPACE, "gui/switch.png"));
		
		// top right
		controls.addTouchArea(controls.new TouchArea(new Rect(800-96, 0, 800, 96), FFNGView.MORE_BUTTONS, "gui/more.png"));
		controls.addTouchArea(moreGroup[0] = controls.new TouchArea(new Rect(800-2*96, -96, 800-96, 0), FFNGView.SDLK_BACKSPACE, "gui/restart.png"));
		controls.addTouchArea(moreGroup[1] = controls.new TouchArea(new Rect(800-3*96, -96, 800-2*96, 0), FFNGView.SDLK_ESCAPE, "gui/exit.png"));
		controls.addTouchArea(moreGroup[2] = controls.new TouchArea(new Rect(800-4*96, -96, 800-3*96, 0), FFNGView.SDLK_F10, "gui/options.png"));
		controls.addTouchArea(moreGroup[3] = controls.new TouchArea(new Rect(800-5*96, -96, 800-4*96, 0), FFNGView.SDLK_F3, "gui/load.png"));
		controls.addTouchArea(moreGroup[4] = controls.new TouchArea(new Rect(800-6*96, -96, 800-5*96, 0), FFNGView.SDLK_F2, "gui/save.png"));

		// top left
		controls.addTouchArea(controls.new TouchArea(new Rect(0, 0, 96, 96), FFNGView.SDLK_MINUS, "gui/undo.png"));
		controls.addTouchArea(controls.new TouchArea(new Rect(96, 0, 2*96, 96), FFNGView.SDLK_PLUS, "gui/redo.png"));
		
		controls.resizeToScreen(surfaceWidth, surfaceHeight);
	}
	
	public void setContext(View view, Activity activity) {
		this.view = view;
		this.activity = activity;
	}
	
	public void run()
	{
		ffngmain();
		activity.finish();
	}
	
	public void pauseRequest() {
		Log.d("FFNG", "pause");
		pauseRequired = true;
		//super.suspend();
		FFNGMusic.pauseAll();
		FFNGSound.pauseAll();
	}
	
	synchronized public void resumeRequest() {
		Log.d("FFNG", "resume");
		pauseRequired = false;
		//super.resume();
		FFNGMusic.playAll();
		FFNGSound.playAll();
		notifyAll();
	}
	
	synchronized public void disposeRequest() {
		Log.d("FFNG", "dispose");
		disposeRequired = true;
		FFNGMusic.stopAll();
		notifyAll();
	}
	
	public boolean isPauseRequired() {
		return pauseRequired;
	}
	
	public boolean isDisposeRequired() {
		return disposeRequired;
	}
	
	synchronized private boolean pauseAndDisposeChance() {
		if (isPauseRequired()) {
			Log.d("FFNG", "pause thread");
			try {
				this.wait();
			} catch (InterruptedException e) {
			}
			Log.d("FFNG", "resume thread");
		}
		return isDisposeRequired();
	}

	private Paint bilinearPaint = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.FILTER_BITMAP_FLAG);
	synchronized public void render(Canvas canvas) {
		if (canvas.getWidth() != surfaceWidth || canvas.getHeight() != surfaceHeight) {
			surfaceWidth = canvas.getWidth();
			surfaceHeight = canvas.getHeight();
			
			setWindowScale();
		}
		/*
		while (surface == null) {
			try {
				wait();
			} catch(InterruptedException e) {}
		}
		*/
		canvas.save();
		canvas.scale(windowScale, windowScale, 0, 0);
		canvas.drawBitmap(screen, windowX/windowScale, windowY/windowScale, bilinearPaint);
		canvas.restore();

		// HUD - caption, subtitles, buttons
		if (gameState == GAMESTATE_LEVEL) {
			moreButtonsAnim();
			saveEffectRender(canvas);
			
			controls.render(canvas);
			activeFish.render(canvas, 800-96, 480-96, surfaceWidth, surfaceHeight);
		}
		
		// window caption
		renderCaption(canvas);
		//notifyAll();
	}
	
	protected void renderCaption(Canvas canvas) {
		if (windowCaptionFrameCountdown > 0) {
			if (windowCaptionFrameCountdown < 0.2*windowCaptionDelayFrames) {
				captionPaint.setAlpha((int)(windowCaptionFrameCountdown / (0.2*windowCaptionDelayFrames) * 255f));
			}
			
			captionPaint.setARGB(captionPaint.getAlpha(), 0, 0, 0);
			canvas.drawText(windowCaption, captionX-2, captionY-2, captionPaint);
			canvas.drawText(windowCaption, captionX+2, captionY-2, captionPaint);
			canvas.drawText(windowCaption, captionX+2, captionY+2, captionPaint);
			canvas.drawText(windowCaption, captionX-2, captionY+2, captionPaint);
			captionPaint.setARGB(captionPaint.getAlpha(), 255, 255, 255);
			canvas.drawText(windowCaption, captionX, captionY, captionPaint);
			windowCaptionFrameCountdown--;
		}
	}
	
	private Bitmap saveScreenCopy = null;
	private int saveEffectTimer = -1;
	private static final int saveEffectDuration = 10;
	private ColorMatrix saveEffectColorMatrix = new ColorMatrix();
	private float saveEffectRotation = 0f;
	synchronized public void saveEffect() {
		if (saveScreenCopy != null) {
			saveScreenCopy.recycle();
			saveScreenCopy = null;
		}
		
		saveScreenCopy = Bitmap.createBitmap((int)(windowWidth*windowScale/2), (int)(windowHeight*windowScale/2), Config.RGB_565);
		Canvas c = new Canvas(saveScreenCopy);
		
		saveEffectColorMatrix.setSaturation(0.1f);
        ColorMatrixColorFilter f = new ColorMatrixColorFilter(saveEffectColorMatrix);
        Paint p = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.FILTER_BITMAP_FLAG);
        p.setColorFilter(f);
        c.scale(windowScale*0.5f, windowScale*0.5f);
        c.drawBitmap(screen, 0, 0, p);
        
        saveEffectRotation = (float)(10f + Math.random() * 20f);   // 10 to 20 degrees
        if (Math.random() < 0.5f) saveEffectRotation = -saveEffectRotation;
        
		//saveScreenCopy = screen.copy(Config.RGB_565, false);
		saveEffectTimer = saveEffectDuration;
	}
	
	private Paint saveEffectPaint = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.LINEAR_TEXT_FLAG);
	public void saveEffectRender(Canvas c) {
		if (saveEffectTimer > 0 && saveScreenCopy != null) {
			saveEffectPaint.setAlpha(128 + saveEffectTimer * 128 / saveEffectDuration);
			c.save();
			float scale = saveEffectTimer * 2f / saveEffectDuration;
			c.translate(surfaceWidth/2f - saveScreenCopy.getWidth() * scale / 2f, saveEffectTimer * windowY / saveEffectDuration);
			c.rotate(saveEffectRotation, saveScreenCopy.getWidth() * scale / 2f, saveScreenCopy.getHeight() * scale / 2f);
			c.scale(scale, scale);
			c.drawBitmap(saveScreenCopy, 0, 0, saveEffectPaint);
			c.restore();
			
			saveEffectTimer--;
			
			if (saveEffectTimer == 0) {
				saveScreenCopy.recycle();
				saveScreenCopy = null;
			}
		}
	}

	private TouchArea[] moreGroup = new TouchArea[5];
	private int moreButtonsY = -96;
	private int moreButtonsAnim = 0;
	public void moreButtons() {
		if (moreButtonsAnim == 0) {
			moreButtonsAnim = (moreButtonsY >= 0) ? -12 : 12;
		} else {
			moreButtonsAnim = (moreButtonsAnim > 0) ? -12 : 12;
		}
	}
	
	public void moreButtonsClose() {
		moreButtonsAnim = (moreButtonsY > -96) ? -12 : 0;
	}
	
	public void moreButtonsAnim() {
		if (moreButtonsAnim != 0) {
			moreButtonsY += moreButtonsAnim;
			for (int i = 0; i < moreGroup.length; ++i) {
				moreGroup[i].bounds.top = moreButtonsY;
				moreGroup[i].bounds.bottom = moreButtonsY + 96;
				moreGroup[i].resizeToScreen(surfaceWidth, surfaceHeight);
			}
			
			if (moreButtonsY >= 0 || moreButtonsY <= -96)
				moreButtonsAnim = 0;
		}
	}
	
	synchronized public void renderThis(FFNGSurface surface) {
		/*
		while(surface != null) {
			try {
				wait();
			} catch(InterruptedException e) {}
		}*/
		canvas.drawBitmap(surface.getBitmap(), 0, 0, null);
       	activity.runOnUiThread(new Runnable() {
       			@Override public void run() {
       				view.invalidate();
       			}
       		});
		//notifyAll();
	}
	
	private float touchx = -1;
	private float touchy = -1;
	
	public void setTouch(float x, float y) {
		touchx = x;
		touchy = y;
	}

	public int getTouchX() {
		return (int)touchx;
	}

	public int getTouchY() {
		return (int)touchy;
	}
	
	public FFNGInputEvent pollEvent() {
		if (eventQueue.isEmpty())
			return null;
		
		return eventQueue.poll();
	}
	
	private long startTime = System.currentTimeMillis();
	public int getTicks() {
		return (int)(System.currentTimeMillis() - startTime);
	}
	
	public void setWindowSize(int width, int height) {
		windowWidth = width;
		windowHeight = height;
		windowCaptionFrameCountdown = windowCaptionDelayFrames;

		setWindowScale();
		controls.resizeToScreen(surfaceWidth, surfaceHeight);
		
		canvas.drawARGB(255, 0, 0, 0);
	}

	protected void setWindowScale() {
		float scaleX = (float)surfaceWidth / windowWidth,
			  scaleY = (float)surfaceHeight / windowHeight;
	
		windowScale = (scaleX < scaleY ? scaleX : scaleY);
		windowX = (surfaceWidth-windowWidth*windowScale)/2f;
		windowY = (surfaceHeight-windowHeight*windowScale)/2f;
	}
	
	public void setWindowCaption(String caption) {
		windowCaption = caption;
		windowCaptionFrameCountdown = windowCaptionDelayFrames;
		captionPaint.setAlpha(255);
		
		captionPaint.getTextBounds(windowCaption, 0, windowCaption.length(), captionBounds); 
		captionX = (surfaceWidth - captionBounds.width()) / 2f + captionBounds.left;
		captionY = (surfaceHeight - captionBounds.height()) / 2f + captionBounds.height() + captionBounds.top;
	}
	
	private void setGameState(int newState) {
		moreButtonsClose();
		gameState = newState;
	}
	
	private int getGameState() {
		return gameState;
	}
	
	private void switchFishes(boolean isBig) {
		activeFish.switchFishes(isBig);
	}
	
    static {
    	System.loadLibrary("ffng");
    }
       
    //@SuppressWarnings("unused")
    private native int ffngmain();
    
    public static class FFNGView extends View implements OnTouchListener, OnKeyListener {
    	private FFNGApp app = null;
    	
        public FFNGView(Context context, FFNGApp app) {
            super(context);
            setFocusable(true);
            
            this.app = app;
            
            setOnTouchListener(this);
            setOnKeyListener(this);
        }

        //private static int counter = 0;
        
        @Override protected void onDraw(Canvas canvas) {
        	app.render(canvas);
        	//invalidate();

        	/*
        	if (++counter == 1000) {
        		Log.d("FFNG", "1000");
        		counter = 0;
        	}
        	*/
        }

        private static final int TOUCH_STATE_NONE = 0;
        private static final int TOUCH_STATE_BUTTON_PRESSED = 1;
        private static final int TOUCH_STATE_JAVA_BUTTON_PRESSED = 2;
        private int touchState = TOUCH_STATE_NONE;
        private int lastKey = 0;
        private Controls.TouchArea touchedArea = null; 
		@Override
		public boolean onTouch(View view, MotionEvent event) {
			//Log.d("FFNG", "event: " + event.getAction());
			
			float screenX = event.getX();
			float screenY = event.getY();
			
			boolean result = false;

			switch(touchState) {

			case TOUCH_STATE_BUTTON_PRESSED:
				switch(event.getAction()) {
				case MotionEvent.ACTION_MOVE:
					//sleep(5);
					result = true;
					break;
				case MotionEvent.ACTION_UP:
				//case MotionEvent.ACTION_OUTSIDE:
				//case MotionEvent.ACTION_CANCEL:
					app.eventQueue.add(new FFNGInputEvent(FFNGInputEvent.KEYUP, lastKey));
					for (int i = 0; i < app.moreGroup.length; ++i) {
						if (app.moreGroup[i] == touchedArea)
							app.moreButtonsClose();
					}
					touchState = TOUCH_STATE_NONE;
					touchedArea.touched = false;
					touchedArea = null;
					result = true;
					break;
				}
				break;
				
			case TOUCH_STATE_JAVA_BUTTON_PRESSED:
				switch(lastKey) {
				case MORE_BUTTONS:
					if (event.getAction() == MotionEvent.ACTION_UP) {
						app.moreButtons();
						touchState = TOUCH_STATE_NONE;
						touchedArea.touched = false;
						touchedArea = null;
					}
					result = true;
					break;
				}
				break;
				
			case TOUCH_STATE_NONE:
				int key = 0;
				if (app.gameState == GAMESTATE_LEVEL) {
					touchedArea = app.controls.getTouchArea(screenX, screenY);
					key = touchedArea == null ? 0 : touchedArea.value;
					//key = app.controls.getTouchValue(screenX, screenY);
				}
				
				if (key >= 100) {   // FFNG java buttons
					switch (event.getAction()) {
					case MotionEvent.ACTION_DOWN:
						touchState = TOUCH_STATE_JAVA_BUTTON_PRESSED;
						touchedArea.touched = true;
						lastKey = key;
						result = true;
						break;
					}
				} else if (key > 0) {
			        switch (event.getAction()) {
		            	case MotionEvent.ACTION_DOWN:
		            		app.eventQueue.add(new FFNGInputEvent(FFNGInputEvent.KEYDOWN, key));
		            		touchState = TOUCH_STATE_BUTTON_PRESSED;
		            		touchedArea.touched = true;
		            		lastKey = key;
		            		result = true;
		            		break;
			        }
				} else {
			        float x = (screenX - app.windowX) / app.windowScale;
			        float y = (screenY - app.windowY) / app.windowScale;
			        
			        switch (event.getAction()) {
			            case MotionEvent.ACTION_DOWN:
			            	app.setTouch(x, y);
			                //touch_start(x, y);
			                //invalidate();
			                result = true;
			                break;
			            case MotionEvent.ACTION_MOVE:
			            	app.setTouch(x, y);
			            	//sleep(5);
			                //touch_move(x, y);
			                //invalidate();
			                result = true;
			                break;
			            case MotionEvent.ACTION_UP:
						//case MotionEvent.ACTION_OUTSIDE:
						//case MotionEvent.ACTION_CANCEL:
			            	app.setTouch(-1, -1);
			            	app.eventQueue.add(new FFNGInputEvent(FFNGInputEvent.MOUSEBUTTONDOWN, x, y));
			                //touch_up();
			                //invalidate();
			                result = true;
			                break;
			        }
				}
				break;
			}

			return result;
		}
		
		private void sleep(int ms) {
			try {
				Thread.sleep(ms);
			}
			catch(InterruptedException ie){
				//If this thread was intrrupted by nother thread 
			}
		}

		// see SDLKey in FFNGInputEvent.h
		public static final int SDLK_UP = 1;
		public static final int SDLK_DOWN = 2;
		public static final int SDLK_LEFT = 3;
		public static final int SDLK_RIGHT = 4;
		public static final int SDLK_SPACE = 5;
		public static final int SDLK_F2 = 6;
		public static final int SDLK_F3 = 7;
		public static final int SDLK_F10 = 8;
		public static final int SDLK_BACKSPACE = 9;
		public static final int SDLK_MINUS = 10;
		public static final int SDLK_PLUS = 11;
		public static final int SDLK_ESCAPE = 12;
		// FFNG java buttons
		public static final int MORE_BUTTONS = 100;
		@Override
		public boolean onKey(View v, int keyCode, KeyEvent event) {
			int key = -1;
			switch (keyCode) {
				case KeyEvent.KEYCODE_DPAD_UP: key = SDLK_UP; break;
				case KeyEvent.KEYCODE_DPAD_DOWN: key = SDLK_DOWN; break;
				case KeyEvent.KEYCODE_DPAD_LEFT: key = SDLK_LEFT; break;
				case KeyEvent.KEYCODE_DPAD_RIGHT: key = SDLK_RIGHT; break;
				case KeyEvent.KEYCODE_DPAD_CENTER: key = SDLK_SPACE; break;
			}
			
			if (key > 0) {
				switch (event.getAction()) {
					case KeyEvent.ACTION_DOWN:
						app.eventQueue.add(new FFNGInputEvent(FFNGInputEvent.KEYDOWN, key));
						return true;
					case KeyEvent.ACTION_UP:
						app.eventQueue.add(new FFNGInputEvent(FFNGInputEvent.KEYUP, key));
						return true;
				}
			}
			return false;
		}
    }
}
