#include "Material.h"

const int Material::textureIndices[32] =
{
	GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2,
	GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5,
	GL_TEXTURE6, GL_TEXTURE7, GL_TEXTURE8,
	GL_TEXTURE9, GL_TEXTURE10, GL_TEXTURE11, 
	GL_TEXTURE12, GL_TEXTURE13, GL_TEXTURE14,
	GL_TEXTURE15, GL_TEXTURE16, GL_TEXTURE17, 
	GL_TEXTURE18, GL_TEXTURE19, GL_TEXTURE20,
	GL_TEXTURE21, GL_TEXTURE22, GL_TEXTURE23, 
	GL_TEXTURE24, GL_TEXTURE25, GL_TEXTURE26,
	GL_TEXTURE27, GL_TEXTURE28, GL_TEXTURE29,
	GL_TEXTURE30, GL_TEXTURE31
};

Material::Material(int diffuseIndex, const char* diffusePath, int normalIndex, const char* normalPath)
{
	stbi_set_flip_vertically_on_load(true);

	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	int width, height, nChannels;
	unsigned char* data = stbi_load(diffusePath, &width, &height, &nChannels, 3);

	if (data == NULL)
	{
		const char* reason = stbi_failure_reason();
		std::cout << "stbi_load failed: " << (reason ? reason : "unknown") << std::endl;
	}

	glGenTextures(1, &diffuseTexture);
	glActiveTexture(textureIndices[diffuseIndex]);
	glBindTexture(GL_TEXTURE_2D, diffuseTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);


	data = stbi_load(normalPath, &width, &height, &nChannels, 3);

	if (data == NULL)
	{
		const char* reason = stbi_failure_reason();
		std::cout << "stbi_load failed: " << (reason ? reason : "unknown") << std::endl;
	}

	glGenTextures(1, &normalMap);
	glActiveTexture(textureIndices[normalIndex]);
	glBindTexture(GL_TEXTURE_2D, normalMap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	this->diffuseIndex = diffuseIndex;
	this->normalIndex = normalIndex;
}

void Material::UpdateRoughness(float newRoughness)
{
	roughness = newRoughness;
}

Material::~Material()
{
	glDeleteTextures(1, &diffuseTexture);
}

void Material::UseMaterial(Shader& shader)
{
	glActiveTexture(textureIndices[diffuseIndex]);
	glBindTexture(GL_TEXTURE_2D, diffuseTexture);
	shader.SetInt("material.diffuse", diffuseIndex);

	glActiveTexture(textureIndices[normalIndex]);
	glBindTexture(GL_TEXTURE_2D, normalMap);
	shader.SetInt("material.normalMap", normalIndex);

	shader.SetFloat("material.roughness", roughness);
}