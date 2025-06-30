#include "Application.h"

/* 
TODO:
1. Emission / Bloom
2. Custom Mesh importing
3. Textures 
4. Normal Maps
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