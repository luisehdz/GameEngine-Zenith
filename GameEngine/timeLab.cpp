#include "timeLab.h"
#include <string>
using namespace std;

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.H>
#endif

void TimeLab::onInit()
{
	LoadParameters parameters;
	parameters.assetType = AssetFont;
	parameters.path = "8_bit_pusab.ttf";
	parameters.size = 18;
	fontId = assetDatabase.add(parameters);

	startTime = SDL_GetTicks();
}

void TimeLab::onUpdate()
{
	clearScreen(0, 0, 0);

	drawHudText("Delta Time: " + to_string(getDeltaTimeSeconds()), 16, 16);
	drawHudText("FPS: " + to_string(1.0 / getDeltaTimeSeconds()), 16, 48);
}

void TimeLab::drawHudText(string text, int x, int y)
{
	Font* font = assetDatabase.get<Font>(fontId);
	font->draw(text.c_str(), x, y, 255, 255, 255, renderer);
}