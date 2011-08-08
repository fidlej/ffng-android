package cz.ger.ffng;

import java.util.Vector;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;

public class Controls {
	
	private Vector<TouchArea> touchAreas = new Vector<TouchArea>();
	
	private Paint debugPaint = new Paint();
	
	Controls() {
		debugPaint.setARGB(40, 255, 255, 255);
	}
	
	public void addTouchArea(TouchArea area) {
		touchAreas.add(area);
	}
	
	public TouchArea getTouchArea(float x, float y) {
		for(TouchArea ta : touchAreas) {
			if (ta.screenResizedBounds.contains((int)x, (int)y)) 
				return ta;
		}
		return null;
	}
	
	public int getTouchValue(float x, float y) {
		for(TouchArea ta : touchAreas) {
			if (ta.screenResizedBounds.contains((int)x, (int)y)) 
				return ta.value;
		}
		return 0;
	}
	
	private Paint guiPaint = new Paint(Paint.FILTER_BITMAP_FLAG | Paint.ANTI_ALIAS_FLAG);
	public void render(Canvas canvas) {
		for (TouchArea ta : touchAreas) {
			if (ta.image == null) {
				canvas.drawRect(ta.screenResizedBounds, debugPaint);
			} else {
				guiPaint.setAlpha(ta.touched ? 200 : 100);
				canvas.drawBitmap(ta.image, null, ta.screenResizedBounds, guiPaint);
			}
		}
	}
	
	public void resizeToScreen(int screenWidth, int screenHeight) {
		for (TouchArea ta : touchAreas) {
			ta.resizeToScreen(screenWidth, screenHeight);
		}
	}
	
	public class TouchArea {
		public Bitmap image = null;
		public Rect bounds;
		public Rect screenResizedBounds;
		public int value;
		public boolean touched = false;
		
		TouchArea(Rect bounds, int value, String image) {
			init(bounds, value, BitmapFactory.decodeStream(FFNG.files.internal(image).read(), null, null));
		}
		
		TouchArea(Rect bounds, int value) {
			init(bounds, value, null);
		}
		
		private void init(Rect bounds, int value, Bitmap image) {
			this.bounds = bounds;
			this.screenResizedBounds = new Rect(0, 0, 0, 0);
			this.value = value;
			this.image = image;
		}
		
		public void resizeToScreen(int screenWidth, int screenHeight) {
			screenResizedBounds.set(
				(int)(bounds.left*screenWidth/800f),
				(int)(bounds.top*screenHeight/480f),
				(int)(bounds.right*screenWidth/800f),
				(int)(bounds.bottom*screenHeight/480f));
		}
	}
}
