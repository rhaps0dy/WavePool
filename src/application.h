/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#ifndef APPLICATION_H
#define APPLICATION_H

#include "includes.h"
#include "framework.h"
#include "image.h"
#include "wavepool.h"

class Application
{
public:
	//window
	SDL_Window* window;
	Float window_width;
	Float window_height;

	//world renderer
	WavePool *wp;

#ifdef APP_TIME_ENABLED
	Float time;
#endif //APP_TIME_ENABLED

	//keyboard state
	const Uint8* keystate;

	//mouse state
	int mouse_state; //tells which buttons are pressed
	Vector2 mouse_position; //last mouse position
	Vector2 mouse_delta; //mouse movement in the last frame

	//constructor and destructor
	Application(const char* caption, int width, int height);
	~Application();

	//main methods
	void init( void );
	void render( void );
	void update( Float seconds_elapsed );

	//methods for events
	void onKeyPressed( SDL_KeyboardEvent event );
	void onMouseButton( SDL_MouseButtonEvent event );

	//other methods to control the app
	void setWindowSize(int width, int height) {
		glViewport( 0,0, width, height );
		this->window_width = width;
		this->window_height = height;
	}

	Vector2 getWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(w,h);
	}

	void start();
};

#endif
