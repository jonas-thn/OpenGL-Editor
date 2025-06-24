#include "Application.h"

void Application::Init()
{
	display.emplace(800, 600, "OpenGL PBR");
	running = display->Init();

	gui.emplace(display->GetWindow(), display->GetGLContext());
	gui->Init();
}

void Application::Setup()
{
}

void Application::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		gui->ProcessEvent(event);

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
}

void Application::Update()
{
}

void Application::Render()
{
	display->Clear(1, 1, 1, 1);
	gui->Render();
	display->SwapBuffers();
}

void Application::Cleanup()
{
}
