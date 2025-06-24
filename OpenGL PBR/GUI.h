#pragma once

#include <GL/glew.h>
#include <SDL.h>

#include "Display.h"

class GUI
{
public:
	GUI() = default;
	GUI(SDL_Window* window, SDL_GLContext glContext) : window(window), glContext(glContext) {}
	~GUI();
	void Init();
	void ProcessEvent(SDL_Event& event);
	void Render(unsigned int fboTexture, Display& display);

private:
	SDL_Window* window;
	SDL_GLContext glContext;
};

