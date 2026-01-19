#include "Globals.h"

#include "Entity.h"
#include "GameMgr.h"
#include "SpriteComponent.h"
#include "TextureMgr.h"
#include "TransformComponent.h"

Globals::Globals() {
}

Globals::~Globals() {
}

void Globals::Init() {
    textureMgr = new TextureMgr();
    gameMgr = new GameMgr();
}

void Globals::Destroy() {
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
