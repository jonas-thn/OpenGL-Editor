#pragma once

#include <vector>
#include "Shader.h"
#include "Material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/transform.hpp>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh
{
public:
	Mesh(std::vector<float> vertices, std::vector<unsigned int> indices = {}) : vertices(vertices), indices(indices) {}
	Mesh(std::string path) : path(path) {}

	~Mesh() = default;
	void Init(glm::vec3 lightPos, int doubleLighting = 0);

	void Rotate(float angle, const glm::vec3& axis);
	void ResetScale();
	void SetScale(glm::vec3 scale);

	std::vector<float>& GetVertices() { return vertices; }
	std::vector<unsigned int>& GetIndices() { return indices; }

	void Draw(Shader& shader, Material& material, glm::mat4& view, glm::mat4& projection, glm::vec3 color) const;

private:
	void LoadModel();
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	std::string path = "";

	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 lightPosition;
	int doubleLighting = 0;
};

