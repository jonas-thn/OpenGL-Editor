#pragma once

#include "stb_image.h"

#include <vector>
#include <string>
#include <iostream>

#include "Shader.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Skybox
{
public:
	Skybox();

	void Draw(Shader& shader, glm::mat4& view, glm::mat4& projection, int textureIndex);

	unsigned int GetCubemapTexture();

private:
	unsigned int LoadCubemap();

	unsigned int cubeVAO, cubeVBO;
	unsigned int skyboxVAO, skyboxVBO;

	unsigned int cubemapTexture;

	static std::vector<std::string> cubemapFaces;

	static float skyboxVertices[];
};

