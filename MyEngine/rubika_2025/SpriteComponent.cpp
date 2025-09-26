#include <SpriteComponent.h>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Entity.h"
#include "Globals.h"
#include "TransformComponent.h"


SpriteComponent::SpriteComponent(Entity& entity) : IComponent(entity),
    sprite(Globals::GetInstance()->GetTextureMgr()->GetTextureData("").Texture)
{
}

void SpriteComponent::Start() {
}

void SpriteComponent::Update(float fDeltaTime) {

    //Handle animation update
    if (!bPause && currentAnimationData != nullptr) {
        if (currentAnimationData->AnimationSpriteCount < 2) return;
        
        currentTimer += fDeltaTime;
        if (currentTimer >= currentAnimationData->TimeBetweenAnimationInS) {

            if (currentAnimationData->IsReverted) {
                currentSpriteIndex--;
                
                if (currentSpriteIndex < 0) 
                    currentSpriteIndex = currentAnimationData->AnimationSpriteCount - 1;
            }
            else {
                currentSpriteIndex++;
                
                if (currentSpriteIndex >= currentAnimationData->AnimationSpriteCount) 
                    currentSpriteIndex = 0;
            }
                
            currentTimer = 0.0f;
            UpdateAnimation(currentSpriteIndex);
        }
    }
}

void SpriteComponent::Draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

void SpriteComponent::Destroy() {
    if (textureData != nullptr) {
        auto previousTexture = textureData->TextureName;
        Globals::GetInstance()->GetTextureMgr()->RemoveEntityUsedTexture(previousTexture, GetEntity());
    }
}

void SpriteComponent::PlayAnimation(bool bPause) {
    this->bPause = bPause;
}

void SpriteComponent::UpdateAnimation(int currentSprite) {
    
    //if (bPause) return;
    
    int currentX = currentAnimationData->StartX + currentAnimationData->SizeX * currentSprite + currentAnimationData->OffsetX * currentSprite;
    int currentY = currentAnimationData->StartY /*+ currentAnimationData->SizeY * currentSprite + currentAnimationData->OffsetY * currentSprite*/;

    sprite.setTextureRect(sf::IntRect({currentX, currentY}, {currentAnimationData->SizeX, currentAnimationData->SizeY}));
    sprite.setOrigin({(float)currentAnimationData->SizeX / 2,(float)currentAnimationData->SizeY / 2});

    //Set Position, rotation and scale to transform position, rotation and scale
    if (GetEntity().GetComponent<TransformComponent>()) { //Securité pour check que l'entity a un transform
        sprite.setPosition(GetEntity().GetComponent<TransformComponent>()->GetPosition());
        sprite.setRotation(GetEntity().GetComponent<TransformComponent>()->GetRotation());
        sprite.setScale(GetEntity().GetComponent<TransformComponent>()->GetScale());
    }
}

void SpriteComponent::SetTexture(const std::string& textureName) {
    if (textureData != nullptr) {
        auto previousTexture = textureData->TextureName;
        Globals::GetInstance()->GetTextureMgr()->RemoveEntityUsedTexture(previousTexture, GetEntity());
    }
    
    textureData = &Globals::GetInstance()->GetTextureMgr()->GetTextureData(textureName);
    sprite.setTexture(textureData->Texture);

    Globals::GetInstance()->GetTextureMgr()->PopulateTextureUsedByEntity(textureName, GetEntity());
}

void SpriteComponent::SetAnimation(const std::string& animationName) {
    //Get animation data dans le textureData
    //Stock dans mon current animation data

    const auto& it = textureData->AnimationData.find(animationName);
    if (it != textureData->AnimationData.end()) {
        currentAnimationData = &it->second;
        currentAnimationName = animationName;
    }
    else {
        currentAnimationData = &textureData->AnimationData.begin()->second; //Sécurité dans le doute de ne pas trouver l'animation souhaité
        currentAnimationName = textureData->AnimationData.begin()->first.c_str();
    }

    currentSpriteIndex = 0;
    UpdateAnimation(currentSpriteIndex);
    
}
