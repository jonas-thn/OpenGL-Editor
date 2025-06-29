#version 330 core

struct Material
{
	sampler2D diffuse;
};

uniform Material material;

in vec2 TexCoords;
in vec3 Normals;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 color;
uniform vec3 lightPos;
uniform int doubleLighting;

void main()
{
	vec3 norm = normalize(Normals);
	if (!gl_FrontFacing && (doubleLighting == 1))
	{
		norm = -norm;
	}
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0) + 0.3;

	FragColor = vec4(texture(material.diffuse, TexCoords).rgb * color * diff, 1.0);
//	FragColor = vec4(color * diff, 1.0);
}
