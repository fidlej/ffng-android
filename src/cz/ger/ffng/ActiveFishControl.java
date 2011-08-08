package cz.ger.ffng;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;

public class ActiveFishControl {

	private static final int GOAL_NONE = 0;
	private static final int GOAL_SMALL = 1;
	private static final int GOAL_BIG = 2;
	
	private Bitmap small;
	private Bitmap big;
	
	private boolean isBig = false;
	private int goal = GOAL_NONE;
	private float animProgress = 0;
	private float animStep = 0.1f;
	
	ActiveFishControl() {
		small = BitmapFactory.decodeStream(FFNG.files.internal("gui/small.png").read(), null, null);
		big = BitmapFactory.decodeStream(FFNG.files.internal("gui/big.png").read(), null, null);
	}
	
	public void switchFishes(boolean isBig) {
		this.isBig = isBig;
		goal = isBig ? GOAL_BIG : GOAL_SMALL;
	}
	
	public void render(Canvas canvas, int x, int y, int surfaceWidth, int surfaceHeight) {
		float scaleX = surfaceWidth / 800f,
    	      scaleY = surfaceHeight / 480f;

		if (animProgress >= 0.5f) {
			renderBig(canvas, x, y, scaleX, scaleY);
			renderSmall(canvas, x, y, scaleX, scaleY);
		} else {
			renderSmall(canvas, x, y, scaleX, scaleY);
			renderBig(canvas, x, y, scaleX, scaleY);
		}
		
		step();
	}
	
	private Rect renderRect = new Rect();
	private Paint renderPaint = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.FILTER_BITMAP_FLAG);
	public void renderBig(Canvas canvas, int x, int y, float scaleX, float scaleY) {
		double xx = x + 18,
	           yy = y + 33 - Math.sin(animProgress * Math.PI) * 4;
		renderRect.set(
			(int)(xx * scaleX),
			(int)(yy * scaleY),
			(int)((xx + 60) * scaleX),
			(int)((yy + 30) * scaleY));
		renderPaint.setAlpha((int)(50 + animProgress * 150));	
		canvas.drawBitmap(big, null, renderRect, renderPaint);
	}

	public void renderSmall(Canvas canvas, int x, int y, float scaleX, float scaleY) {
		double xx = x + 25,
		       yy = y + 52 + Math.sin(animProgress * Math.PI) * 4;
		renderRect.set(
			(int)(xx * scaleX),
			(int)(yy * scaleY),
			(int)((xx + 45) * scaleX),
			(int)((yy + 15) * scaleY));
		renderPaint.setAlpha((int)(200 - animProgress * 150));	
		canvas.drawBitmap(small, null, renderRect, renderPaint);
	}
	
	protected void step() {
		if (animProgress <= 0 && animProgress >= 1) {
			goal = GOAL_NONE;
		}
		
		if (goal != GOAL_NONE) {
			animProgress += (goal == GOAL_SMALL ? -animStep : animStep);
			if (animProgress < 0) animProgress = 0;
			if (animProgress > 1) animProgress = 1;
		}
	}
}
