#pragma once

#include <GL/glew.h>

#include "stb_image.h"

#include "Shader.h"

class Material
{
public:
	static const int textureIndices[32];

	Material(int diffuseIndex, const char* diffusePath, int normalIndex, const char* normalPath, float roughness);
	void UpdateRoughness(float newRoughness);

	~Material();

	void UseMaterial(Shader& shader);

private:
	int diffuseIndex;
	unsigned int diffuseTexture;

	int normalIndex;
	unsigned int normalMap;

	float roughness;
};