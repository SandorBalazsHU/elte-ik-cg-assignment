#include "Window.h"

Window::Window() {
	sdlWindow = nullptr;
};

Window::~Window() {};

int Window::open() {
	SDL_LogSetPriority(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR);
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "[SDL initialization] Error during the SDL initialization: %s", SDL_GetError());
		return 1;
	}

	std::atexit(SDL_Quit);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#ifdef _DEBUG 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	sdlWindow = SDL_CreateWindow("World of Warships", 100, 100, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

	if (sdlWindow == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "[Window creation] Error during the SDL initialization: %s", SDL_GetError());
		return 1;
	}

	sdlContext = SDL_GL_CreateContext(sdlWindow);
	if (sdlContext == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "[OGL context creation] Error during the creation of the OGL context: %s", SDL_GetError());
		return 1;
	}

	SDL_GL_SetSwapInterval(1);

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "[GLEW] Error during the initialization of glew.");
		return 1;
	}

	int glVersion[2] = { -1, -1 };
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Running OpenGL %d.%d", glVersion[0], glVersion[1]);

	if (glVersion[0] == -1 && glVersion[1] == -1)
	{
		SDL_GL_DeleteContext(sdlContext);
		SDL_DestroyWindow(sdlWindow);

		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "[OGL context creation] Error during the inialization of the OGL context! Maybe one of the SDL_GL_SetAttribute(...) calls is erroneous.");

		return 1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(sdlWindow, sdlContext);
	ImGui_ImplOpenGL3_Init();

	return 0;
};

void Window::close() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_GL_DeleteContext(sdlContext);
	SDL_DestroyWindow(sdlWindow);
};