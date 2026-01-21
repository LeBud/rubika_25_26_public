#include "CellularAutomata.h"

#include <SFML/Graphics/Image.hpp>

CellularAutomata::CellularAutomata(unsigned sizeX, unsigned sizeY, double spawnPercent, unsigned threshold, unsigned iteration) {
    SizeX = sizeX;
    SizeY = sizeY;
    SpawnPercent = spawnPercent;
    Iteration = iteration;
    NeighborThreshold = threshold;

    Cells.resize(SizeX * SizeY);
    std::memset(Cells.data(), 0, SizeX * SizeY * sizeof(int));
}

bool CellularAutomata::Generate() {
    //Border are living cells
    //A cell can't die if it was set alive (on) at some point
    //If cell is surrounded by X (neightborThreshold) living cell, the cell turn alive (on)
    
    //D'abord initialisé la grille avec le spawn percent par case pour avoir l'état de "base"
    //Puis appliqué les ruleset

    //Il me faut un random de double (linear je penses)
    //itérer sur chaque cellule et lancer le random, si inférieur au spawnPercent alors on
    //Check en même temps si une cellule est une bordure x/y 0 ou size 
    
    GenerateTexture(Texture);
    return false;
}

void CellularAutomata::GenerateTexture(sf::Texture& textures) const {
    sf::Image image;
    image.resize({SizeX,SizeY});
    for (unsigned i = 0; i < SizeX * SizeY; i++) {
        image.setPixel({i % SizeY, i / SizeY}, Cells[i] == 1 ? sf::Color::White : sf::Color::Black);
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
