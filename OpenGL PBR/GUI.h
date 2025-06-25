#pragma once

#include <GL/glew.h>
#include <SDL.h>

#include "Display.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl2.h"
#include "ImGui/imgui_impl_opengl3.h"

struct ImageData
{
	ImVec2 bottomLeft;
	ImVec2 topRight;

	ImVec2 uv0;
	ImVec2 uv1;

	float aspectRatio;
	float textureWidth;
	float textureHeight;
};

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
	void CalculateImageData();

	SDL_Window* window;
	SDL_GLContext glContext;

	ImageData imageData;
	ImVec2 bottomLeft = ImVec2(0.0f, 0.0f);
	ImVec2 topRight = ImVec2(1.0f, 1.0f);
	float height = 400.0f;
};

