#include "Application.h"
#include "DataTransfer.h"

void Application::Init()
{
	lastFrame = SDL_GetTicks();

	display.emplace(width, height, "OpenGL Editor");
	running = display->Init();

	gui.emplace(display->GetWindow(), display->GetGLContext(), height, 250);
	gui->Init();

	//Shaders
	simpleShader.emplace("shaders/simple.vert", "shaders/simple.frag");
	skyboxShader.emplace("shaders/skybox.vert", "shaders/skybox.frag");
	backgroundShader.emplace("shaders/background.vert", "shaders/background.frag");

	//Objects
	triangle.Init(lightPosition, 1);
	quad.Init(lightPosition, 1);
	cube.Init(lightPosition);
	sphere.Init(lightPosition);
	monkey.Init(lightPosition);
	ring.Init(lightPosition);
	cone.Init(lightPosition);
	cylinder.Init(lightPosition);

	background.emplace(std::vector<float>(std::begin(screenVertices), std::end(screenVertices)), GetNextTextureIndex(), "./Textures/Cubemap/left.jpg");
	background->Init();

	//Materials
	material.emplace(GetNextTextureIndex(), "./Textures/brickwall.jpg", 0.0f);

	skybox.emplace();
}

void Application::Setup()
{
	view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, distance), // cam pos
		glm::vec3(0.0f, 0.0f, 0.0f), // look at
		glm::vec3(0.0f, 1.0f, 0.0f)  // up
	);

	ortho = DataTransfer::Instance().GetOrtho();

	projection = glm::perspective(glm::radians(45.0f), (float)display->GetWidth() / (float)display->GetHeight(), 0.1f, 100.0f);
	currentMesh->SetScale(glm::vec3(1.0, 0.8, 1.0));

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

	DataTransfer::Instance().UpdateVertices(currentMesh->GetVertices());
	DataTransfer::Instance().UpdateIndices(currentMesh->GetIndices());
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

	currentMesh->Rotate(deltaTime * 50, glm::vec3(0, 1, 0));

	if (DataTransfer::Instance().HasChanged(DISTANCE_CHANGED))
	{
		DataTransfer::Instance().ClearChanged(DISTANCE_CHANGED);

		distance = DataTransfer::Instance().GetDistance();
		
		view = glm::lookAt(
			glm::vec3(0.0f, 0.0f, distance), // cam pos
			glm::vec3(0.0f, 0.0f, 0.0f), // look at
			glm::vec3(0.0f, 1.0f, 0.0f)  // up
		);

		DataTransfer::Instance().SetChanged(ORTHO_CHANGED);
	}

	if (DataTransfer::Instance().HasChanged(ORTHO_CHANGED))
	{
		DataTransfer::Instance().ClearChanged(ORTHO_CHANGED);

		ortho = DataTransfer::Instance().GetOrtho();

		if (!ortho)
		{
			projection = glm::perspective(glm::radians(45.0f), (float)display->GetWidth() / (float)display->GetHeight(), 0.1f, 100.0f);
			currentMesh->SetScale(glm::vec3(1.0, 0.8, 1.0));
		}
		else
		{			
			float newDistance = distance * 0.5f;
			projection = glm::ortho(-newDistance, newDistance, -newDistance, newDistance, 0.1f, 100.0f);
			currentMesh->ResetScale();
		}
	}

	if (DataTransfer::Instance().HasChanged(MESH_CHANGED))
	{
		DataTransfer::Instance().ClearChanged(MESH_CHANGED);

		MeshSelection meshSelection = DataTransfer::Instance().GetMeshSelection();

		switch (meshSelection)
		{
		case MeshSelection::Triangle:
			currentMesh = &triangle;
			break;
		case MeshSelection::Quad:
			currentMesh = &quad;
			break;
		case MeshSelection::Cube:
			currentMesh = &cube;
			break;
		case MeshSelection::Sphere:
			currentMesh = &sphere;
			break;
		case MeshSelection::Ring:
			currentMesh = &ring;
			break;
		case MeshSelection::Cylinder:
			currentMesh = &cylinder;
			break;
		case MeshSelection::Cone:
			currentMesh = &cone;
			break;
		case MeshSelection::Monkey:
			currentMesh = &monkey;
			break;
		}

		if(DataTransfer::Instance().GetOrtho())
		{
			currentMesh->ResetScale();
		}
		else
		{
			currentMesh->SetScale(glm::vec3(1.0, 0.8, 1.0));
		}

		DataTransfer::Instance().UpdateVertices(currentMesh->GetVertices());
		DataTransfer::Instance().UpdateIndices(currentMesh->GetIndices());
	}

	if (DataTransfer::Instance().HasChanged(COLOR_CHANGED))
	{
		DataTransfer::Instance().ClearChanged(COLOR_CHANGED);

		color = DataTransfer::Instance().GetColor();
	}

	if (DataTransfer::Instance().HasChanged(SKYBOX_CHANGED))
	{
		DataTransfer::Instance().ClearChanged(SKYBOX_CHANGED);

		skyboxActive = DataTransfer::Instance().GetSkybox();
	}

	if (DataTransfer::Instance().HasChanged(ROUGHNESS_CHANGED))
	{
		DataTransfer::Instance().ClearChanged(ROUGHNESS_CHANGED);

		roughness = DataTransfer::Instance().GetRoughness();

		material.value().UpdateRoughness(roughness);
	}
}

void Application::Render()
{
	//FIRST PASS
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	display->Clear(0.05, 0.05, 0.05, 1);

	if(skyboxActive)
	{
		background->Draw(backgroundShader.value());
	}
	
	currentMesh->Draw(simpleShader.value(), material.value(), view, projection, glm::vec3(color.x, color.y, color.z), 0, skybox->GetCubemapTexture(), distance);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//SECOND PASS	
	display->Clear(0, 0, 0, 1);
	gui->Render(fboTexture, display.value());
	display->SwapBuffers();
}

void Application::Cleanup()
{
}

int Application::GetNextTextureIndex()
{
	static int number = 1;
	if (number < 31)
	{
		return number++;
	}
	else
	{
		return 0;
		std::cout << "Texture Index Error" << std::endl;
	}
}
