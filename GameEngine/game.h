#pragma once
#include "assetDatabase.h"
#include "sprite.h"

struct lua_State;
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
union SDL_Event;

class Game
{
public:
	bool init(int screenWidth, int screenHeight, bool fullscreen, bool vsync);
	void run();
	void shutdown();

	virtual void onEvent(SDL_Event& event);

	void clearScreen(uint8_t r, uint8_t g, uint8_t b);
	void drawSprite(const Sprite& sprite);
	void setWindowTitle(const char* title);
	
	int getClientWidth() const;
	int getClientHeight() const;
	int getRand(int min, int max);
	float getRand();
	double getDeltaTimeSeconds() const;	

	AssetDatabase assetDatabase;
	SDL_Renderer* renderer = nullptr;

private:
	bool pollEvents();
	void beginFrame();
	void endFrame();
	void initLua();

	lua_State* luaState;
	SDL_Window* window = nullptr;
	SDL_Texture* backbuffer = nullptr;

	int clientWidth;
	int clientHeight;

	uint64_t currentTime = 0;
	uint64_t previousTime = 0;
	double deltaTimeSeconds = 0;
};