#pragma once

#include <optional>

#include "Display.h"
#include "GUI.h"

#include <GL/glew.h>

#include <SDL.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl2.h"
#include "ImGui/imgui_impl_opengl3.h"

class Application
{
public:
	Application() = default;
	~Application() = default;

	void Init();
	void Setup();
	void ProcessInput();
	void Update();
	void Render();
	void Cleanup();

	bool IsRunning() const { return running; }

private:
	bool running;
	std::optional<Display> display;
	std::optional<GUI> gui;
};

