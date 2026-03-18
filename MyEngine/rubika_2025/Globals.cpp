#include "Globals.h"

#include "Entity.h"
#include "Manager/GameMgr.h"
#include "Component/SpriteComponent.h"
#include "Manager/TextureMgr.h"
#include "Component/TransformComponent.h"

Globals gData;

Globals::Globals() {
    taskMgr = new TaskMgr();
    textureMgr = new TextureMgr();
    gameMgr = new GameMgr();
    randomMgr = new RandomMgr();
}

Globals::~Globals() {
    Destroy();
}

void Globals::Init() {
    taskMgr->Init();
}

void Globals::Shut()
{
    taskMgr->Shut();
    //GameMgr->Shut();
    //DebugMgr->Shut();
    // Console->Shut();
    // RandomMgr->Shut();
}

void Globals::Destroy() {
    delete gameMgr;
    gameMgr = nullptr;

    delete textureMgr;
    textureMgr = nullptr;

    delete randomMgr;
    randomMgr = nullptr;
}

void Globals::CreateBaseEntity(std::string baseEntityName) {
    Entity* baseEntity = new Entity(baseEntityName);
    baseEntity->AddComponent<TransformComponent>();
    baseEntity->GetComponent<TransformComponent>()->SetPosition(sf::Vector2f{200,200});
    
    baseEntity->AddComponent<SpriteComponent>();

    if (GetTextureMgr()->LoadTexture("../Ressources/IsaacSprite.png")) {
        SpriteComponent* sprite = baseEntity->GetComponent<SpriteComponent>();
        sprite->SetTexture("../Ressources/IsaacSprite.png");
        sprite->SetAnimation("Body_Vertical");
        sprite = nullptr;
    }

    GetGameMgr()->AddEntity(baseEntity);
}

TextureMgr* Globals::GetTextureMgr() const {
    if (textureMgr != nullptr)
        return textureMgr;

    return nullptr;
}

GameMgr* Globals::GetGameMgr() const {
    if (gameMgr != nullptr)
        return gameMgr;
    
    return nullptr;
}

RandomMgr* Globals::GetRandomMgr() const {
    if (randomMgr != nullptr)
        return randomMgr;
    
    return nullptr;
}

TaskMgr* Globals::GetTaskMgr() const {
    if (taskMgr != nullptr)
        return taskMgr;
    
    return nullptr;
}
