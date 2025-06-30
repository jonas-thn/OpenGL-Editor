#include "Mesh.h"

#include <GL/glew.h>

#include "DataTransfer.h"

void Mesh::Init(glm::vec3 lightPos, int doubleLighting)
{
	if (!path.empty())
	{
		LoadModel();
		std::cout << "Vertices: " << vertices.size() << std::endl;
	}

	if (vertices.empty()) return;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	this->lightPosition = lightPos;
	this->doubleLighting = doubleLighting;
}

void Mesh::Rotate(float angle, const glm::vec3& axis)
{
	model = glm::rotate(model, glm::radians(angle), axis);
}

void Mesh::RotateWorld(float dx, float dy, float sensitivity)
{
	glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), glm::radians(dx * sensitivity), glm::vec3(0, 1, 0));
	glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f), glm::radians(dy * sensitivity), glm::vec3(1, 0, 0));

	model = yawRotation * model;
	model = pitchRotation * model;
}

void Mesh::ResetScale()
{
	glm::vec3 scale;
	scale.x = glm::length(glm::vec3(model[0])); 
	scale.y = glm::length(glm::vec3(model[1])); 
	scale.z = glm::length(glm::vec3(model[2])); 

	glm::mat4 result = model;
	result[0] = glm::vec4(glm::normalize(glm::vec3(model[0])), 0.0f);
	result[1] = glm::vec4(glm::normalize(glm::vec3(model[1])), 0.0f);
	result[2] = glm::vec4(glm::normalize(glm::vec3(model[2])), 0.0f);

	result[3] = model[3];

	model = result;
}

void Mesh::SetScale(glm::vec3 scale)
{
	{
		glm::vec3 xAxis = glm::normalize(glm::vec3(model[0]));
		glm::vec3 yAxis = glm::normalize(glm::vec3(model[1]));
		glm::vec3 zAxis = glm::normalize(glm::vec3(model[2]));
		glm::vec3 translation = glm::vec3(model[3]);

		xAxis *= scale.x;
		yAxis *= scale.y;
		zAxis *= scale.z;

		model[0] = glm::vec4(xAxis, 0.0f);
		model[1] = glm::vec4(yAxis, 0.0f);
		model[2] = glm::vec4(zAxis, 0.0f);
		model[3] = glm::vec4(translation, 1.0f);
	}
}

void Mesh::Draw(Shader& shader, Material& material, glm::mat4& view, glm::mat4& projection, glm::vec3 color, int skyboxTextureIndex, unsigned int cubemapTexture, float distance) const
{
	if (vertices.empty()) return;

	shader.UseShader();
	shader.SetMat4("model", model);
	shader.SetMat4("view", view);
	shader.SetMat4("projection", projection);
	shader.SetVec3("color", color);
	shader.SetVec3("lightPos", lightPosition);
	shader.SetInt("doubleLighting", doubleLighting);

	glm::vec3 camPos(0.0f, 0.0f, distance);
	shader.SetVec3("cameraPos", camPos);

	glActiveTexture(GL_TEXTURE0 + skyboxTextureIndex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	shader.SetInt("skyboxTexture", skyboxTextureIndex);

	material.UseMaterial(shader);
	glBindVertexArray(VAO);
	if (indices.empty())
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 8);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}

void Mesh::LoadModel()
{
	Assimp::Importer import;

	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Error Assimp: " << import.GetErrorString() << std::endl;
		return;
	}

	ProcessNode(scene->mRootNode, scene);
}

void Mesh::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.push_back(mesh->mVertices[i].x);
		vertices.push_back(mesh->mVertices[i].y);
		vertices.push_back(mesh->mVertices[i].z);

		if (mesh->HasNormals()) 
		{
			vertices.push_back(mesh->mNormals[i].x);
			vertices.push_back(mesh->mNormals[i].y);
			vertices.push_back(mesh->mNormals[i].z);
		}
		else 
		{
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}

		if (mesh->mTextureCoords[0]) 
		{
			vertices.push_back(mesh->mTextureCoords[0][i].x);
			vertices.push_back(mesh->mTextureCoords[0][i].y);
		}
		else 
		{
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
}


	
	
