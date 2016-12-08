#include "stdafx.h"
#include "WindowClient.h"
#include <SDL2/SDL.h>

namespace
{
const glm::ivec2 WINDOW_SIZE = {1600, 900};
const char WINDOW_TITLE[] = "Stay Alive";
const char ERROR_TITLE[] = "Fatal Error";
}

int main(int, char *[])
{
	try
	{
		CWindow window(ContextProfile::RobustOpenGL_3_2, ContextMode::Debug);
		window.Show(WINDOW_TITLE, WINDOW_SIZE);
		CWindowClient client(window);
		window.DoMainLoop();
	}
	catch (const std::exception &ex)
	{
		const char *title = "Fatal Error";
		const char *message = ex.what();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, nullptr);
	}

    return 0;
}
