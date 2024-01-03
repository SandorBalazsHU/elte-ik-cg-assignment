/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#include "WorldOfWarships.h"

GLint WorldOfWarships::ul(const char* uniformName) noexcept
{
	GLuint programID = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, reinterpret_cast<GLint*>(&programID));
	return glGetUniformLocation(programID, uniformName);
}

void WorldOfWarships::setupDebugCallback()
{
	GLint context_flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
	if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
		glDebugMessageCallback(SDL_GLDebugMessageCallback, nullptr);
	}
};

void WorldOfWarships::keyboardDown(const SDL_KeyboardEvent& key)
{
	if (key.repeat == 0)
	{
		if (key.keysym.sym == SDLK_F5 && key.keysym.mod & KMOD_CTRL)
		{
			cleanShaders();
			initShaders();
		}
		if (key.keysym.sym == SDLK_F1)
		{
			GLint polygonModeFrontAndBack[2] = {};
			glGetIntegerv(GL_POLYGON_MODE, polygonModeFrontAndBack);
			GLenum polygonMode = (polygonModeFrontAndBack[0] != GL_FILL ? GL_FILL : GL_LINE);
			glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
		}
		if (key.keysym.sym == SDLK_UP)
		{
			shipAccelerate(allyShips[playerShipID]);
		}
		if (key.keysym.sym == SDLK_DOWN)
		{
			shipSlow(allyShips[playerShipID]);
		}
		/*if (key.keysym.sym == SDLK_UP)
		{
			allyShips[playerShipID].velocity.x += 5.0f;
		}
		if (key.keysym.sym == SDLK_UP)
		{
			allyShips[playerShipID].velocity.x += 5.0f;
		}*/
	}
	camera.KeyboardDown(key);
};

void WorldOfWarships::keyboardUp(const SDL_KeyboardEvent& key)
{
	camera.KeyboardUp(key);
};

void WorldOfWarships::mouseMove(const SDL_MouseMotionEvent& mouse)
{
	camera.MouseMove(mouse);
};

void WorldOfWarships::mouseDown(const SDL_MouseButtonEvent& mouse)
{
};

void WorldOfWarships::mouseUp(const SDL_MouseButtonEvent& mouse)
{
};

void WorldOfWarships::mouseWheel(const SDL_MouseWheelEvent& wheel)
{
	camera.MouseWheel(wheel);
};

void WorldOfWarships::resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);
	camera.Resize(_w, _h);
};