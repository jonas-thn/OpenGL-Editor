#include "Screen.h"

Screen::Screen(std::vector<float> vertices, int textureIndex, unsigned int textureID)
{
	this->vertices = vertices;
	this->textureIndex = textureIndex;
	this->textureID = textureID;
}

Screen::Screen(std::vector<float> vertices, int textureIndex, const char* path)
{
	stbi_set_flip_vertically_on_load(true);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);

	if (data == NULL)
	{
		std::cout << "Failed to load texture." << std::endl;
	}

	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(GL_TEXTURE_2D, textureID);

	if (nChannels == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else if (nChannels == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "nChannels Image Error." << std::endl;
	}
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	this->vertices = vertices;
	this->textureIndex = textureIndex;
}

void Screen::Init()
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

	glBindVertexArray(0);
}

void Screen::Draw(Shader& shader, glm::vec3 emissionColor, float emissionRadius) const
{
	if (vertices.empty()) return;

	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);

	shader.UseShader();
	
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(GL_TEXTURE_2D, textureID);
	shader.SetInt("screenTexture", textureIndex);
	shader.SetVec3("emissionColor", emissionColor);
	shader.SetInt("bloomRadius", (int)emissionRadius);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 5);
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}
