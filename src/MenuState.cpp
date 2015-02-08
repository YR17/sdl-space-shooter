/*******************************************************************//*
 * Implementation of the MenuState class.
 *
 * @author      Brandon To
 * @version     1.0
 * @since       2014-08-05
 * @modified    2015-02-08
 *********************************************************************/
#include "MenuState.h"

#include <time.h>
#include <SDL2/SDL.h>
#include "ApplicationStateManager.h"
#include "GameEntity.h"
#include "RenderComponent.h"
#include "WindowElements.h"

MenuState::MenuState(ApplicationStateManager* applicationStateManager,
                     WindowElements* windowElements)
:   fadeIn(false),
    fadeOut(false),
    menuAlpha(0),
    randomMeteorTime(0),
    nextState(0),
    blackScreen(windowElements),
    gameEntityManager(windowElements,this)
{
    this->applicationStateManager = applicationStateManager;
    this->windowElements = windowElements;
    this->xmlPath = "data/xml/states/MenuState.xml"; //Default xml path
    srand(time(NULL));
}

MenuState::~MenuState()
{
}

void MenuState::onEnter()
{
    blackScreen.startBlackIn();
    //background = gameEntityManager.getFactory()->createBackground();
    background = gameEntityManager.getFactory()->createBackgroundEntities();
    //mainMenu = gameEntityManager.getFactory()->createMainMenu(this);
    mainMenu = gameEntityManager.getFactory()->createUIEntities();

    // Initializes alpha value of ui to 0
    for (int i=0; i<mainMenu.size(); i++)
    {
        mainMenu[i]->getRenderComponent()->getTexture()->setAlphaBlend(menuAlpha);
    }

    meteorTimer.start();
}

void MenuState::onEvent()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            applicationStateManager->setNextState(STATE_EXIT);
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.scancode)
            {
                case SDL_SCANCODE_ESCAPE:
                    stateTransition(STATE_EXIT);
                    break;

                case SDL_SCANCODE_RETURN:
                    stateTransition(STATE_GAME);
                    break;
            }
        }
        gameEntityManager.onEvent(&event);
    }
}

void MenuState::onUpdate()
{
    if (fadeIn)
    {
        if (menuAlpha<200)
        {
            menuAlpha+=10;
            if (menuAlpha==200) { fadeIn = false; }
        }
        for (int i=0; i<mainMenu.size(); i++)
        {
            mainMenu[i]->getRenderComponent()->getTexture()->setAlphaBlend(menuAlpha);
        }
    }
    else if (fadeOut)
    {
        if (menuAlpha>0)
        {
            menuAlpha-=10;
            if (menuAlpha==0) { fadeOut = false; blackScreen.startBlackOut(); }
        }
        for (int i=0; i<mainMenu.size(); i++)
        {
            mainMenu[i]->getRenderComponent()->getTexture()->setAlphaBlend(menuAlpha);
        }
    }
    else if (blackScreen.isBlackingIn())
    {
        blackScreen.onUpdate();
        if (!blackScreen.isBlackingIn())
        {
            fadeIn = true;
        }
    }
    else if (blackScreen.isBlackingOut())
    {
        blackScreen.onUpdate();
        if (!blackScreen.isBlackingOut())
        {
            applicationStateManager->setNextState(nextState);
        }
    }

    if (meteorTimer.getTimeOnTimer() > randomMeteorTime)
    {
        GameEntity *meteor = gameEntityManager.getFactory()->createMeteor();
        meteor->position.x = 1.1*windowElements->WINDOW_WIDTH;
        meteor->position.y = rand()%(int)(0.8*windowElements->WINDOW_HEIGHT);
        meteors.push_back(meteor);
        randomMeteorTime = rand()%5000+(rand()%10000+5000);
        meteorTimer.stop();
        meteorTimer.start();
    }

    gameEntityManager.onUpdate();
}

void MenuState::onRender()
{
    //SDL_RenderClear(windowElements->renderer);
    gameEntityManager.onRender();
    blackScreen.onRender();
    //SDL_RenderPresent(windowElements->renderer);
}

void MenuState::onExit()
{

}

void MenuState::stateTransition(int nextState)
{
    this->nextState = nextState;
    fadeOut = true;
}
