#pragma once

inline float triangleVertices[] = 
{
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

inline float quadVertices[] = 
{
	0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
};
inline unsigned int quadIndices[] = 
{ 
	0, 1, 3, 
	1, 2, 3 
};