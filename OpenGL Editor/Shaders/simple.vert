#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;
out vec3 Normals;
out vec3 FragPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	Normals = normalize(transpose(inverse(mat3(model))) * aNormals);
	TexCoords = aTexCoords;
	FragPos = vec3(model * vec4(aPos, 1.0));
}