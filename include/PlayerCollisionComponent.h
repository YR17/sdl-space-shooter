/*******************************************************************//*
 * Collision component for Player.
 *
 * @author      Brandon To
 * @version     1.0
 * @since       2014-09-18
 * @modified    2014-09-22
 *********************************************************************/
#ifndef SPACESHOOTER_PLAYERCOLLISIONCOMPONENT_
    #define SPACESHOOTER_PLAYERCOLLISIONCOMPONENT_

#include "CollisionComponent.h"
#include <SDL2/SDL.h>
#include "Timer.h"
#include "Vector2D.h"

//NOT COMPLETE

class CollisionManager;
class GameEntity;
class GameEntityManager;
class PlayerPhysicsComponent;
class PlayerRenderComponent;

struct WindowElements;

class PlayerCollisionComponent : public CollisionComponent
{
    private:
        CollisionManager* collisionManager;

        PlayerPhysicsComponent* physics;
        PlayerRenderComponent* render;

    public:
        //Constructor
        PlayerCollisionComponent(GameEntity* gameEntity, WindowElements* windowElements,
                                CollisionManager* collisionManager);

        //Destructor
        ~PlayerCollisionComponent();

        //Methods
        void update();
};

#endif