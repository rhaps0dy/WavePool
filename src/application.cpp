#include "application.h"
#include "utils.h"
#include "image.h"
#include "readme.h"

Application::Application(const char* caption, int width, int height) :
	doMove(false)
{
	this->window = createWindow(caption, width, height);

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT
	// things create here cannot access opengl
	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(NULL);
}

//Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
	showREADME();
	wp = new WavePool(window_width, window_height);
}

Application::~Application()
{
	delete wp;
}

//render one frame
void Application::render(void)
{
	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderImage(wp->getNewImage());

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

//called after render
void Application::update(Uint dt)
{
	wp->update(dt);
	if(doMove)
		wp->move(&mouse_position);
}

//keyboard press event
void Application::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: exit(0); break; //ESC key, kill the app
		case SDLK_p:
			wp->togglePause();
			break;
		case SDLK_a:
			wp->add(&mouse_position);
			break;
		case SDLK_d:
			wp->remove();
			break;
		case SDLK_q:
			wp->addWL(10);
			break;
		case SDLK_z:
			wp->rmWL(10);
			break;
		case SDLK_w:
			wp->addAmp(10);
			break;
		case SDLK_x:
			wp->rmAmp(10);
			break;
		case SDLK_e:
			wp->addSpd(10);
			break;
		case SDLK_c:
			wp->rmSpd(10);
			break;
		case SDLK_1:
			wp->setColor(0);
			break;
		case SDLK_2:
			wp->setColor(1);
			break;
		case SDLK_3:
			wp->setColor(2);
			break;
	}
}

//mouse button event
void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_RIGHT)
		wp->select(&mouse_position);
	else if(event.button == SDL_BUTTON_LEFT)
		doMove = true;
}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	if(event.button == SDL_BUTTON_LEFT)
		doMove = false;
}

//when the app starts
void Application::start()
{
	launchLoop(this);
}

void Application::setWindowSize(int width, int height)
{
	glViewport( 0,0, width, height );
	window_width = width;
	window_height = height;
	wp->resize(width, height);
}
