#pragma once

inline float triangleVertices[] = 
{
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

inline float quadVertices[] = 
{
	0.5f, 0.5f, 0.0f, 
	0.5f, -0.5f, 0.0f, 
	-0.5f, -0.5f, 0.0f, 
	-0.5f, 0.5f, 0.0f
};
inline unsigned int quadIndices[] = 
{ 
	0, 1, 3, 
	1, 2, 3 
};

//float ertices[] = {
//	// positions // colors // texture coords
//	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
//	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
//	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
//	-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
//};