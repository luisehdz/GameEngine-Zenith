#include "CritterGame.h"
#include "Button.h"

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.H>
#endif

CritterGame::CritterGame()
{}

Button userButtons[TotalButtons];

void CritterGame::onInit()
{
	LoadParameters parameters;
	parameters.assetType = AssetFont;
	parameters.path = "8_bit_pusab.ttf";
	parameters.size = 16;
	fontId = assetDatabase.add(parameters);

	parameters.assetType = AssetImage;
	parameters.path = "snake.png";
	parameters.renderer = renderer;
	critterAtlasId = assetDatabase.add(parameters);	
	
	parameters.path = "snakedead.png";
	parameters.renderer = renderer;
	critterAtlasIdTwo = assetDatabase.add(parameters);

	parameters.path = "CritterButtons.png";
	parameters.renderer = renderer;
	buttonAtlasId = assetDatabase.add(parameters);	
	
	parameters.path = "food.png";
	parameters.renderer = renderer;
	foodAtlasId = assetDatabase.add(parameters);

	parameters.assetType = AssetSound;
	parameters.path = "eat.wav";
	eatSfxId = assetDatabase.add(parameters);

	parameters.assetType = AssetSound;
	parameters.path = "play.wav";
	playSfxId = assetDatabase.add(parameters);

	parameters.assetType = AssetSound;
	parameters.path = "talk.mp3";
	talkSfxId = assetDatabase.add(parameters);

	userButtons->setupButtons(userButtons, buttonSize);
}

void CritterGame::onUpdate()
{
	Image* imageCritter = assetDatabase.get<Image>(critterAtlasId);
	Image* imageCritterTwo = assetDatabase.get<Image>(critterAtlasIdTwo);
	Image* imageButton = assetDatabase.get<Image>(buttonAtlasId);
	Image* imageFood = assetDatabase.get<Image>(foodAtlasId);
	Font* font = assetDatabase.get<Font>(fontId);
	Sound* soundEat = assetDatabase.get<Sound>(eatSfxId);
	Sound* soundPlay = assetDatabase.get<Sound>(playSfxId);
	Sound* soundTalk = assetDatabase.get<Sound>(talkSfxId);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	//handles the font rendering of a new game.
	if (newGame)
	{
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
		font->draw("Welcome to Critter Simulator", textUpdatePosX, textUpdatePosY, 255, 255, 255, renderer);
		newGame = false;
	}

	int buttonOffsetY = 0;
	for (int i = 0; i < TotalButtons; i++)
	{
		imageButton->drawFrame(0, buttonOffsetY, 100, buttonSize, i, 0.0f, worldScale, 255, 255, 255, renderer);
		buttonOffsetY += buttonSize;
	}

	//draws player
	int playerXPos = playerX * (int)worldScale;
	int playerYPos = playerY * (int)worldScale;
	imageCritter->drawFrame(playerXPos, playerYPos, playerSizeX, playerSizeY, playerTile, 0.0f, worldScale, 255, 255, 255, renderer);
	imageFood->drawFrame(playerXPos - foodDiff, playerYPos, foodSize, foodSize, foodTile, 0.0f, worldScale, 255, 255, 255, renderer);

	if (selection && !endGame)
	{
		SDL_RenderClear(renderer);
		switch (textOption)
		{
		case 0:
			font->draw("Welcome to Critter Simulator", textUpdatePosX, textUpdatePosY, 255, 255, 255, renderer);
			break;
		case 1:
			Play();
			soundPlay->play();
			font->draw("You play with your critter", textUpdatePosX, textUpdatePosY, 255, 255, 255, renderer);
			break;
		case 2:
			Talk();
			soundTalk->play();
			checkMood(font);
			break;
		case 3:
			Eat();
			soundEat->play();
			font->draw("You feed your critter", textUpdatePosX, textUpdatePosY, 255, 255, 255, renderer);
			break;
		case 4:
			font->draw("Not Implemented", textUpdatePosX, textUpdatePosY, 255, 255, 255, renderer);
			break;
		case 5:
			font->draw("Press ESC to quit", textUpdatePosX, textUpdatePosY + 20, 255, 255, 255, renderer);
			break;
		default:
			font->draw("Unknown option", textUpdatePosX, textUpdatePosY, 255, 255, 255, renderer);
		}
		selection = false;
	}
	else if (endGame)
	{
		SDL_RenderClear(renderer);
		imageCritterTwo->drawFrame(playerXPos, playerYPos, playerSizeX, playerSizeY, playerTile, 0.0f, worldScale, 255, 255, 255, renderer);
		font->draw("Your critter becomes deceased", textUpdatePosX - 50, textUpdatePosY, 255, 255, 255, renderer);
		font->draw("Press ESC to quit", textUpdatePosX - 50, textUpdatePosY + 20, 255, 255, 255, renderer);
		font->draw("Pres SPACE to play again", textUpdatePosX - 50, textUpdatePosY + 40, 255, 255, 255, renderer);
	}
	
	SDL_RenderPresent(renderer);
}

void CritterGame::onEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		for (int i = 0; i < TotalButtons; i++)
		{
			userButtons[i].option = textOption;
			textOption = userButtons[i].handleEvent(&event);
		}
		selection = true;
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_SPACE:
			if (endGame)
			{
				reset();
			}
			break;
		}
		break;
	}
}

void CritterGame::Talk()
{
	PassTime();
}

void CritterGame::Eat()
{
	hunger -= food;
	if (hunger < 0)
	{
		hunger = 0;
	}

	PassTime();
}

void CritterGame::Play()
{
	boredom -= fun;
	if (boredom < 0)
	{
		boredom = 0;
	}

	PassTime();
}

int CritterGame::getHunger() const
{
	return hunger;
}

int CritterGame::getBoredom() const
{
	return boredom;
}

void CritterGame::checkMood(Font* font)
{
	int hungerLevel = getHunger();
	int mood = getBoredom();
	bool priority = false;
	if (mood > hungerLevel)
	{
		priority = true;
	}
	else
	{
		priority = false;
	}

	if (mood > 4 && priority)
	{
		//say want to play
		font->draw("I want to play", textUpdatePosX, textUpdatePosY, 255, 255, 255, renderer);
	}
	else if (hungerLevel > 4)
	{
		//say am honger
		font->draw("I am hungry", textUpdatePosX, textUpdatePosY, 255, 255, 255, renderer);
	}
	else
	{
		//say happy
		font->draw("I am happy", textUpdatePosX, textUpdatePosY, 255, 255, 255, renderer);
	}
}

void CritterGame::PassTime()
{
	hunger += time;
	boredom += time;

	int hungerLevel = getHunger();
	int mood = getBoredom();
	
	if (hungerLevel > 10 || mood > 10)
	{
		endGame = true;
	}
}

void CritterGame::reset()
{
	hunger = 3;
	boredom = 3;
	textOption = 0;
	endGame = false;
	newGame = true;
}