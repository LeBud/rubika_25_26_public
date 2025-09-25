#pragma once

#include "IComponent.h"
#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "TextureMgr.h"

class SpriteComponent : public IComponent
{
public:
	SpriteComponent(Entity& entity);
	~SpriteComponent() = default;

	virtual void Start() override;

	virtual void Update(float fDeltaTime) override;

	virtual void Draw(sf::RenderWindow& window) const override;

	virtual void Destroy() override;

	void SetTexture(const std::string& textureName);
	void SetAnimation(const std::string& animationName);

	void PlayAnimation(bool bPause);

	void UpdateAnimation(int currentSprite);

	const sTextureData* textureData;
	const sAnimationData* currentAnimationData;
	
	sf::Sprite sprite;
	
	bool bPause;
	int currentSpriteIndex = 1;
	float currentTimer = 0;
};
