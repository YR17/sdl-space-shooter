/*******************************************************************//*
 * Implementation of the UIPanelInputComponent class.
 *
 * @author      Brandon To
 * @version     1.0
 * @since       2014-09-03
 * @modified    2014-09-06
 *********************************************************************/
#include "UIPanelInputComponent.h"

#include "GameEntity.h"
#include "UIClickFunction.h"
#include "UIPanelRenderComponent.h"

UIPanelInputComponent::UIPanelInputComponent(GameEntity* gameEntity)
:   gameEntity(gameEntity),
    clickFunction(NULL),
    render(NULL),
    buttonPressedDown(false)
{
    render = dynamic_cast<UIPanelRenderComponent*>(gameEntity->getRenderComponent());
    uiRect = render->getRenderRect();
}

UIPanelInputComponent::~UIPanelInputComponent()
{
    if (clickFunction!=NULL) { delete clickFunction; }
}

void UIPanelInputComponent::update(SDL_Event* event)
{
    if (event->type == SDL_MOUSEMOTION)
    {
        //If the mouse moves inside the button
        if (event->motion.x >= uiRect->x && event->motion.x <= uiRect->x+uiRect->w &&
            event->motion.y >= uiRect->y && event->motion.y <= uiRect->y+uiRect->h)
        {
            if (!render->isSelected() && render->buttonReady())
            {
                render->toggleSelected();
                //Element should now be selected
            }
        }
        else
        {
            if (render->isSelected() && render->buttonReady())
            {
                render->toggleSelected();
                //Element should now be unselected
            }
            if (clickFunction!=NULL && buttonPressedDown)
            {
                buttonPressedDown = false;
            }
        }
    }
    else if (clickFunction!=NULL && event->type == SDL_MOUSEBUTTONDOWN)
    {
        if (render->isSelected())
        {
            buttonPressedDown = true;
        }
    }
    else if (clickFunction!=NULL && event->type == SDL_MOUSEBUTTONUP && buttonPressedDown)
    {
        if (render->isSelected())
        {
            clickFunction->onClick();
        }
        buttonPressedDown = false;
    }
}

void UIPanelInputComponent::addClickFunction(UIClickFunction* clickFunction)
{
    this->clickFunction = clickFunction;
}