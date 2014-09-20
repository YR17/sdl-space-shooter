/*******************************************************************//*
 * Implementation of the GameEntity class.
 *
 * @author      Brandon To
 * @version     1.0
 * @since       2014-08-10
 * @modified    2014-09-18
 *********************************************************************/
#include "GameEntity.h"

#include <cstddef>
#include <SDL2/SDL.h>
#include "InputComponent.h"
#include "RenderComponent.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"

//Constructor
GameEntity::GameEntity()
:   position(0, 0),
    remove(false),
    active(false),
    input(NULL),
    render(NULL),
    physics(NULL),
    collision(NULL)
{}

GameEntity::~GameEntity()
{
    if (input!=NULL) { delete input; }
    if (render!=NULL) { delete render; }
    if (physics!=NULL) { delete physics; }
    if (collision!=NULL) { delete collision; }
}

void GameEntity::addInputComponent(InputComponent* input)
{
    this->input = input;
}

void GameEntity::addRenderComponent(RenderComponent* render)
{
    this->render = render;
}

void GameEntity::addPhysicsComponent(PhysicsComponent* physics)
{
    this->physics = physics;
}

void GameEntity::addCollisionComponent(CollisionComponent* collision)
{
    this->collision = collision;
}

void GameEntity::onEvent(SDL_Event *event)
{
    if (input!=NULL) { input->update(event); }
}

void GameEntity::onUpdate()
{
    if (physics!=NULL) { physics->update(); }
    if (collision!=NULL) { collision->update(); }
}

void GameEntity::onRender()
{
    if (render!=NULL) { render->update(); }
}

InputComponent* GameEntity::getInputComponent()
{
    return input;
}

RenderComponent* GameEntity::getRenderComponent()
{
    return render;
}

PhysicsComponent* GameEntity::getPhysicsComponent()
{
    return physics;
}

CollisionComponent* GameEntity::getCollisionComponent()
{
    return collision;
}
