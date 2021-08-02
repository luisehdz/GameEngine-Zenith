#include "game.h"

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

int main(int argc, char* argv[])
{
	Game game;
	game.init(480, 600, false, false);
	game.run();
	game.shutdown();

	return 0;
}