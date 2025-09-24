#include <Entity.h>

void Entity::Update(float fDeltaTime) {
    for (IComponent* c : components)
    {
        c->Update(fDeltaTime);
    }
}

void Entity::Draw(sf::RenderWindow& window) const {
    for (IComponent* c : components)
    {
        c->Draw(window);
    }
}
