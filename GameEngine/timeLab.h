#pragma once
#include "Game.h"
#include <string>

class TimeLab : public Game
{
public:
	void onInit() override;
	void onUpdate() override;

private:
	void drawHudText(std::string text, int x, int y);

	int fontId = -1;
	uint32_t startTime = 0;
};

