/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#include "Window.h"

void Window::eventListener() {
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		bool is_mouse_captured = ImGui::GetIO().WantCaptureMouse;
		bool is_keyboard_captured = ImGui::GetIO().WantCaptureKeyboard;

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) quit = true;
			if ((event.key.keysym.sym == SDLK_RETURN) && (event.key.keysym.mod & KMOD_ALT) && !(event.key.keysym.mod & (KMOD_SHIFT | KMOD_CTRL | KMOD_GUI)))
			{
				Uint32 FullScreenSwitchFlag = (SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP;
				SDL_SetWindowFullscreen(sdlWindow, FullScreenSwitchFlag);
			}
			if (!is_keyboard_captured) WorldOfWarships.keyboardDown(event.key);
			break;
		case SDL_KEYUP:
			if (!is_keyboard_captured) WorldOfWarships.keyboardUp(event.key);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (!is_mouse_captured) WorldOfWarships.mouseDown(event.button);
			break;
		case SDL_MOUSEBUTTONUP:
			if (!is_mouse_captured) WorldOfWarships.mouseUp(event.button);
			break;
		case SDL_MOUSEWHEEL:
			if (!is_mouse_captured) WorldOfWarships.mouseWheel(event.wheel);
			break;
		case SDL_MOUSEMOTION:
			if (!is_mouse_captured) WorldOfWarships.mouseMove(event.motion);
			break;
		case SDL_WINDOWEVENT:
			if ((event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) || (event.window.event == SDL_WINDOWEVENT_SHOWN))
			{
				int w, h;
				SDL_GetWindowSize(sdlWindow, &w, &h);
				WorldOfWarships.resize(w, h);
			}
			break;
		}
	}
};

void Window::update() {
	static Uint32 LastTick = SDL_GetTicks();
	Uint32 CurrentTick = SDL_GetTicks();
	SUpdateInfo updateInfo
	{
		static_cast<float>(CurrentTick) / 1000.0f,
		static_cast<float>(CurrentTick - LastTick) / 1000.0f
	};
	LastTick = CurrentTick;

	WorldOfWarships.Update(updateInfo);
};

void Window::render() {
	WorldOfWarships.Render();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();

	ImGui::NewFrame();
	WorldOfWarships.RenderGUI();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(sdlWindow);
};

int Window::init() {
	if (!WorldOfWarships.Init())
	{
		close();
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "[app.Init] Error during the initialization of the application!");
		return 1;
	}
	return 0;
};

void Window :: clean() {
	WorldOfWarships.Clean();
};