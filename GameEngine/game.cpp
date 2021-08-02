#include "game.h"
#include <lua.hpp>
#include <cstdlib>
#include <ctime>
using namespace std;

#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#endif

static Game* pGame = nullptr;

// buffers to hold key input
const int NUM_SDL_SCANCODES = 512;
bool prevKeys[NUM_SDL_SCANCODES];
bool keys[NUM_SDL_SCANCODES];

void printError(lua_State* state)
{
	SDL_Log(luaL_checkstring(state, -1));
}

void runScript(lua_State* state, const char* file)
{
	int result = luaL_loadfile(state, file);
	if (result == LUA_OK)
	{
		result = lua_pcall(state, 0, LUA_MULTRET, 0);

		if (result != LUA_OK)
		{
			printError(state);
		}
	}
	else
	{
		printError(state);
	}
}

void call(lua_State* state, const char* functionName)
{
	int type = lua_getglobal(state, functionName);
	if (type == LUA_TNIL)
	{
		SDL_Log("No function found with name");
	}
	else
	{
		lua_pcall(state, 0, 0, 0);
	}
}

bool Game::init(int screenWidth, int screenHeight, bool fullscreen, bool vsync)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		return false;
	}

	Uint32 flags = SDL_WINDOW_SHOWN;
	if (fullscreen)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}

	window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, flags);

	if (!window)
	{
		return false;
	}

	if (vsync)
	{
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	}
	else
	{
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	}
	
	if (!renderer)
	{
		return false;
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	int imageFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imageFlags) & imageFlags))
	{
		return false;
	}

	if (Mix_OpenAudio(20050, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		return false;
	}

	if (TTF_Init() == -1)
	{
		return false;
	}

	backbuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);

	if (!backbuffer)
	{
		return false;
	}

	unsigned int seed = (unsigned int)time(0);
	srand(seed);

	clientWidth = screenWidth;
	clientHeight = screenHeight;

	previousTime = SDL_GetPerformanceFrequency();

	initLua();
	call(luaState, "Start");

	return true;
}

void Game::run()
{
	bool quit = false;

	while (!quit)
	{
		quit = pollEvents();
		call(luaState, "Update");

		SDL_memcpy(&prevKeys, &keys, NUM_SDL_SCANCODES);

		beginFrame();
		call(luaState, "Draw");
		endFrame();
	}
}

void Game::shutdown()
{
	lua_close(luaState);
	assetDatabase.clear();

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer); // free backbuffer
	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::onEvent(SDL_Event& event)
{
	//nothing by default
}

void Game::beginFrame()
{
	currentTime = SDL_GetPerformanceCounter();
	deltaTimeSeconds = (double)(currentTime - previousTime) * 1000.0 / SDL_GetPerformanceFrequency() / 1000;
	SDL_SetRenderTarget(renderer, backbuffer);
}

void Game::endFrame()
{
	SDL_SetRenderTarget(renderer, nullptr);
	SDL_RenderCopyEx(renderer, backbuffer, nullptr, nullptr, 0, nullptr, SDL_FLIP_NONE);
	SDL_RenderPresent(renderer);

	previousTime = currentTime;
}

bool Game::pollEvents()
{
	bool quit = false;
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			keys[event.key.keysym.scancode] = true;

			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
			break;
		case SDL_KEYUP:
			keys[event.key.keysym.scancode] = false;
			break;
		case SDL_QUIT:
			quit = true;
			break;
		}

		onEvent(event);
	}

	return quit;
}

void Game::setWindowTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void Game::clearScreen(uint8_t r, uint8_t g, uint8_t b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, 0);
	SDL_RenderClear(renderer);
}

void Game::drawSprite(const Sprite& sprite)
{
	Image* spriteSheet = assetDatabase.get<Image>(sprite.spriteSheetId);
	
	int spritePosX = (int)(sprite.posX - (sprite.width / 2) * sprite.scale);
	int spritePosY = (int)(sprite.posY - (sprite.height / 2) * sprite.scale);

	spriteSheet->drawFrame(spritePosX, spritePosY, sprite.width, sprite.height, sprite.frameIndex, sprite.rotation, sprite.scale, 255, 255, 255, renderer);
}

double Game::getDeltaTimeSeconds() const
{
	return deltaTimeSeconds;
}

float Game::getRand()
{
	return (float)rand() / RAND_MAX;
}

int Game::getRand(int min, int max)
{
	return (int)(min + (max - min + 1) * getRand());
}

int Game::getClientHeight() const
{
	return clientHeight;
}

int Game::getClientWidth() const
{
	return clientWidth;
}

extern "C"
{
	int LoadImage(lua_State* state)
	{
		LoadParameters loadParameters;
		loadParameters.path = lua_tostring(state, 1);
		loadParameters.assetType = AssetImage;
		loadParameters.renderer = pGame->renderer;

		int imageId = pGame->assetDatabase.add(loadParameters);
		lua_pushinteger(state, imageId);
		return 1;
	}

	int LoadSound(lua_State* state)
	{
		LoadParameters loadParameters;
		loadParameters.path = lua_tostring(state, 1);
		loadParameters.assetType = AssetSound;
		loadParameters.renderer = pGame->renderer;

		int soundId = pGame->assetDatabase.add(loadParameters);
		lua_pushinteger(state, soundId);
		return 1;
	}

	int LoadMusic(lua_State* state)
	{
		LoadParameters loadParameters;
		loadParameters.path = lua_tostring(state, 1);
		loadParameters.assetType = AssetMusic;
		loadParameters.renderer = pGame->renderer;

		int musicId = pGame->assetDatabase.add(loadParameters);
		lua_pushinteger(state, musicId);
		return 1;
	}

	int LoadFont(lua_State* state)
	{
		LoadParameters loadParameters;
		loadParameters.path = lua_tostring(state, 1);
		loadParameters.size = (int)lua_tointeger(state, 2);
		loadParameters.assetType = AssetFont;

		int fontId = pGame->assetDatabase.add(loadParameters);
		lua_pushnumber(state, fontId);
		return 1;
	}

	int DrawImage(lua_State* state)
	{
		int imageId = (int)lua_tointeger(state, 1);
		int frameId = (int)lua_tointeger(state, 2);
		int posX = (int)lua_tonumber(state, 3);
		int posY = (int)lua_tonumber(state, 4);
		int frameSize = (int)lua_tointeger(state, 5);

		Image* image = pGame->assetDatabase.get<Image>(imageId);
		image->drawFrame(posX, posY, frameSize, frameSize, frameId, 0.0f, 1.0f, 255, 255, 255, pGame->renderer);
		return 0;
	}

	int DrawImageUneven(lua_State* state)
	{
		int imageId = (int)lua_tointeger(state, 1);
		int frameId = (int)lua_tointeger(state, 2);
		int posX = (int)lua_tonumber(state, 3);
		int posY = (int)lua_tonumber(state, 4);
		int frameSizeX = (int)lua_tointeger(state, 5);
		int frameSizeY = (int)lua_tointeger(state, 6);

		Image* image = pGame->assetDatabase.get<Image>(imageId);
		image->drawFrame(posX, posY, frameSizeX, frameSizeY, frameId, 0.0f, 1.0f, 255, 255, 255, pGame->renderer);
		return 0;
	}

	int DrawImageRotated(lua_State* state)
	{
		int imageId = (int)lua_tointeger(state, 1);
		int frameId = (int)lua_tointeger(state, 2);
		int posX = (int)lua_tonumber(state, 3);
		int posY = (int)lua_tonumber(state, 4);
		int frameSize = (int)lua_tointeger(state, 5);
		double rotation = (double)lua_tonumber(state, 6);

		Image* image = pGame->assetDatabase.get<Image>(imageId);
		image->drawFrame(posX, posY, frameSize, frameSize, frameId, rotation, 1.0f, 255, 255, 255, pGame->renderer);
		return 0;
	}

	int PlaySound(lua_State* state)
	{
		int soundId = (int)lua_tointeger(state, 1);
		Sound* sound = pGame->assetDatabase.get<Sound>(soundId);
		sound->play();

		return 0;
	}

	int PlayMusic(lua_State* state)
	{
		int musicId = (int)lua_tointeger(state, 1);
		Music* music = pGame->assetDatabase.get<Music>(musicId);
		music->play();

		return 0;
	}

	int DrawText(lua_State* state)
	{
		string text = lua_tostring(state, 1);
		int posX = (int)lua_tonumber(state, 2);
		int posY = (int)lua_tonumber(state, 3);
		int fontId = (int)lua_tonumber(state, 4);
		uint8_t r = (uint8_t)lua_tointeger(state, 5);
		uint8_t g = (uint8_t)lua_tointeger(state, 6);
		uint8_t b = (uint8_t)lua_tointeger(state, 7);

		Font* font = pGame->assetDatabase.get<Font>(fontId);
		font->draw(text.c_str(), posX, posY, r, g, b, pGame->renderer);
		return 0;
	}

	int DrawTextWhite(lua_State* state)
	{
		string text = lua_tostring(state, 1);
		int posX = (int)lua_tonumber(state, 2);
		int posY = (int)lua_tonumber(state, 3);
		int fontId = (int)lua_tonumber(state, 4);

		Font* font = pGame->assetDatabase.get<Font>(fontId);
		font->draw(text.c_str(), posX, posY, 255, 255, 255, pGame->renderer);
		return 0;
	}

	int ClearScreen(lua_State* state)
	{
		uint8_t r = (uint8_t)lua_tointeger(state, 1);
		uint8_t g = (uint8_t)lua_tointeger(state, 2);
		uint8_t b = (uint8_t)lua_tointeger(state, 3);
		
		pGame->clearScreen(r, g, b);

		return 0;
	}

	int GetBaseAssetDirectory(lua_State* state)
	{
		string directory = pGame->assetDatabase.getBasePath();
		lua_pushstring(state, directory.c_str());

		return 1;
	}

	int GetDeltaTime(lua_State* state)
	{
		lua_pushnumber(state, pGame->getDeltaTimeSeconds());
		return 1;
	}

	int IsKeyDown(lua_State* state)
	{
		int scancode = (int)lua_tointeger(state, 1);
		lua_pushboolean(state, keys[scancode]);
		return 1;
	}

	int IsKeyPressed(lua_State* state)
	{
		int scancode = (int)lua_tointeger(state, 1);
		bool is = keys[scancode];
		bool was = prevKeys[scancode];

		lua_pushboolean(state, is && !was);
		return 1;
	}

	int IsKeyReleased(lua_State* state)
	{
		int scancode = (int)lua_tointeger(state, 1);
		bool is = keys[scancode];
		bool was = prevKeys[scancode];

		lua_pushboolean(state, !is && was);
		return 1;
	}

	int SetWindowTitle(lua_State* state)
	{
		string title = lua_tostring(state, 1);
		pGame->setWindowTitle(title.c_str());
		return 0;
	}

	int GetClientWidth(lua_State* state)
	{
		int width = (int)lua_tointeger(state, 1);
		lua_pushinteger(state, pGame->getClientWidth());
		return 1;
	}

	int GetClientHeight(lua_State* state)
	{
		int height = (int)lua_tointeger(state, 1);
		lua_pushinteger(state, pGame->getClientHeight());
		return 1;
	}
}

void Game::initLua()
{
	pGame = this;
	luaState = luaL_newstate();
	luaL_openlibs(luaState);

	//register lua glue functions
	lua_register(luaState, "LoadImage", LoadImage);
	lua_register(luaState, "LoadSound", LoadSound);
	lua_register(luaState, "LoadMusic", LoadMusic);
	lua_register(luaState, "LoadFont", LoadFont);
	lua_register(luaState, "DrawImage", DrawImage);
	lua_register(luaState, "DrawImageUneven", DrawImageUneven);
	lua_register(luaState, "DrawImageRotated", DrawImageRotated);
	lua_register(luaState, "PlaySound", PlaySound);
	lua_register(luaState, "PlayMusic", PlayMusic);
	lua_register(luaState, "DrawText", DrawText);
	lua_register(luaState, "DrawTextWhite", DrawTextWhite);
	lua_register(luaState, "ClearScreen", ClearScreen);
	lua_register(luaState, "GetBaseAssetDirectory", GetBaseAssetDirectory);
	lua_register(luaState, "GetDeltaTime", GetDeltaTime);
	lua_register(luaState, "IsKeyDown", IsKeyDown);
	lua_register(luaState, "IsKeyPressed", IsKeyPressed);
	lua_register(luaState, "IsKeyReleased", IsKeyReleased);
	lua_register(luaState, "SetWindowTitle", SetWindowTitle);
	lua_register(luaState, "GetClientHeight", GetClientHeight);
	lua_register(luaState, "GetClientWidth", GetClientWidth);

	string pathToConfig = assetDatabase.getBasePath() + "config.lua";
	runScript(luaState, pathToConfig.c_str());
}