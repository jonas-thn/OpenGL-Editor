#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D screenTexture;
uniform vec3 emissionColor;
uniform int bloomRadius;

void main()
{
	int counter = 0;

	vec2 tex_offset = 1.0 / textureSize(screenTexture, 0) * 5; 
	vec3 original = texture(screenTexture, TexCoords).rgb;
	vec3 result = texture(screenTexture, TexCoords).rgb; 
	for(int y = -bloomRadius; y < bloomRadius; ++y)
	{
		for(int x = -bloomRadius; x < bloomRadius; ++x)
		{
			if(vec2(x,y).length() > bloomRadius) continue;
			result += pow(texture(screenTexture, TexCoords + vec2(tex_offset.x * x, tex_offset.y * y)).rgb, vec3(8));
			counter++;
		}
		
	}
	counter = counter > 0 ? counter : 1;
	result /= counter;

	FragColor = vec4(original + result * emissionColor, 1.0);
}
