#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <__msvc_ostream.hpp>

#include "IComponent.h"

namespace sf
{
	class RenderWindow;
}

class Entity
{
public:
	/*
	* Constructor of the Entity class. The friendlyName parameter is for debugging purpose
	*/
	Entity(std::string friendlyName = "");

	/*
	* Destructor of the Entity class. All allocated resources must be released
	*/
	virtual ~Entity() = default;

	/*
	* First function that will be called when an entity will be added to the current scene.
	*/
	virtual void Start();

	/*
	* Function that will be called each frame if our entity is part of the current scene.
	*/
	virtual void Update(float fDeltaTime);

	/*
	* Last function that will be called before an entity is removed of the current scene.
	*/
	virtual void Destroy();

	/*
	* This function will be removed later, when the rendering system will be more robust. But for now, we need it here.
	  * Function that will be called each frame to draw the entity
	*/
	void Draw(sf::RenderWindow& window) const;

	std::vector<IComponent*>& GetAllComponents();
	
	std::string entityName;
	std::vector<IComponent*> components;
	bool bIsStarted = false;
	
	//Template ne peut être déclaré que dans le .h, le .cpp ne sait pas ce qu'est une template
	template<typename C> requires (std::is_base_of_v<IComponent, C>)
	C* AddComponent() {
		C* component = new C(*this);

		if (bIsStarted)
			component->Start();
		
		components.push_back(component);
		return component;
	}
	
	template<typename C> requires (std::is_base_of_v<IComponent, C>)
	C* GetComponent() const {
		for (IComponent* component : components) {
			if (C* comp = dynamic_cast<C*>(component))
				return comp;
		}

		return nullptr;
	}

	template<typename C> requires (std::is_base_of_v<IComponent, C>)
	bool RemoveComponent() {
		for (auto it = components.begin(); it != components.end(); ++it) {
			if (dynamic_cast<C*>(*it) != nullptr) {
				(*it)->Destroy();
				delete *it;
				components.erase(it);
				return true;
			}
		}

		return false;
	}
	
};
