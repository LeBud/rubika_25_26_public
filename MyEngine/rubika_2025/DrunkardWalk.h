#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <vector>

class DrunkardWalk {
public:
    DrunkardWalk(unsigned sizeX, unsigned sizeY, unsigned iteration, unsigned distance, unsigned spawnNumber);

    bool Generate();
    void GenerateTexture(sf::Texture& texture) const;

private:
    
    unsigned SizeX;
    unsigned SizeY;
    unsigned Iteration;
    unsigned Distance;
    unsigned SpawnNumber;

    std::vector<int> Cells;
};
