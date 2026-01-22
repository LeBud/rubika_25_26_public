#pragma once
#include <unordered_map>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class CellularAutomata {
public:
    CellularAutomata(unsigned sizeX, unsigned sizeY, double spawnPercent, unsigned threshold, unsigned iteration);

    bool Generate();
    void GenerateTexture(sf::Texture& textures) const;

    sf::Sprite Sprite(sf::Texture& tex) const;
    std::vector<sf::Sprite> Sprites;
private:

    unsigned SizeX;
    unsigned SizeY;
    double SpawnPercent;
    unsigned NeighborThreshold;
    unsigned Iteration;

    std::vector<int> Cells;
    std::vector<sf::Texture> Texture;
};
