#include "Button.h"

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.H>
#endif

Button::Button()
{
    mousePosition.x = 0;
    mousePosition.y = 0;

    currentButtonSprite = PlayButton;
}

void Button::setPosition(int x, int y)
{
    mousePosition.x = x;
    mousePosition.y = y;
}

void Button::setButtonType(ButtonSpriteType type)
{
    currentButtonSprite = type;
}

void Button::setupButtons(Button buttons[] , int buttonOffsetY)
{
    int buttonSize = 0;

    buttons[0].setButtonType(PlayButton);
    buttons[1].setButtonType(EatButton);
    buttons[2].setButtonType(TalkButton);
    buttons[3].setButtonType(FightButton);
    buttons[4].setButtonType(QuitButton);

    for (int i = 0; i < totalButtons; i++)
    {
        buttons[i].setPosition(0, buttonSize);
        buttonSize += buttonOffsetY;
    }
}

int Button::handleEvent(SDL_Event* e)
{
    //If mouse event happened
    if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState(&x, &y);

        //Check if mouse is in button
        bool inside = true;

        //Mouse is left of the button
        if (x < mousePosition.x)
        {
            inside = false;
        }
        //Mouse is right of the button
        else if (x > mousePosition.x + buttonWidth)
        {
            inside = false;
        }
        //Mouse above the button
        else if (y < mousePosition.y)
        {
            inside = false;
        }
        //Mouse below the button
        else if (y > mousePosition.y + buttonHeight)
        {
            inside = false;
        }
        //Mouse is outside button
        if (inside)
        {
            int z = 0;
            int b = 0;
            switch(currentButtonSprite)
            {
            case PlayButton:
                option = 1;
                break;
            case EatButton:
                option = 2;
                break;
            case TalkButton:
                option = 3;
                break;
            case FightButton:
                option = 4;
                break;
            case QuitButton:
                option = 5;
                break;
            }
        }

        return option;
    }
}