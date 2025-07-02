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

	const int stride = 14 * sizeof(float);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		//pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(0);

		//normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//uv
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		//tangent
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(3);

		//bitangent
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void*)(11 * sizeof(float)));
		glEnableVertexAttribArray(4);

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

void Mesh::Draw(Shader& shader, Material& material, glm::mat4& view, glm::mat4& projection, glm::vec3 color, int skyboxTextureIndex, unsigned int cubemapTexture, float distance, glm::vec3 emissionColor) const
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

	shader.SetVec3("emissionColor", emissionColor);

	glActiveTexture(GL_TEXTURE0 + skyboxTextureIndex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	shader.SetInt("skyboxTexture", skyboxTextureIndex);

	material.UseMaterial(shader);
	glBindVertexArray(VAO);
	if (indices.empty())
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 14);
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
	// Collect position, normal, texcoord
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		positions.emplace_back(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		if (mesh->HasNormals())
			normals.emplace_back(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		else
			normals.emplace_back(0.0f, 0.0f, 0.0f);

		if (mesh->mTextureCoords[0])
			texCoords.emplace_back(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		else
			texCoords.emplace_back(0.0f, 0.0f);
	}

	// Initialize tangents and bitangents
	std::vector<glm::vec3> tangents(mesh->mNumVertices, glm::vec3(0.0f));
	std::vector<glm::vec3> bitangents(mesh->mNumVertices, glm::vec3(0.0f));

	// Compute tangents and bitangents
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];
		if (face.mNumIndices != 3) continue; // Only triangles

		unsigned int i0 = face.mIndices[0];
		unsigned int i1 = face.mIndices[1];
		unsigned int i2 = face.mIndices[2];

		const glm::vec3& v0 = positions[i0];
		const glm::vec3& v1 = positions[i1];
		const glm::vec3& v2 = positions[i2];

		const glm::vec2& uv0 = texCoords[i0];
		const glm::vec2& uv1 = texCoords[i1];
		const glm::vec2& uv2 = texCoords[i2];

		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		if (r == 0.0f) r = 1.0f; else r = 1.0f / r;

		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

		tangents[i0] += tangent;
		tangents[i1] += tangent;
		tangents[i2] += tangent;

		bitangents[i0] += bitangent;
		bitangents[i1] += bitangent;
		bitangents[i2] += bitangent;
	}

	// Pack final vertex buffer with position, normal, texcoord, tangent, bitangent
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// position
		vertices.push_back(positions[i].x);
		vertices.push_back(positions[i].y);
		vertices.push_back(positions[i].z);

		// normal
		vertices.push_back(normals[i].x);
		vertices.push_back(normals[i].y);
		vertices.push_back(normals[i].z);

		// texcoord
		vertices.push_back(texCoords[i].x);
		vertices.push_back(texCoords[i].y);

		glm::vec3 n = glm::normalize(normals[i]);
		glm::vec3 t = tangents[i];

		// Gram-Schmidt orthogonalize
		t = glm::normalize(t - n * glm::dot(n, t));

		// Calculate handedness (optional, not used in your shaders but good to have)
		glm::vec3 b = glm::cross(n, t);
		float handedness = (glm::dot(b, bitangents[i]) < 0.0f) ? -1.0f : 1.0f;

		// Store
		vertices.push_back(t.x);
		vertices.push_back(t.y);
		vertices.push_back(t.z);

		vertices.push_back(b.x);
		vertices.push_back(b.y);
		vertices.push_back(b.z);
	}

	// Indices (unchanged)
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
}




	
	
