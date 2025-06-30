#include "Application.h"

/* 
TODO:
1. Mouse Rotation
2. Emission / Bloom
3. Custom Mesh importing
4. Textures 
5. Normal Maps
*/

int main(int argc, char* argv[])
{
	Application app;

	app.Init();
	app.Setup();

	while (app.IsRunning())
	{
		app.ProcessInput();
		app.Update();
		app.Render();
	}

	app.Cleanup();

	return 0;
}