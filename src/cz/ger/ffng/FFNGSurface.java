package cz.ger.ffng;

import android.graphics.AvoidXfermode;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.graphics.Rect;
import android.graphics.Bitmap.Config;

public class FFNGSurface {

	private Bitmap bitmap = null;
	private Canvas canvas = null;
	
	FFNGSurface(int width, int height) {
		bitmap = Bitmap.createBitmap(width, height, Config.ARGB_8888);
		canvas = new Canvas(bitmap);
	}

	//private BitmapFactory.Options options = new BitmapFactory.Options();
	FFNGSurface(String file) {
		/*
		options.inDither = false;
		Bitmap rawBitmap = BitmapFactory.decodeStream(FFNG.files.internal(file).read(), null, options);
		bitmap = rawBitmap.copy(Config.ARGB_8888, true);
		rawBitmap.recycle();
		canvas = new Canvas(bitmap);
		*/
		bitmap = BitmapFactory.decodeStream(FFNG.files.internal(file).read(), null, null); 
		canvas = null;
	}
	
	public static FFNGSurface newSurface(int width, int height) {
		return new FFNGSurface(width, height);
	}
	
	public static FFNGSurface newSurface(String file) {
		return new FFNGSurface(file);
	}
	
	public static FFNGSurface newSurface(FFNGFont font, String text, int frontColor, int bgColor, int outlineWidth) {
		Rect bounds = font.getBounds(text);
		FFNGSurface surface = newSurface(bounds.width() + 2*outlineWidth, bounds.height() + 2*outlineWidth);
		font.render(surface.getCanvas(), text, frontColor, bgColor, outlineWidth);
		return surface;
	}

	public void render(Canvas canvas, float x, float y) {
		canvas.drawBitmap(bitmap, x, y, null);
	}
	
	public Bitmap getBitmap() {
		return bitmap;
	}
	
	public Canvas getCanvas() {
		return canvas;
	}
	
	private Rect srcRect = new Rect(), dstRect = new Rect(); // reused in blitWavy
	public void blit(int dstx, int dsty, FFNGSurface source, int srcx, int srcy, int srcw, int srch) {
		if (canvas == null || !bitmap.isMutable()) {
			convertBitmapToMutable();
		}
		
		srcRect.set(srcx, srcy, srcx+srcw, srcy+srch);
		dstRect.set(dstx, dsty, dstx+srcw, dsty+srch);
		
		canvas.drawBitmap(source.getBitmap(), srcRect, dstRect, null);
	}

	private void convertBitmapToMutable() {
		Bitmap mutableBitmap = bitmap.copy(Config.ARGB_8888, true);
		bitmap.recycle();
		bitmap = mutableBitmap;
		canvas = new Canvas(bitmap);
	}
	
	private Paint avoidPaint = new Paint(Paint.FILTER_BITMAP_FLAG | Paint.ANTI_ALIAS_FLAG);
	private PorterDuffXfermode pdXfermode = new PorterDuffXfermode(PorterDuff.Mode.SRC_OUT);
	private Paint pdPaint = new Paint(Paint.FILTER_BITMAP_FLAG | Paint.ANTI_ALIAS_FLAG);
	public void blitMasked(int x, int y, final FFNGSurface mask, int color, final FFNGSurface layer) {
		if (canvas == null || !bitmap.isMutable()) {
			convertBitmapToMutable();
		}
		
		// draw layer2 where color in mask, otherwise layer1
		
		Bitmap bmask = mask.getBitmap(),
			   blayer = layer.getBitmap();
		int W = mask.getBitmap().getWidth(),
		    H = mask.getBitmap().getHeight();
		
		AvoidXfermode avoidXfermode = new AvoidXfermode(color, 0, AvoidXfermode.Mode.TARGET);
		
		avoidPaint.setColor(Color.TRANSPARENT);

        // draw the src/dst example into our offscreen bitmap
        int sc = canvas.saveLayer(x, y, x + W-1, y + H-1, null, 
                                  //Canvas.MATRIX_SAVE_FLAG |
                                  //Canvas.CLIP_SAVE_FLAG |
                                  Canvas.HAS_ALPHA_LAYER_SAVE_FLAG |
                                  Canvas.FULL_COLOR_LAYER_SAVE_FLAG //|
                                  //Canvas.CLIP_TO_LAYER_SAVE_FLAG
                                  );
        canvas.drawBitmap(bmask, x, y, null);
        avoidPaint.setXfermode(avoidXfermode);
        canvas.drawRect(x, y, x+W-1, y+H-1, avoidPaint);
        pdPaint.setXfermode(pdXfermode);
        canvas.drawBitmap(blayer, x, y, pdPaint);
        canvas.restoreToCount(sc);
	}
	
	public void blitWavy(final FFNGSurface surface, int x, int y, float amp, float periode, float shift) {
		if (canvas == null || !bitmap.isMutable()) {
			convertBitmapToMutable();
		}
		
		int width = surface.getWidth(),
		    height = surface.getHeight();
		float vertices[] = new float[(height+1) * 2 * 2];
		int index = 0;
	    for (int py = 0; py <= height; ++py) {
	    	int shiftX = (int)(0.5 + amp * Math.sin(py / periode + shift));
	    	
	    	vertices[index++] = x - shiftX;
	    	vertices[index++] = y + py;
	    	vertices[index++] = x + width - shiftX;
	    	vertices[index++] = y + py;
	    	
	    	/*
	        line_rect.x = shiftX;
	        line_rect.y = py;
	        dest_rect.x = m_loc.getX();
	        dest_rect.y = m_loc.getY() + py;
	        FFNGSurface::blitSurface(m_surface, &line_rect, screen, &dest_rect);
	        */

	    	int xx = (shiftX < 0) ? 0 : width - shiftX;
	    	int absShiftX = (shiftX < 0) ? -shiftX : shiftX;
	    	srcRect.set(xx, py, xx + absShiftX, py+1);
	    	dstRect.set(x + xx, y + py, x + xx + absShiftX, y + py + 1);
	    	canvas.drawBitmap(surface.getBitmap(), srcRect, dstRect, null);
	    }
	    
	    canvas.drawBitmapMesh(surface.getBitmap(), 1, height, vertices, 0, null, 0, null);
	}
	
	private final static int randomArraySize = 255;
	private static int randomArray[] = null;
	public void blitDisintegrate(final FFNGSurface surface, int x, int y, int disint) {
		if (canvas == null || !bitmap.isMutable()) {
			convertBitmapToMutable();
		}
		
		if (randomArray == null) {
			randomArray = new int[randomArraySize];
			
			for (int i = 0; i < randomArraySize; ++i) {
				randomArray[i] = (int)(Math.random() * 256);
			}
		}
		
		int width = surface.getWidth(),
		    height = surface.getHeight();
		
	    for (int py = 0; py < height; ++py) {
	        for (int px = 0; px < width; ++px) {
	            if (randomArray[(py * width + px) % randomArraySize] < disint) {
	            	long pixel = surface.getBitmap().getPixel(px, py);
	                if (((pixel >> 24) & 0xFF) == 255) {
	                	bitmap.setPixel(x+px, y+py, (int)pixel);
	                }
	            }
	        }
	    }
	}
	
	public void blitMirror(final FFNGSurface surface, int x, int y, int border) {
		if (canvas == null || !bitmap.isMutable()) {
			convertBitmapToMutable();
		}
		
		int width = surface.getWidth(),
		    height = surface.getHeight();
		long mask = surface.getBitmap().getPixel((int)(width / 2), (int)(height/2));

	    for (int py = 0; py < height; ++py) {
	        for (int px = 0; px < width; ++px) {
	            long pixel = surface.getBitmap().getPixel(px, py);
	            if (px > border && pixel == mask) {
	                long sample = bitmap.getPixel(x - px + border, y + py);
	                bitmap.setPixel(x + px, y + py, (int)sample);
	            }
	            else {
	                if (((pixel >> 24) & 0xFF) == 255) {
	                	bitmap.setPixel(x + px, y + py, (int)pixel);
	                }
	            }
	        }
	    }
	}
	
	public void blitReverse(final FFNGSurface surface, int x, int y) {
		if (canvas == null || !bitmap.isMutable()) {
			convertBitmapToMutable();
		}
		
		int width = surface.getWidth(),
		    height = surface.getHeight();
		
	    for (int py = 0; py < height; ++py) {
	        for (int px = 0; px < width; ++px) {
	            long pixel = surface.getBitmap().getPixel(px, py);
	            if (((pixel >> 24) & 0xFF) == 255) {
	            	bitmap.setPixel(x + width - 1 - px, y + py, (int)pixel);
	            }
	        }
	    }
	}
	
    public static final int ZX1 = 1;  //red
    public static final int ZX2 = 2;  //cyan
    public static final int ZX3 = 3;  //blue
    public static final int ZX4 = 4;  //yellow
    public static final int zxColors[] = {0xFF800000, 0xFF008080, 0xFF000080, 0xFF808000};
    private AvoidXfermode zxXfermode = new AvoidXfermode(0xFF808080, 0, AvoidXfermode.Mode.TARGET);
	private Paint zxPaint = new Paint();
	public void blitZX(final FFNGSurface surface, int x, int y, int zx, int countHeight, int stripeHeight) {
		if (canvas == null || !bitmap.isMutable()) {
			convertBitmapToMutable();
		}
		
		int width = surface.getWidth(),
     	    height = surface.getHeight();

        canvas.drawBitmap(surface.getBitmap(), x, y, null);
        zxPaint.setXfermode(zxXfermode);
        
	    for (int py = 0; py < height; ++py) {
	        countHeight++;
	        if (countHeight > stripeHeight) {
	            countHeight -= stripeHeight;
	            switch (zx) {
	                case 1:
	                    zx = ZX2;
	                    break;
	                case ZX2:
	                    zx = ZX1;
	                    break;
	                case ZX3:
	                    zx = ZX4;
	                    break;
	                default:
	                    zx = ZX3;
	                    break;
	            }
	        }

	        zxPaint.setColor(zxColors[zx-1]);
	        canvas.drawLine(x, y+py, x+width-1, y+py, zxPaint);

	        /*
	        for (int px = 0; px < width; ++px) {
	        	long spixel = surface.getBitmap().getPixel(px, py); 
	        	if (((spixel >> 24) & 0xFF) == 255) {
	        		bitmap.setPixel(x+px, y+py, (int)usedColor);
	            }
	        }
	        */
	    }
	}
	
	private Paint primitivesPaint = new Paint();
	public void line(int x1, int y1, int x2, int y2, int color) {
		if (canvas == null || !bitmap.isMutable()) {
			convertBitmapToMutable();
		}
		
		primitivesPaint.setColor(color);
		canvas.drawLine(x1, y1, x2, y2, primitivesPaint);
	}

	public void fillRect(int x, int y, int w, int h, int color) {
		if (canvas == null || !bitmap.isMutable()) {
			convertBitmapToMutable();
		}
		
		primitivesPaint.setColor(color);
		canvas.drawRect(x, y, x+w-1, y+h-1, primitivesPaint);
	}
	
	public void filledCircle(int x, int y, int radius, int color) {
		if (canvas == null || !bitmap.isMutable()) {
			convertBitmapToMutable();
		}
		
		primitivesPaint.setColor(color);
		canvas.drawCircle(x, y, radius, primitivesPaint);
	}
	
	public int getPixel(int x, int y) {
		return bitmap.getPixel(x, y);
	}

	public void putPixel(int x, int y, int color) {
		bitmap.setPixel(x, y, color);
	}
	
	public void dispose() {
		bitmap.recycle();
	}
	
	public int getWidth() {
		return bitmap.getWidth();
	}
	
	public int getHeight() {
		return bitmap.getHeight();
	}
	
	protected void finalize() throws Throwable
	{
		dispose();
		super.finalize(); //not necessary if extending Object.
	} 
}
