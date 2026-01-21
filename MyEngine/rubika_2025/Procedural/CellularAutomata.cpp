#include "CellularAutomata.h"

#include <SFML/Graphics/Image.hpp>

CellularAutomata::CellularAutomata(unsigned sizeX, unsigned sizeY, double spawnPercent, unsigned threshold, unsigned iteration) {
    SizeX = sizeX;
    SizeY = sizeY;
    SpawnPercent = spawnPercent;
    Iteration = iteration;
    NeighborThreshold = threshold;
}

bool CellularAutomata::Generate() {
    
    return false;
}

void CellularAutomata::GenerateTexture(sf::Texture& textures) const {
    sf::Image image;
    image.resize({SizeX,SizeY});
    for (unsigned i = 0; i < SizeX * SizeY; i++) {
        image.setPixel({i % SizeY, i / SizeY}, Cells[i] == 1 ? sf::Color::White : sf::Color::Red);
    }
    textures.resize({SizeX,SizeY});
    textures.loadFromImage(image);
    textures.update(image);
}

sf::Sprite CellularAutomata::Sprite() {
    sf::Sprite sprite(Texture);

    sprite.setOrigin({static_cast<float>(SizeX) / 2, static_cast<float>(SizeY) / 2});
    sprite.setScale({1.f,1.f});
    sprite.setPosition({800.f,200.f});
    return sprite;
}
