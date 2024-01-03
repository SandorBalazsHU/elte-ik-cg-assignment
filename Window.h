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

class Window
{
public:
	Window();
	~Window();
	int open();
	void close();

	SDL_Window* getSDLwindow() {
		return sdlWindow;
	};

	SDL_GLContext	getSDLcontext() {
		return sdlContext;
	};

private:
	SDL_Window* sdlWindow;
	SDL_GLContext sdlContext;
};

