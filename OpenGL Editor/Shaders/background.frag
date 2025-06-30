#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D screenTexture;

void main()
{
	FragColor = vec4(texture2D(screenTexture, TexCoords).rgb, 1.0f);
//	FragColor = vec4(1, 0, 0, 1);
}
