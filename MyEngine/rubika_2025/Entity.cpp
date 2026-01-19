#include <Entity.h>

Entity::Entity(std::string friendlyName) {
    entityName = friendlyName;
}

void Entity::Start() {
    for (IComponent* c : components){
        c->Start();
    }

    bIsStarted = true;
}

void Entity::Update(float fDeltaTime) {
    for (IComponent* c : components){
        c->Update(fDeltaTime);
    }
}

void Entity::Destroy() {
    for (IComponent* c : components){
        c->Destroy();
    }
}

void Entity::Draw(sf::RenderWindow& window) const {
    for (IComponent* c : components){
        c->Draw(window);
    }
}

std::vector<IComponent*>& Entity::GetAllComponents() {
    return components;
}
