#include "Mesh.h"

#include <GL/glew.h>

#include "DataTransfer.h"

void Mesh::Init()
{
	if (vertices.empty()) return;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::Rotate(float angle, const glm::vec3& axis)
{
	model = glm::rotate(model, glm::radians(angle), axis);
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

void Mesh::Draw(Shader& shader, Material& material, glm::mat4& view, glm::mat4& projection) const
{
	if (vertices.empty()) return;

	shader.UseShader();
	shader.SetMat4("model", model);
	shader.SetMat4("view", view);
	shader.SetMat4("projection", projection);
	material.UseMaterial(shader);
	glBindVertexArray(VAO);
	if (indices.empty())
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 5);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}
	
	
