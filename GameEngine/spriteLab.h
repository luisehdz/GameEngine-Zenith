#pragma once
#include "Game.h"
#include <string>

class SpriteLab : public Game
{
public:
	void onInit() override;
	void onUpdate() override;

private:
	void drawHudText(std::string text, int x, int y);

	int fontId = -1;
	uint32_t startTime = 0;

	static const int numSprites = 2400;
	Sprite sprites[numSprites];
};