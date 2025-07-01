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
#include "Skybox.h"
#include "Screen.h"

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

	unsigned int screenFBO;
	unsigned int screenFBOTexture;
	unsigned int screenFBODepth;

	unsigned int postFBO;
	unsigned int postFBOTexture;
	unsigned int postFBODepth;

	//Shader
	std::optional<Shader> simpleShader;
	std::optional<Shader> skyboxShader;
	std::optional<Shader> backgroundShader;
	std::optional<Shader> postShader;

	//Objects
	Mesh triangle = Mesh(std::vector<float>(std::begin(triangleVertices), std::end(triangleVertices)));
	Mesh quad = Mesh(std::vector<float>(std::begin(quadVertices), std::end(quadVertices)), std::vector<unsigned int>(std::begin(quadIndices), std::end(quadIndices)));
	Mesh cube = Mesh(std::vector<float>(std::begin(cubeVertices), std::end(cubeVertices)));
	Mesh sphere = Mesh("./Models/sphere.obj");
	Mesh monkey = Mesh("./Models/monkey.obj");
	Mesh ring = Mesh("./Models/ring.obj");
	Mesh cone = Mesh("./Models/cone.obj");
	Mesh cylinder = Mesh("./Models/cylinder.obj");

	Mesh* currentMesh = &cube;

	std::optional<Screen> background;
	std::optional<Screen> postScreen;

	//Materials
	std::optional<Material> noMaterial;
	std::optional<Material> brickMaterial;
	std::optional<Material> containerMaterial;
	std::optional<Material> woodMaterial;
	std::optional<Material> concreteMaterial;
	std::optional<Material> boxMaterial;

	std::optional<Material>* currentMaterial = &noMaterial;

	//Extra
	int width = 800;
	int height = 700;
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	float distance = 2.5f;
	bool ortho = false;
	ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPosition = glm::vec3(-0.0f, 1.0f, 1.0f);
	bool skyboxActive = false;
	std::optional<Skybox> skybox;
	float roughness;
	ImVec4 emissionColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	float emissionRadius = 10.0f;

private:
	int GetNextTextureIndex();
};

