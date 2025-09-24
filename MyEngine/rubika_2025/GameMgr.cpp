#include "GameMgr.h"

#include "Entity.h"

GameMgr::GameMgr()
{}

GameMgr::~GameMgr()
{
	for (Entity* e : Entities)
	{
		e->Destroy();
		delete e;
	}

	Entities.clear();
}

void GameMgr::Update(float deltaTime)
{
	for (Entity* e : Entities)
	{
		e->Update(deltaTime);
	}
}

void GameMgr::Draw(sf::RenderWindow& window)
{
	for (Entity* e : Entities)
	{
		e->Draw(window);
	}
}

void GameMgr::AddEntity(Entity* entity)
{
	if (!entity)
	{
		return;
	}

	Entities.push_back(entity);
	entity->Start();
}

void GameMgr::AddComponent(Entity* entity, IComponent* component) {
	if (!entity)
		return;

	entity->components.emplace_back(component);
}

void GameMgr::RemoveComponent(Entity* entity, IComponent* component) {
	if (!entity)
		return;

	for (IComponent* e : entity->components) {
		if (e == component) {
			//Faire des trucs je sais pas
			break;
		}
	}
}
