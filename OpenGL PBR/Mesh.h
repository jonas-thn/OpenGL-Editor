#pragma once

#include <vector>
#include "Shader.h"

class Mesh
{
public:
	Mesh(std::vector<float> verts) : vertices(verts) {}
	~Mesh() = default;
	void Init();
	void Draw(Shader& shader) const;

private:
	std::vector<float> vertices;
	unsigned int VBO;
	unsigned int VAO;
};

