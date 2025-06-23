#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL.h>

int main(int argc, char* argv[])
{
	const int WIDTH = 800;
	const int HEIGHT = 600;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow("OpenGL PBR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);

	if (!window)
	{
		std::cout << "Window Error" << std::endl;
		return 1;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);
	
	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
		return 1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	bool running = true;

	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = false;
					break;
				}
			}
		}

		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}