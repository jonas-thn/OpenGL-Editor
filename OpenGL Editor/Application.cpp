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
	postShader.emplace("shaders/post.vert", "shaders/post.frag");

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
	brickMaterial.emplace(GetNextTextureIndex(), "./Textures/brickwall.jpg", 0.0f);
	containerMaterial.emplace(GetNextTextureIndex(), "./Textures/container2.png", 0.0f);
	woodMaterial.emplace(GetNextTextureIndex(), "./Textures/wood.png", 0.0f);

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
	currentMesh->SetScale(glm::vec3(1.0, 1.0, 1.0));

	//SCREEN FRAMEBUFFER
	glGenFramebuffers(1, &screenFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);

	glGenTextures(1, &screenFBOTexture);
	glBindTexture(GL_TEXTURE_2D, screenFBOTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, display->GetWidth(), display->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenFBOTexture, 0);

	glGenRenderbuffers(1, &screenFBODepth);
	glBindRenderbuffer(GL_RENDERBUFFER, screenFBODepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, display->GetWidth(), display->GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, screenFBODepth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	postScreen.emplace(std::vector<float>(std::begin(screenVertices), std::end(screenVertices)), GetNextTextureIndex(), screenFBOTexture);
	postScreen->Init();

	//POST FRAMEBUFFER
	glGenFramebuffers(1, &postFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, postFBO);

	glGenTextures(1, &postFBOTexture);
	glBindTexture(GL_TEXTURE_2D, postFBOTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, display->GetWidth(), display->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postFBOTexture, 0);

	glGenRenderbuffers(1, &postFBODepth);
	glBindRenderbuffer(GL_RENDERBUFFER, postFBODepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, display->GetWidth(), display->GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, postFBODepth);

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
		if (event.type == SDL_MOUSEMOTION) 
		{
			int mouseX, mouseY;
			int buttons = SDL_GetMouseState(&mouseX, &mouseY);

			if ((buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) && (mouseX > 0) && (mouseX < width - 250) && (mouseY > 0) && (mouseY < height))
			{
				int dx = event.motion.xrel;
				int dy = event.motion.yrel;

				currentMesh->RotateWorld(dx, dy, 0.3f);
			}
		}

		if (event.type == SDL_MOUSEWHEEL) 
		{
			distance -= event.wheel.y * 0.1f;
			distance = std::clamp(distance, 1.0f, 5.0f);
			view = glm::lookAt(
				glm::vec3(0.0f, 0.0f, distance), // cam pos
				glm::vec3(0.0f, 0.0f, 0.0f), // look at
				glm::vec3(0.0f, 1.0f, 0.0f)  // up
			);

			DataTransfer::Instance().SetChanged(ORTHO_CHANGED);
		}
	}
}

void Application::Update()
{
	deltaTime = (SDL_GetTicks() - lastFrame) / 1000.0f; 
	lastFrame = SDL_GetTicks();

	if (DataTransfer::Instance().HasChanged(ORTHO_CHANGED))
	{
		DataTransfer::Instance().ClearChanged(ORTHO_CHANGED);

		ortho = DataTransfer::Instance().GetOrtho();

		if (!ortho)
		{
			projection = glm::perspective(glm::radians(45.0f), (float)display->GetWidth() / (float)display->GetHeight(), 0.1f, 100.0f);
			currentMesh->SetScale(glm::vec3(1.0, 1.0, 1.0));
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
			currentMesh->SetScale(glm::vec3(1.0, 1.0, 1.0));
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

		currentMaterial->value().UpdateRoughness(roughness);
	}

	if (DataTransfer::Instance().HasChanged(EMISSION_COLOR_CHANGED))
	{
		DataTransfer::Instance().ClearChanged(EMISSION_COLOR_CHANGED);

		emissionColor = DataTransfer::Instance().GetEmissionColor();
	}

	if (DataTransfer::Instance().HasChanged(EMISSION_RADIUS_CHANGED))
	{
		DataTransfer::Instance().ClearChanged(EMISSION_RADIUS_CHANGED);

		emissionRadius = DataTransfer::Instance().GetEmissionRadius();
	}

	if (DataTransfer::Instance().HasChanged(MATERIAL_CHANGED))
	{
		DataTransfer::Instance().ClearChanged(MATERIAL_CHANGED);

		MaterialSelection materialSelection = DataTransfer::Instance().GetMaterialSelection();

		switch (materialSelection)
		{
		case MaterialSelection::Brick:
			currentMaterial = &brickMaterial;
			break;
		case MaterialSelection::Wood:
			currentMaterial = &woodMaterial;
			break;
		case MaterialSelection::Container:
			currentMaterial = &containerMaterial;
			break;
		}
	}
}

void Application::Render()
{
	//FIRST PASS - Scene
	glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
	display->Clear(0.05, 0.05, 0.05, 1);

	if(skyboxActive)
	{
		background->Draw(backgroundShader.value(), glm::vec3(emissionColor.x, emissionColor.y, emissionColor.z), emissionRadius);
	}
	
	currentMesh->Draw(simpleShader.value(), currentMaterial->value(), view, projection, glm::vec3(color.x, color.y, color.z), 0, skybox->GetCubemapTexture(), distance, glm::vec3(emissionColor.x, emissionColor.y, emissionColor.z));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//SECOND PASS - Post
	glBindFramebuffer(GL_FRAMEBUFFER, postFBO);
	glDisable(GL_DEPTH_TEST);
	display->Clear(0.05, 0.05, 0.05, 1);

	postScreen->Draw(postShader.value(), glm::vec3(emissionColor.x, emissionColor.y, emissionColor.z), emissionRadius);

	glEnable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//THIRD PASS - Gui
	display->Clear(0, 0, 0, 1);
	gui->Render(postFBOTexture, display.value());
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
