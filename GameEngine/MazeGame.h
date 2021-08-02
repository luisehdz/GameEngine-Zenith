#pragma once
#include "game.h"

class MazeGame : public Game
{
protected:
	void onInit() override;
	void onUpdate() override;
	void onEvent(SDL_Event& event) override;

private:
	int fontId = -1;
	int tileAtlasId = -1;
	int victorySfxId = -1;

	//Static Variables
	const int numTileRows = 16;
	const int numTileCols = 16;
	const int tileSize = 32;
	const float worldScale = 1.0f;

	const int playerStartRow = 1;
	const int playerStartCol = 1;
	const int playerTile = 781;
	const int floorTile = 97;
	const int wallTile = 312;
	const int exitTile = 308;

	static const int floorMapIndex = 0;
	static const int wallMapIndex = 1;
	static const int exitMapIndex = 2;

	//dynamic variables
	int playerRow = -1;
	int playerCol = -1;
	bool isExitFound = false;

	void tryMove(int rowDiff, int colDiff);
	bool isWalkable(int row, int col);
	void reset();
};