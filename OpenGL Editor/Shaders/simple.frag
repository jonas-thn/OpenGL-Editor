#version 330 core

struct Material
{
	sampler2D normalMap;
	sampler2D diffuse;
	float roughness;
};

uniform Material material;

in vec2 TexCoords;
in vec3 Normals;
in vec3 FragPos;
in mat3 TBN;

out vec4 FragColor;

uniform vec3 color;
uniform vec3 lightPos;
uniform int doubleLighting;

uniform samplerCube skyboxTexture;
uniform vec3 cameraPos;

uniform vec3 emissionColor;

void main()
{
//	//DIFFUSE
//	vec3 norm = normalize(Normals);
//	if (!gl_FrontFacing && (doubleLighting == 1))
//	{
//		norm = -norm;
//	}
//	vec3 lightDir = normalize(lightPos - FragPos);
//	float diff = max(dot(norm, lightDir), 0.0) + 0.2;
//	vec4 diffuse = vec4(texture(material.diffuse, TexCoords).rgb * color * diff, 1.0);
//
//	//ROUGHNESS
//	vec3 I = normalize(FragPos - cameraPos);
//	vec3 R = reflect(I, normalize(Normals));
//	vec4 reflection = vec4(texture(skyboxTexture, R).rgb, 1.0);
//
//	vec4 mixedColor = mix(diffuse, reflection, material.roughness);
//	FragColor = vec4(mixedColor + vec4(emissionColor * 2, 1.0f));


	// Sample normal from normal map
    vec3 normalMapSample = texture(material.normalMap, TexCoords).rgb;
    normalMapSample = normalize(normalMapSample * 2.0 - 1.0); // From [0,1] to [-1,1]

    // Transform to world space
    vec3 norm = normalize(TBN * normalMapSample);

    // Flip normal for backfaces if doubleLighting
    if (!gl_FrontFacing && (doubleLighting == 1))
    {
        norm = -norm;
    }

    // Lighting calculation
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0) + 0.2;
    vec4 diffuseColor = vec4(texture(material.diffuse, TexCoords).rgb * color * diff, 1.0);

    // Reflection
    vec3 I = normalize(FragPos - cameraPos);
    vec3 R = reflect(I, norm);
    vec4 reflection = vec4(texture(skyboxTexture, R).rgb, 1.0);

    vec4 mixedColor = mix(diffuseColor, reflection, material.roughness);

    // Emission
    FragColor = mixedColor + vec4(emissionColor * 2.0, 1.0);
}
