#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <vector>
#include <SFML/Graphics/Sprite.hpp>

class DrunkardWalk {
public:
    DrunkardWalk(unsigned sizeX, unsigned sizeY, unsigned iteration, unsigned distance, unsigned spawnNumber);

    bool Generate();
    void GenerateTexture(sf::Texture& texture) const;

    sf::Sprite Sprite();
    bool Generated;
private:
    
    unsigned SizeX;
    unsigned SizeY;
    unsigned Iteration;
    unsigned Distance;
    unsigned SpawnNumber;


    std::vector<int> Cells;
    sf::Texture Texture;
};
