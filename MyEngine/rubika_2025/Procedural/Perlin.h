#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class Perlin {
public:
    Perlin(unsigned sizeX, unsigned sizeY, unsigned cellSize, unsigned octaves);

    bool Generate();

    void GenerateTexture(sf::Texture& texture) const;

    sf::Sprite Sprite();
private:
    
    unsigned SizeX;
    unsigned SizeY;
    unsigned CellSize;
    unsigned Octaves;

    std::vector<int> Cells;
    sf::Texture Texture;
    
    sf::Color lerpColor(const sf::Color color1, const sf::Color color2, float amount) const;
};
