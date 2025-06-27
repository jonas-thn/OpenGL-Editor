#include "Application.h"
#include "DataTransfer.h"

void Application::Init()
{
	lastFrame = SDL_GetTicks();

	display.emplace(800, 600, "OpenGL PBR");
	running = display->Init();

	gui.emplace(display->GetWindow(), display->GetGLContext());
	gui->Init();

	//Shaders
	simpleShader.emplace("shaders/simple.vert", "shaders/simple.frag");

	//Objects
	quad.Init();
	cube.Init();

	//Materials
	material.emplace(0, "./Textures/brickwall.jpg");
}

void Application::Setup()
{
	view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, distance), // cam pos
		glm::vec3(0.0f, 0.0f, 0.0f), // look at
		glm::vec3(0.0f, 1.0f, 0.0f)  // up
	);

	ortho = DataTransfer::Instance().GetOrtho();

	if(ortho)
	{
		projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

	}
	else
	{
		projection = glm::perspective(glm::radians(45.0f), (float)display->GetWidth() / (float)display->GetHeight(), 0.1f, 100.0f);
		//quad.SetScale(glm::vec3(1.0, 0.8, 1.0));
		cube.SetScale(glm::vec3(1.0, 0.8, 1.0));
	}

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

	glEnable(GL_DEPTH_TEST);
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
	deltaTime = (SDL_GetTicks() - lastFrame) / 1000.0f; 
	lastFrame = SDL_GetTicks();

	//quad.Rotate(deltaTime * 50, glm::vec3(0, 1, 0));
	cube.Rotate(deltaTime * 50, glm::vec3(0, 1, 0));

	if (DataTransfer::Instance().distanceChanged)
	{
		DataTransfer::Instance().distanceChanged = false;

		distance = DataTransfer::Instance().GetDistance();
		
		view = glm::lookAt(
			glm::vec3(0.0f, 0.0f, distance), // cam pos
			glm::vec3(0.0f, 0.0f, 0.0f), // look at
			glm::vec3(0.0f, 1.0f, 0.0f)  // up
		);

		DataTransfer::Instance().orthoisChanged = true;
	}
	

	if (DataTransfer::Instance().orthoisChanged)
	{
		DataTransfer::Instance().orthoisChanged = false;

		ortho = DataTransfer::Instance().GetOrtho();

		if (!ortho)
		{
			projection = glm::perspective(glm::radians(45.0f), (float)display->GetWidth() / (float)display->GetHeight(), 0.1f, 100.0f);
			//quad.SetScale(glm::vec3(1.0, 0.8, 1.0));
			cube.SetScale(glm::vec3(1.0, 0.8, 1.0));

		}
		else
		{			
			float newDistance = distance * 0.5f;
			projection = glm::ortho(-newDistance, newDistance, -newDistance, newDistance, 0.1f, 100.0f);
			//cube.ResetScale();
			cube.ResetScale();
		}
	}
}

void Application::Render()
{
	//FIRST PASS
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	display->Clear(0.05, 0.05, 0.05, 1);
	//quad.Draw(simpleShader.value(), material.value(), view, projection);
	cube.Draw(simpleShader.value(), material.value(), view, projection);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//SECOND PASS	
	display->Clear(0, 0, 0, 1);
	gui->Render(fboTexture, display.value());
	display->SwapBuffers();

}

void Application::Cleanup()
{
}
