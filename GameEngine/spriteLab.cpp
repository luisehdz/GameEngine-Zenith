#include "spriteLab.h"
#include <string>

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.H>
#endif

using namespace std;

void SpriteLab::onInit()
{
	LoadParameters parameters;
	parameters.assetType = AssetImage;
	parameters.path = "tiles1.bmp";
	parameters.renderer = renderer;
	int spriteSheetId = assetDatabase.add(parameters);

	parameters.assetType = AssetFont;
	parameters.path = "8_bit_pusab.ttf";
	parameters.size = 18;
	fontId = assetDatabase.add(parameters);

	startTime = SDL_GetTicks();

	for (int i = 0; i < numSprites; ++i)
	{
		sprites[i].posX = getRand(0, getClientWidth());
		sprites[i].posY = getRand(0, getClientHeight());
		sprites[i].scale = getRand(1, 8);
		sprites[i].rotationSpeed = getRand(0, 720);
		sprites[i].xSpeed = getRand(1, 2);
		sprites[i].ySpeed = getRand(1, 2);

		if (getRand() < 0.5f)
		{
			sprites[i].rotationSpeed = -sprites[i].rotationSpeed;
			sprites[i].xSpeed = -sprites[i].xSpeed;
			sprites[i].ySpeed = -sprites[i].ySpeed;
		}

		sprites[i].spriteSheetId = spriteSheetId;
		sprites[i].width = 32;
		sprites[i].height = 32;
		sprites[i].frameIndex = 781;
	}
}

void SpriteLab::onUpdate()
{
	clearScreen(0, 0, 0);

	for (int i = 0; i < numSprites; i++)
	{
		drawSprite(sprites[i]);
		sprites[i].rotation += sprites[i].rotationSpeed * getDeltaTimeSeconds();
		sprites[i].posX += sprites[i].xSpeed;
		sprites[i].posY += sprites[i].ySpeed;

		//Bounds Checking
		if (sprites[i].posX > getClientWidth())
		{
			sprites[i].posX = 0;
		}
		else if (sprites[i].posX < 0)
		{
			sprites[i].posX = getClientWidth();
		}

		if (sprites[i].posY > getClientHeight())
		{
			sprites[i].posY = 0;
		}
		else if (sprites[i].posY < 0)
		{
			sprites[i].posY = getClientHeight();
		}
	}

	drawHudText("Num Sprites: " + to_string(numSprites), 16, 16);
	drawHudText("FPS: " + to_string(1.0 / getDeltaTimeSeconds()), 16, 48);
}

void SpriteLab::drawHudText(string text, int x, int y)
{
	Font* font = assetDatabase.get<Font>(fontId);
	font->draw(text.c_str(), x, y, 255, 255, 255, renderer);
}
