#include "Application.h"

/* 
TODO:
1. Materials:
2. Normal Maps
3. Bloom Map
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