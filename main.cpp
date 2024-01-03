/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#include "Window.h"
#include "WorldOfWarships.h"

int main( int argc, char* args[] ) 
{
	Window window;

	if (window.open() != 0) {
		return 1;
	}

	SDL_Window* sdlWindow = window.getSDLwindow();

	{
		bool quit = false;
		SDL_Event ev;

		WorldOfWarships app;

		if (!app.Init())
		{
			SDL_GL_DeleteContext(window.getSDLcontext());
			SDL_DestroyWindow(sdlWindow);
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "[app.Init] Error during the initialization of the application!");
			return 1;
		}

		while (!quit)
		{
			// amíg van feldolgozandó üzenet dolgozzuk fel mindet:
			while ( SDL_PollEvent(&ev) )
			{
				ImGui_ImplSDL2_ProcessEvent(&ev);
				bool is_mouse_captured    = ImGui::GetIO().WantCaptureMouse;    //kell-e az imgui-nak az egér
				bool is_keyboard_captured = ImGui::GetIO().WantCaptureKeyboard;	//kell-e az imgui-nak a billentyűzet

				switch (ev.type)
				{
					case SDL_QUIT:
						quit = true;
						break;
					case SDL_KEYDOWN:
						if ( ev.key.keysym.sym == SDLK_ESCAPE )
							quit = true;
						// ALT + ENTER vált teljes képernyőre, és vissza.
						if ( ( ev.key.keysym.sym == SDLK_RETURN )
							 && ( ev.key.keysym.mod & KMOD_ALT ) && !( ev.key.keysym.mod & ( KMOD_SHIFT | KMOD_CTRL | KMOD_GUI ) ) )
						{
							Uint32 FullScreenSwitchFlag = ( SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP ) ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP;
							SDL_SetWindowFullscreen(sdlWindow, FullScreenSwitchFlag );
						}
						if ( !is_keyboard_captured )
							app.KeyboardDown(ev.key);
						break;
					case SDL_KEYUP:
						if ( !is_keyboard_captured )
							app.KeyboardUp(ev.key);
						break;
					case SDL_MOUSEBUTTONDOWN:
						if ( !is_mouse_captured )
							app.MouseDown(ev.button);
						break;
					case SDL_MOUSEBUTTONUP:
						if ( !is_mouse_captured )
							app.MouseUp(ev.button);
						break;
					case SDL_MOUSEWHEEL:
						if ( !is_mouse_captured )
							app.MouseWheel(ev.wheel);
						break;
					case SDL_MOUSEMOTION:
						if ( !is_mouse_captured )
							app.MouseMove(ev.motion);
						break;
					case SDL_WINDOWEVENT:
						if ( ( ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED ) || ( ev.window.event == SDL_WINDOWEVENT_SHOWN ) )
						{
							int w, h;
							SDL_GetWindowSize(sdlWindow, &w, &h );
							app.Resize( w, h );
						}
						break;
				}
			}

			static Uint32 LastTick = SDL_GetTicks();
			Uint32 CurrentTick = SDL_GetTicks();
			SUpdateInfo updateInfo
			{ 
				static_cast<float>(CurrentTick) / 1000.0f, 
				static_cast<float>(CurrentTick - LastTick) / 1000.0f 
			};
			LastTick = CurrentTick;

			app.Update( updateInfo );
			app.Render();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame();

			ImGui::NewFrame();
			app.RenderGUI();
			ImGui::Render();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			SDL_GL_SwapWindow(sdlWindow);
		}
		app.Clean();
	} 
	
	window.close();

	return 0;
}
