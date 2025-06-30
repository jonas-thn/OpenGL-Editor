#pragma once

#include <GL/glew.h>
#include <SDL.h>

#include "Display.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl2.h"
#include "ImGui/imgui_impl_opengl3.h"

#include <algorithm>

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
	GUI(SDL_Window* window, SDL_GLContext glContext, float height, float windowSplit) : window(window), glContext(glContext), height(height), windowSplit(windowSplit) {}
	~GUI();
	void Init();
	void ProcessEvent(SDL_Event& event);
	void Render(unsigned int fboTexture, Display& display);

private:
	void CalculateImageData();

	void SetStyle();
	void ViewportWindow(unsigned int fboTexture);
	void SettingsWindow(Display& display);
	void PropertiesWindow(Display& display);

	SDL_Window* window;
	SDL_GLContext glContext;

	ImageData imageData;
	ImVec2 bottomLeft = ImVec2(0.1f, 0.0f);
	ImVec2 topRight = ImVec2(0.9f, 1.0f);
	float height;
	float windowSplit;

	ImVec4 blackColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 grey02Color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	ImVec4 grey04Color = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
	ImVec4 grey06Color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
	ImVec4 grey08Color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	ImVec4 whiteColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
};

