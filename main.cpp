/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#include "Window.h"

int main( int argc, char* args[] ) 
{
	Window window;
	if (window.open() != 0) {
		return 1;
	}

	SDL_Window* sdlWindow = window.getSDLwindow();
	{
		if (window.init() != 0) return 1;
		while (!window.isQuit())
		{
			window.eventListener();
			window.update();
			window.render();
		}
		window.clean();
	} 
	
	window.close();
	return 0;
}