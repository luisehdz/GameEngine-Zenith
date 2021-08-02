#include "MazeGame.h"
#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.H>
#endif

int map[] = {   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1,
				1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,
				1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1,
				1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1,
				1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
				1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
				1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
				1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1,
				1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1,
				1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1,
				1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1,
				1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1,
				1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1,
				1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 2, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, };

void MazeGame::onInit()
{
	LoadParameters parameters;
	parameters.assetType = AssetFont;
	parameters.path = "8_bit_pusab.ttf";
	parameters.size = 24;
	fontId = assetDatabase.add(parameters);

	parameters.assetType = AssetImage;
	parameters.path = "tiles1.bmp";
	parameters.renderer = renderer;
	tileAtlasId = assetDatabase.add(parameters);

	parameters.assetType = AssetMusic;
	parameters.path = "victory.ogg";
	victorySfxId = assetDatabase.add(parameters);

	reset();
}

void MazeGame::onUpdate()
{
	//Draws Maze
	Image* image = assetDatabase.get<Image>(tileAtlasId);
	for (int row = 0; row < numTileRows; row++)
	{
		for (int col = 0; col < numTileCols; col++)
		{
			int tileIndex = map[col + row * numTileCols];
			int tileId = -1;

			switch (tileIndex)
			{
			case floorMapIndex:
				tileId = floorTile;
				break;
			case wallMapIndex:
				tileId = wallTile;
				break;
			case exitMapIndex:
				tileId = exitTile;
				break;
			}

			int posX = col * tileSize * (int)worldScale;
			int posY = row * tileSize * (int)worldScale;
			image->drawFrame(posX, posY, tileSize, tileSize, tileId, 0.0f, worldScale, 255, 255, 255, renderer);
		}
	}

	//draws player
	int playerX = playerCol * tileSize * (int)worldScale;
	int playerY = playerRow * tileSize * (int)worldScale;
	image->drawFrame(playerX, playerY, tileSize, tileSize, playerTile, 0.0f, worldScale, 255, 255, 255, renderer);

	if (isExitFound)
	{
		Font* font = assetDatabase.get<Font>(fontId);
		font->draw("You Found the exit!", 16, 16, 255, 255, 255, renderer);
		font->draw("Press ESC to quit", 16, 64, 255, 255, 255, renderer);
		font->draw("Pres SPACE to play again", 16, 96, 255, 255, 255, renderer);
	}
	else
	{
		int mapTile = -1;
		if (mapTile == exitMapIndex)
		{
			isExitFound = true;
			Music* music = assetDatabase.get<Music>(victorySfxId);
			music->play();
		}
	}
}

void MazeGame::onEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_UP:
			tryMove(-1, 0);
			break;
		case SDLK_DOWN:
			tryMove(1, 0);
			break;
		case SDLK_LEFT:
			tryMove(0, -1);
			break;
		case SDLK_RIGHT:
			tryMove(0, 1);
			break;
		case SDLK_SPACE:
			if (isExitFound)
			{
				reset();
			}
			break;
		}
		break;
	}
}

void MazeGame::tryMove(int rowDiff, int colDiff)
{
	if (!isExitFound)
	{
		int destRow = playerRow + rowDiff;
		int destCol = playerCol + colDiff;

		if (isWalkable(destRow, destCol))
		{
			playerRow = destRow;
			playerCol = destCol;
		}
	}	
}

bool MazeGame::isWalkable(int row, int col)
{
	if (row >= 0 && row < numTileRows && col >= 0 && col < numTileCols)
	{
		return map[col + row * numTileCols] != wallMapIndex;
	}

	return false;
}

void MazeGame::reset()
{
	playerRow = playerStartRow;
	playerCol = playerStartCol;
	isExitFound = false;
}