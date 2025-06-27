#pragma once

#include <optional>

#include "Display.h"
#include "GUI.h"

#include <GL/glew.h>

#include <SDL.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl2.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Mesh.h"
#include "data.h"
#include "Material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	float deltaTime;
	float lastFrame;
	bool running;
	std::optional<Display> display;
	std::optional<GUI> gui;

	unsigned int fbo;
	unsigned int fboTexture;
	unsigned int fboDepth;

	//Shader
	std::optional<Shader> simpleShader;

	//Objects
	Mesh quad = Mesh(std::vector<float>(std::begin(quadVertices), std::end(quadVertices)), std::vector<unsigned int>(std::begin(quadIndices), std::end(quadIndices)));
	Mesh cube = Mesh(std::vector<float>(std::begin(cubeVertices), std::end(cubeVertices)));

	//Materials
	std::optional<Material> material;

	//Extra
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	float distance = 2.0f;
	bool ortho = false;
};

