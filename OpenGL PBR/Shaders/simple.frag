#version 330 core

struct Material
{
	sampler2D diffuse;
};

uniform Material material;

in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 color;

void main()
{
	FragColor = vec4(texture(material.diffuse, TexCoords).rgb * color, 1.0);
}
