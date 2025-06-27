#pragma once

#include <vector>
#include "Shader.h"
#include "Material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/transform.hpp>

class Mesh
{
public:
	Mesh(std::vector<float> vertices, std::vector<unsigned int> indices = {}) : vertices(vertices), indices(indices) {}
	~Mesh() = default;
	void Init();

	void Rotate(float angle, const glm::vec3& axis);
	void ResetScale();
	void SetScale(glm::vec3 scale);

	void Draw(Shader& shader, Material& material, glm::mat4& view, glm::mat4& projection) const;

private:
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;

	glm::mat4 model = glm::mat4(1.0f);
};

