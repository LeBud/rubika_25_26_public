#pragma once

#include <vector>

#include "IComponent.h"

namespace sf
{
	class RenderWindow;
};

class Entity;

// This all class is temporary and will be reworked later
class GameMgr
{
public:
	GameMgr();
	~GameMgr();

	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window);

	void AddEntity(Entity* entity);

	void AddComponent(Entity* entity, IComponent* component);
	void RemoveComponent(Entity* entity, IComponent* component);

	
private:
	std::vector<Entity*> Entities;
};