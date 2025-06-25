#pragma once

#include <vector>
#include "Shader.h"

class Mesh
{
public:
	Mesh(std::vector<float> vertices, std::vector<unsigned int> indices = {}) : vertices(vertices), indices(indices) {}
	~Mesh() = default;
	void Init();
	void Draw(Shader& shader) const;

private:
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
};

