#pragma once
#include <SFML/Graphics/Texture.hpp>

class CellularAutomata {
public:
    CellularAutomata(unsigned sizeX, unsigned sizeY, double spawnPercent, unsigned threshold, unsigned iteration);

    bool Generate();
    void GenerateTexture(sf::Texture& textures) const;

private:

    unsigned SizeX;
    unsigned SizeY;
    double SpawnPercent;
    unsigned NeighborThreshold;
    unsigned Iteration;
};
