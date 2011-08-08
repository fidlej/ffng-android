package cz.ger.ffng;

import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.graphics.Paint.Align;

public class FFNGFont {
    private Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
    private Typeface face;
    private Rect bounds = new Rect();
    
    FFNGFont(String file, int size) {
    	face = Typeface.createFromAsset(FFNG.assets, file);
    	paint.setTextAlign(Align.LEFT);
    	paint.setTypeface(face);
    	paint.setTextSize(size);     // TODO convert size(mm) to pixelsize(dpi?)
    }
    
    public static FFNGFont createFont(String file, int size) {
    	return new FFNGFont(file, size);
    }
    
    public int getWidth(String text) {
    	synchronized(bounds) {
    		bounds = getBounds(text);
	    	return bounds.width();
    	}
    }
    
    synchronized public int getHeight(String text) {
    	synchronized(bounds) {
	    	bounds = getBounds(text);
	    	return bounds.height();
    	}
    }
    
    synchronized public Rect getBounds(String text) {
    	paint.getTextBounds(text, 0, text.length(), bounds);
    	return bounds;
    }
    
    public void render(Canvas canvas, String text, int frontColor, int bgColor, int outlineWidth) {
    	/* apply background color
    	int a = (bgColor & 0xFF000000) >> 24,
    	    r = (bgColor & 0x00FF0000) >> 16,
    	    g = (bgColor & 0x0000FF00) >> 8,
    	    b = (bgColor & 0x000000FF);
    	canvas.drawARGB(a, r, g, b);
    	*/
    	bounds = getBounds(text);
    	if (outlineWidth > 0) {   // simple shadow because I don't how to draw an outline
    		paint.setARGB(255, 0, 0, 0);
    		paint.setColor(0xff000000);
    		paint.setStrokeWidth(outlineWidth);
    		canvas.drawText(text, -bounds.left-outlineWidth, -bounds.top-outlineWidth, paint);
    		canvas.drawText(text, -bounds.left+outlineWidth, -bounds.top+outlineWidth, paint);
    		canvas.drawText(text, -bounds.left-outlineWidth, -bounds.top+outlineWidth, paint);
    		canvas.drawText(text, -bounds.left+outlineWidth, -bounds.top-outlineWidth, paint);
    	}
    	paint.setARGB(255, 255, 255, 255);
    	paint.setColor(frontColor);
    	paint.setStrokeWidth(0);
    	canvas.drawText(text, -bounds.left, -bounds.top, paint);
    }
}
