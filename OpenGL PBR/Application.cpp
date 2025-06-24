#include "Application.h"

void Application::Init()
{
	display.emplace(800, 600, "OpenGL PBR");
	running = display->Init();

	gui.emplace(display->GetWindow(), display->GetGLContext());
	gui->Init();

	simpleShader.emplace("shaders/simple.vert", "shaders/simple.frag");
	triangle.Init();
}

void Application::Setup()
{
	//FRAMEBUFFER
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &fboTexture);
	glBindTexture(GL_TEXTURE_2D, fboTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, display->GetWidth(), display->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

	glGenRenderbuffers(1, &fboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, fboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, display->GetWidth(), display->GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fboDepth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
	//FIRST PASS
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	display->Clear(1, 1, 1, 1);
	triangle.Draw(simpleShader.value());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//SECOND PASS	
	display->Clear(0, 0, 0, 1);
	gui->Render(fboTexture, display.value());
	display->SwapBuffers();

}

void Application::Cleanup()
{
}
