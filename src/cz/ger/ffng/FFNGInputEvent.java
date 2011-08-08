package cz.ger.ffng;

public class FFNGInputEvent {
	// see InputAgent::own_update and FFNGInputEvent.h (SDL_Event)
	public static final int NOEVENT = 0;
	public static final int QUIT = 1;
	public static final int KEYDOWN = 2;
	public static final int KEYUP = 3;
	public static final int MOUSEBUTTONDOWN = 4;
	
	public int type = 0;
	// mouse button down
	public float x = -1f, y = -1f;
	// key events
	public int key = -1;
	
	// No event
	FFNGInputEvent(int type) {
		this.type = type;
	}

	// Mouse Button Down
	FFNGInputEvent(int type, float x, float y) {
		this.type = type;
		this.x = x;
		this.y = y;
	}

	// Key event
	FFNGInputEvent(int type, int key) {
		this.type = type;
		this.key = key;
	}
}
