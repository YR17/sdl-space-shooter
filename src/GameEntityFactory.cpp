/*******************************************************************//*
 * Implementation of the GameEntityFactory class.
 *
 * @author      Brandon To
 * @version     1.0
 * @since       2015-02-07
 * @modified    2015-02-17
 *********************************************************************/
#include "GameEntityFactory.h"

#include "ApplicationState.h"
#include "BackgroundPhysicsComponent.h"
#include "BackgroundRenderComponent.h"
#include "EnemyCollisionComponent.h"
#include "EnemyPhysicsComponent.h"
#include "EnemyRenderComponent.h"
#include "ExplosionPhysicsComponent.h"
#include "ExplosionRenderComponent.h"
#include "EnemyProjectileCollisionComponent.h"
#include "EnemyProjectilePhysicsComponent.h"
#include "EnemyProjectileRenderComponent.h"
#include "GameEntity.h"
#include "GameEntityManager.h"
#include "MeteorPhysicsComponent.h"
#include "MeteorRenderComponent.h"
#include "MovementPattern.h"
#include "PlayerCollisionComponent.h"
#include "PlayerInputComponent.h"
#include "PlayerInstructionsInputComponent.h"
#include "PlayerInstructionsPhysicsComponent.h"
#include "PlayerInstructionsProjectilePhysicsComponent.h"
#include "PlayerPhysicsComponent.h"
#include "PlayerRenderComponent.h"
#include "PlayerProjectileCollisionComponent.h"
#include "PlayerProjectilePhysicsComponent.h"
#include "PlayerProjectileRenderComponent.h"
#include "SpriteRenderComponent.h"
#include "TextRenderComponent.h"
#include "Texture.h"
#include "UIClickFunction.h"
#include "UIPanelInputComponent.h"
#include "UIPanelRenderComponent.h"
#include "WindowElements.h"

GameEntityFactory::GameEntityFactory(GameEntityManager* gameEntityManager,
                                    WindowElements* windowElements)
:   gameEntityManager(gameEntityManager),
    windowElements(windowElements)
{
    stringToEntityEnum["background"] = ENTITY_BACKGROUND;
    stringToEntityEnum["enemyProjectileHit"] = ENTITY_ENEMYPROJECTILEHIT;
    stringToEntityEnum["enemyStraight"] = ENTITY_ENEMYSTRAIGHT;
    stringToEntityEnum["enemyCarrier"] = ENTITY_ENEMYCARRIER;
    stringToEntityEnum["enemySwoopLeft"] = ENTITY_ENEMYSWOOPLEFT;
    stringToEntityEnum["enemySwoopRight"] = ENTITY_ENEMYSWOOPRIGHT;
    stringToEntityEnum["enemyZigZag"] = ENTITY_ENEMYZIGZAG;
    stringToEntityEnum["player"] = ENTITY_PLAYER1;
    stringToEntityEnum["playerProjectileHit"] = ENTITY_PLAYERPROJECTILEHIT;
    stringToEntityEnum["sprite"] = ENTITY_SPRITE;
    stringToEntityEnum["text"] = ENTITY_TEXT;
    stringToEntityEnum["uiPanel"] = ENTITY_UIPANEL;
}

GameEntityFactory::~GameEntityFactory()
{

}

std::vector<GameEntity*> GameEntityFactory::createBackgroundEntities()
{
    std::vector<EntityXmlStruct> xmlStructs = gameEntityData.getBackgroundData(gameEntityManager->getState());
    GameEntity* background[1];
    background[0] = createBackground();
    std::vector<GameEntity*> backgroundVector(background, background + sizeof(background)/sizeof(GameEntity*));
    return backgroundVector;
}

std::vector<GameEntity*> GameEntityFactory::createPhysicalEntities()
{
    std::vector<EntityXmlStruct> xmlStructs = gameEntityData.getPhysicalData(gameEntityManager->getState());
    int numEntities = xmlStructs.size();

    GameEntity* physical[numEntities];

    for (int i=0; i<numEntities; i++)
    {
        physical[i] = createEntity(xmlStructs[i]);
    }

    std::vector<GameEntity*> physicalVector(physical, physical + sizeof(physical)/sizeof(GameEntity*));
    return physicalVector;
}

std::vector<GameEntity*> GameEntityFactory::createEffectEntities()
{
    //gameEntityData.getEffectData(gameEntityManager->getState());
}

std::vector<GameEntity*> GameEntityFactory::createUIEntities()
{
    std::vector<EntityXmlStruct> xmlStructs = gameEntityData.getUIData(gameEntityManager->getState());
    int numEntities = xmlStructs.size();

    GameEntity* ui[numEntities];

    for (int i=0; i<numEntities; i++)
    {
        ui[i] = createEntity(xmlStructs[i]);
    }

    std::vector<GameEntity*> uiVector(ui, ui + sizeof(ui)/sizeof(GameEntity*));
    return uiVector;
}

GameEntity* GameEntityFactory::createEntity(std::string type)
{
    EntityXmlStruct xmlStruct = gameEntityData.getDataByType(gameEntityManager->getState(), type);
    return createEntity(xmlStruct);
}

GameEntity* GameEntityFactory::createEntity(SpawnData data)
{
    EntityXmlStruct xmlStruct = gameEntityData.getDataByType(gameEntityManager->getState(), data.type);
    xmlStruct.x = data.x;
    xmlStruct.y = data.y;
    xmlStruct.health = data.health;
    return createEntity(xmlStruct);
}

GameEntity* GameEntityFactory::createEntity(EntityXmlStruct xmlStruct)
{
    GameEntity* entity = new GameEntity();

    switch (stringToEntityEnum[xmlStruct.type])
    {
        case ENTITY_BACKGROUND:
        {
            entity->addRenderComponent(new BackgroundRenderComponent(entity, windowElements));
            entity->addPhysicsComponent(new BackgroundPhysicsComponent(entity, windowElements));
            configureEntity(entity, xmlStruct);
            gameEntityManager->addBackgroundEntity(entity);
            break;
        }

        case ENTITY_ENEMYPROJECTILEHIT:
        {
            SpriteRenderComponent* render = new SpriteRenderComponent(entity, windowElements);
            SDL_Rect rect = render->getRenderRect();
            rect.x = xmlStruct.x - xmlStruct.width/2;
            rect.y = xmlStruct.y - xmlStruct.height/2;
            render->setRenderRect(&rect);
            render->setActiveTimer(100);
            entity->addRenderComponent(render);
            configureEntity(entity, xmlStruct);
            gameEntityManager->addEffectEntity(entity);
            break;
        }

        case ENTITY_ENEMYSTRAIGHT:
        {
            entity->addRenderComponent(new EnemyRenderComponent(entity, windowElements));
            EnemyPhysicsComponent* physics = new EnemyPhysicsComponent(entity, windowElements, this);
            physics->getMovementPattern()->setMovementPattern(MOVEMENT_STRAIGHT);
            entity->addPhysicsComponent(physics);
            entity->addCollisionComponent(new EnemyCollisionComponent(entity, windowElements, gameEntityManager->getCollisionManager()));
            entity->position.x = xmlStruct.x;
            entity->position.y = xmlStruct.y;
            configureEntity(entity, xmlStruct);
            gameEntityManager->addPhysicalEntity(entity);
            break;
        }

        case ENTITY_ENEMYCARRIER:
        {
            entity->addRenderComponent(new EnemyRenderComponent(entity, windowElements));
            EnemyPhysicsComponent* physics = new EnemyPhysicsComponent(entity, windowElements, this);
            physics->getMovementPattern()->setMovementPattern(MOVEMENT_STRAIGHTSLOW);
            physics->setMaxHealth(xmlStruct.health);
            entity->addPhysicsComponent(physics);
            entity->addCollisionComponent(new EnemyCollisionComponent(entity, windowElements, gameEntityManager->getCollisionManager()));
            entity->position.x = xmlStruct.x;
            entity->position.y = xmlStruct.y;
            configureEntity(entity, xmlStruct);
            gameEntityManager->addPhysicalEntity(entity);
            break;
        }

        case ENTITY_ENEMYSWOOPLEFT:
        {
            entity->addRenderComponent(new EnemyRenderComponent(entity, windowElements));
            EnemyPhysicsComponent* physics = new EnemyPhysicsComponent(entity, windowElements, this);
            physics->getMovementPattern()->setMovementPattern(MOVEMENT_SWOOPLEFT);
            entity->addPhysicsComponent(physics);
            entity->addCollisionComponent(new EnemyCollisionComponent(entity, windowElements, gameEntityManager->getCollisionManager()));
            entity->position.x = xmlStruct.x;
            entity->position.y = xmlStruct.y;
            configureEntity(entity, xmlStruct);
            gameEntityManager->addPhysicalEntity(entity);
            break;
        }

        case ENTITY_ENEMYSWOOPRIGHT:
        {
            entity->addRenderComponent(new EnemyRenderComponent(entity, windowElements));
            EnemyPhysicsComponent* physics = new EnemyPhysicsComponent(entity, windowElements, this);
            physics->getMovementPattern()->setMovementPattern(MOVEMENT_SWOOPRIGHT);
            entity->addPhysicsComponent(physics);
            entity->addCollisionComponent(new EnemyCollisionComponent(entity, windowElements, gameEntityManager->getCollisionManager()));
            entity->position.x = xmlStruct.x;
            entity->position.y = xmlStruct.y;
            configureEntity(entity, xmlStruct);
            gameEntityManager->addPhysicalEntity(entity);
            break;
        }

        case ENTITY_ENEMYZIGZAG:
        {
            entity->addRenderComponent(new EnemyRenderComponent(entity, windowElements));
            EnemyPhysicsComponent* physics = new EnemyPhysicsComponent(entity, windowElements, this);
            physics->getMovementPattern()->setMovementPattern(MOVEMENT_ZIGZAG);
            entity->addPhysicsComponent(physics);
            entity->addCollisionComponent(new EnemyCollisionComponent(entity, windowElements, gameEntityManager->getCollisionManager()));
            entity->position.x = xmlStruct.x;
            entity->position.y = xmlStruct.y;
            configureEntity(entity, xmlStruct);
            gameEntityManager->addPhysicalEntity(entity);
            break;
        }

        case ENTITY_PLAYER1:
        {
            PlayerRenderComponent* render = new PlayerRenderComponent(entity, windowElements);
            Texture* damagedTexture1 = new Texture(windowElements);
            damagedTexture1->setTexture("../data/graphics/sprites/playerShip1_damage1.png");
            render->addDamagedSprite(3, damagedTexture1);
            Texture* damagedTexture2 = new Texture(windowElements);
            damagedTexture2->setTexture("../data/graphics/sprites/playerShip1_damage2.png");
            render->addDamagedSprite(2, damagedTexture2);
            Texture* damagedTexture3 = new Texture(windowElements);
            damagedTexture3->setTexture("../data/graphics/sprites/playerShip1_damage3.png");
            render->addDamagedSprite(1, damagedTexture3);
            entity->addRenderComponent(render);
            entity->addPhysicsComponent(new PlayerPhysicsComponent(entity, windowElements, this));
            entity->addCollisionComponent(new PlayerCollisionComponent(entity, windowElements, gameEntityManager->getCollisionManager()));
            entity->addInputComponent(new PlayerInputComponent(entity, windowElements));
            entity->position.x = xmlStruct.x;
            entity->position.y = xmlStruct.y;
            configureEntity(entity, xmlStruct);
            gameEntityManager->addPhysicalEntity(entity);
            break;
        }

        case ENTITY_PLAYERPROJECTILEHIT:
        {
            SpriteRenderComponent* render = new SpriteRenderComponent(entity, windowElements);
            SDL_Rect rect = render->getRenderRect();
            rect.x = xmlStruct.x - xmlStruct.width/2;
            rect.y = xmlStruct.y - xmlStruct.height/2;
            render->setRenderRect(&rect);
            render->setActiveTimer(100);
            entity->addRenderComponent(render);
            configureEntity(entity, xmlStruct);
            gameEntityManager->addEffectEntity(entity);
            break;
        }

        case ENTITY_SPRITE:
        {
            entity->addRenderComponent(new SpriteRenderComponent(entity, windowElements));
            SDL_Rect rect = entity->getRenderComponent()->getRenderRect();
            rect.x = xmlStruct.x;
            rect.y = xmlStruct.y;
            entity->getRenderComponent()->setRenderRect(&rect);
            configureEntity(entity, xmlStruct);
            gameEntityManager->addPhysicalEntity(entity);
            break;
        }

        case ENTITY_TEXT:
        {
            TextRenderComponent* entityRender = new TextRenderComponent(entity, windowElements);
            entityRender->setText(xmlStruct.data, 96);
            entity->addRenderComponent(entityRender);
            SDL_Rect rect = entity->getRenderComponent()->getRenderRect();
            rect.x = xmlStruct.x;
            rect.y = xmlStruct.y;
            entity->getRenderComponent()->setRenderRect(&rect);
            configureEntity(entity, xmlStruct);
            gameEntityManager->addUIEntity(entity);
            break;
        }

        case ENTITY_UIPANEL:
        {
            UIPanelRenderComponent* entityRender = new UIPanelRenderComponent(entity, windowElements);
            entity->addRenderComponent(entityRender);
            SDL_Rect rect = entity->getRenderComponent()->getRenderRect();
            rect.x = xmlStruct.x;
            rect.y = xmlStruct.y;
            entity->getRenderComponent()->setRenderRect(&rect);
            if (xmlStruct.function != "NONE")
            {
                UIPanelInputComponent* entityInput = new UIPanelInputComponent(entity);
                UIClickFunction* clickFunction = new UIClickFunction(gameEntityManager->getState());
                clickFunction->setCallback(xmlStruct.function);
                entityInput->addClickFunction(clickFunction);
                entity->addInputComponent(entityInput);
            }
            configureEntity(entity, xmlStruct);
            gameEntityManager->addUIEntity(entity);
            break;
        }

        default:
            fprintf(stderr, "[ERROR] createEntity(): unknown type.\n");
            delete entity;
            entity = NULL;
            break;
    }

    return entity;
}

// Configures entity using the data passed in xmlStruct
void GameEntityFactory::configureEntity(GameEntity* entity, EntityXmlStruct xmlStruct)
{
    entity->setID(xmlStruct.id);
    entity->setName(xmlStruct.name);
    entity->setType(xmlStruct.type);
    // Only sets texture if there was one specified, otherwise use the default
    if (xmlStruct.texture != "")
    {
        entity->getRenderComponent()->getTexture()->setTexture(xmlStruct.texture);
    }
    SDL_Rect rect = entity->getRenderComponent()->getRenderRect();
    rect.w = xmlStruct.width;
    rect.h = xmlStruct.height;
    entity->getRenderComponent()->setRenderRect(&rect);
    if (xmlStruct.alphaEnabled)
    {
        entity->getRenderComponent()->getTexture()->enableAlphaBlend();
    }
}

GameEntity* GameEntityFactory::createBackground()
{
    GameEntity* background = new GameEntity();
    background->addRenderComponent(new BackgroundRenderComponent(background, windowElements));
    background->addPhysicsComponent(new BackgroundPhysicsComponent(background, windowElements));
    gameEntityManager->addBackgroundEntity(background);

    return background;
}

GameEntity* GameEntityFactory::createEnemyProjectile(GameEntity* enemyEntity)
{
    GameEntity* projectile = new GameEntity();
    projectile->addRenderComponent(new EnemyProjectileRenderComponent(projectile, windowElements, enemyEntity));
    projectile->addPhysicsComponent(new EnemyProjectilePhysicsComponent(projectile, windowElements, this));
    projectile->addCollisionComponent(new EnemyProjectileCollisionComponent(projectile, windowElements, gameEntityManager->getCollisionManager()));
    gameEntityManager->addPhysicalEntity(projectile);

    return projectile;
}

GameEntity* GameEntityFactory::createExplosion(GameEntity* destroyedEntity)
{
    GameEntity* explosion = new GameEntity();
    explosion->addRenderComponent(new ExplosionRenderComponent(explosion, windowElements, destroyedEntity));
    explosion->addPhysicsComponent(new ExplosionPhysicsComponent(explosion, windowElements));
    gameEntityManager->addEffectEntity(explosion);

    return explosion;
}

GameEntity* GameEntityFactory::createPlayerProjectile(GameEntity* playerEntity)
{
    GameEntity* projectile = new GameEntity();
    projectile->addRenderComponent(new PlayerProjectileRenderComponent(projectile, windowElements, playerEntity));
    projectile->addPhysicsComponent(new PlayerProjectilePhysicsComponent(projectile, windowElements, this));
    projectile->addCollisionComponent(new PlayerProjectileCollisionComponent(projectile, windowElements, gameEntityManager->getCollisionManager()));
    gameEntityManager->addPhysicalEntity(projectile);

    return projectile;
}

std::vector<GameEntity*> GameEntityFactory::createPlayerInstructions()
{
    GameEntity* player[1];
    player[0]= new GameEntity();
    PlayerRenderComponent* playerRender = new PlayerRenderComponent(player[0], windowElements);
    playerRender->getTexture()->enableAlphaBlend();
    player[0]->addRenderComponent(playerRender);
    player[0]->addPhysicsComponent(new PlayerInstructionsPhysicsComponent(player[0], windowElements, this));
    player[0]->addInputComponent(new PlayerInstructionsInputComponent(player[0], windowElements));
    gameEntityManager->addPhysicalEntity(player[0]);

    std::vector<GameEntity*> playerVector(player, player + sizeof(player)/sizeof(GameEntity*));
    return playerVector;
}

GameEntity* GameEntityFactory::createPlayerInstructionsProjectile(GameEntity* playerEntity)
{
    GameEntity* projectile = new GameEntity();
    projectile->addRenderComponent(new PlayerProjectileRenderComponent(projectile, windowElements, playerEntity));
    projectile->addPhysicsComponent(new PlayerInstructionsProjectilePhysicsComponent(projectile, windowElements));
    projectile->addCollisionComponent(new PlayerProjectileCollisionComponent(projectile, windowElements, gameEntityManager->getCollisionManager()));
    gameEntityManager->addPhysicalEntity(projectile);

    return projectile;
}

GameEntity* GameEntityFactory::createMeteor()
{
    GameEntity* meteor = new GameEntity();
    meteor->addRenderComponent(new MeteorRenderComponent(meteor, windowElements));
    meteor->addPhysicsComponent(new MeteorPhysicsComponent(meteor, windowElements));
    gameEntityManager->addPhysicalEntity(meteor);

    return meteor;
}
