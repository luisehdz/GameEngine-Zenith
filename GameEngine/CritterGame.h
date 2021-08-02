#pragma once
#include "game.h"

class CritterGame : public Game
{
public:
	CritterGame();
	void Talk();
	void Eat();
	void Play();

protected:
	void onInit() override;
	void onUpdate() override;
	void onEvent(SDL_Event& event) override;

private:
	int getHunger() const;
	int getBoredom() const;
	void checkMood(Font* font);
	void PassTime();
	void reset();

	int fontId = -1;
	int critterAtlasId = -1;
	int critterAtlasIdTwo = -1;
	int buttonAtlasId = -1;
	int foodAtlasId = -1;
	int eatSfxId = -1;
	int playSfxId = -1;
	int talkSfxId = -1;

	//Static Variables
	const int numTileRows = 16;
	const int numTileCols = 16;
	const int playerSizeX = 250;
	const int playerSizeY = 313;
	const int buttonSize = 106;
	const int foodSize = 16;
	const int playerTile = 0;
	const int foodTile = 12;

	const int playerX = 175;
	const int playerY = 50;
	const int foodDiff = 30;
	const int textUpdatePosX = 120;
	const int textUpdatePosY = 400;
	const float worldScale = 1.0f;

	const int eatSfx = 0;
	const int playSfx = 0;
	const int talkSfx = 0;

	//Critter variables
	const int time = 1;
	const int food = 1;
	const int fun = 1;

	int hunger = 4;
	int boredom = 4;
	int textOption = 0;

	bool selection = true;
	bool newGame = false;
	bool endGame = false;
	
};

