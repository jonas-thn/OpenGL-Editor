#include "Display.h"

Display::~Display()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool Display::Init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	if (!window)
	{
		std::cout << "Window Error" << std::endl;
		return false;
	}

	glContext = SDL_GL_CreateContext(window);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
		return false;
	}

	glViewport(0, 0, width, height);

	return true;
}

void Display::Clear(float r, float g, float b, float a) const
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::SwapBuffers() const
{
	SDL_GL_SwapWindow(window);
}
