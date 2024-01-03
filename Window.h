/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#pragma once

#include <imgui.h>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include "WorldOfWarships.h"

class Window
{
public:
	Window();
	~Window();
	int open();
	void close();
	int init();
	void eventListener();
	void update();
	void render();
	void clean();

	SDL_Window* getSDLwindow() {
		return sdlWindow;
	};

	SDL_GLContext	getSDLcontext() {
		return sdlContext;
	};

	bool isQuit() {
		return quit;
	};
private:
	SDL_Window* sdlWindow = nullptr;
	SDL_GLContext sdlContext;
	bool quit = false;
	SDL_Event event;
	WorldOfWarships WorldOfWarships;
};