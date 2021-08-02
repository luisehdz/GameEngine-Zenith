#pragma once

class Sprite
{
public:
	double posX = 0;
	double posY = 0;
	int width = 0;
	int height = 0;
	int spriteSheetId;
	int frameIndex;

	double rotation = 0.0;
	double rotationSpeed = 0.0;
	double xSpeed = 0;
	double ySpeed = 0;
	double scale = 1.0;
};