#include "Application.h"

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