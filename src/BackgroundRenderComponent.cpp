/*******************************************************************//*
 * Render component for background
 *
 * @author      Brandon To
 * @version     1.0
 * @since       2014-08-17
 * @modified    2014-09-06
 *********************************************************************/
#include "BackgroundRenderComponent.h"

#include <cstddef>
#include <SDL2/SDL.h>
#include "SDL_util.h"
#include "WindowElements.h"

BackgroundRenderComponent::BackgroundRenderComponent(WindowElements* windowElements)
:   sprite(NULL),
    windowElements(windowElements),
    offset(0),
    offsetIncrementCounter(0),
    offsetIncrementModulus(3)
{
    sprite=SDL_util::create_background_texture(windowElements, "bin/graphics/background/darkPurple.png");
}

void BackgroundRenderComponent::update(GameEntity* gameEntity)
{
    offsetIncrementCounter++;
    if (offsetIncrementCounter % offsetIncrementModulus == 0)
    {
        offset++;
    }
    SDL_Rect temp;
    SDL_QueryTexture(sprite, NULL, NULL, &(temp.w), &(temp.h));
	if( offset > temp.h )
	{
		offset = 0;
	}
    temp.x = 0;
    temp.y = offset;
    SDL_RenderCopyEx(windowElements->renderer, sprite, NULL, &temp, 0.0, NULL, SDL_FLIP_NONE);
    temp.y = offset - temp.h;
    SDL_RenderCopyEx(windowElements->renderer, sprite, NULL, &temp, 0.0, NULL, SDL_FLIP_NONE);
}

BackgroundRenderComponent::~BackgroundRenderComponent()
{
     SDL_DestroyTexture(sprite);
     sprite=NULL;
}
