#include "GameMgr.h"

#include "Entity.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

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

void GameMgr::DestroyEntity(Entity* entity) {
	if (!entity)
		return;

	for (auto it = Entities.begin(); it != Entities.end(); ++it) {
		if (entity == *it) {
			Entities.erase(it);
			break;
		}
	}
	
	entity->Destroy();
	delete entity;
}

std::vector<Entity*> GameMgr::GetEntities() {
	return Entities;
}
