#pragma once

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.H>
#endif

union SDL_Event;

enum ButtonSpriteType
{
    PlayButton = 0,
    TalkButton = 1,
    EatButton = 2,
    FightButton = 3,
    QuitButton = 4,
    TotalButtons = 5
};

class Button
{
public:
    //Initializes internal variables
    Button();

    //Sets top left position
    void setPosition(int x, int y);

    //Handles mouse event
    int handleEvent(SDL_Event* e);

    void setButtonType(ButtonSpriteType type);
    void setupButtons(Button buttons[], int buttonSize);

    int option = 0;

    ButtonSpriteType currentButtonSprite;

private:
    //Static Variables
    const int buttonWidth = 106;
    const int buttonHeight = 106;
    const int totalButtons = 5;    

    SDL_Point mousePosition;
};

