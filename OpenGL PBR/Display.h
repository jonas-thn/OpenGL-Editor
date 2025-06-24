#pragma once

#include <GL/glew.h>

#include <SDL.h>

#include <iostream>
#include <string>

class Display
{
public:
	Display() = default;
	Display(int width, int height, std::string title) : width(width), height(height), title(title) {}
	~Display();

	bool Init();
	void Clear(float r, float g, float b, float a) const;
	void SwapBuffers() const;	

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	SDL_Window* GetWindow() const { return window; }
	SDL_GLContext GetGLContext() { return glContext; }

private:
	int width;
	int height;
	std::string title;
	SDL_Window* window;
	SDL_GLContext glContext;
};

