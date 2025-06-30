#version 330 core

struct Material
{
	sampler2D diffuse;
	float roughness;
};

uniform Material material;

in vec2 TexCoords;
in vec3 Normals;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 color;
uniform vec3 lightPos;
uniform int doubleLighting;

uniform samplerCube skyboxTexture;
uniform vec3 cameraPos;

void main()
{
	//DIFFUSE
	vec3 norm = normalize(Normals);
	if (!gl_FrontFacing && (doubleLighting == 1))
	{
		norm = -norm;
	}
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0) + 0.2;
	vec4 diffuse = vec4(texture(material.diffuse, TexCoords).rgb * color * diff, 1.0);

	//ROUGHNESS
	vec3 I = normalize(FragPos - cameraPos);
	vec3 R = reflect(I, normalize(Normals));
	vec4 reflection = vec4(texture(skyboxTexture, R).rgb, 1.0);

	FragColor = mix(diffuse, reflection, material.roughness);
}
