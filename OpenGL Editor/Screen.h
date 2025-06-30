#pragma once

#include <vector>
#include "Shader.h"
#include "Material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/transform.hpp>

class Screen
{
public:
	Screen(std::vector<float> vertices, int textureIndex, unsigned int textureID);
	Screen(std::vector<float> vertices, int textureIndex, const char* path);

	void Init();

	std::vector<float>& GetVertices() { return vertices; }

	void Draw(Shader& shader) const;

private:
	std::vector<float> vertices;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int textureID;
	int textureIndex;
};