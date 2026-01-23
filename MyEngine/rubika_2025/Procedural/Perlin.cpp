#include "Perlin.h"

#include <SFML/Graphics/Image.hpp>

#include "Globals.h"

Perlin::Perlin(unsigned sizeX, unsigned sizeY, unsigned cellSize, unsigned octaves) {
    SizeX = sizeX;
    SizeY = sizeY;
    CellSize = cellSize;
    Octaves = octaves;

    Cells.resize(SizeX * SizeY);
    std::memset(Cells.data(), 0, SizeX * SizeY * sizeof(int));
}

bool Perlin::Generate() {

    for (unsigned i = 0; i < SizeX * SizeY; i++) {
        Cells[i] = Globals::GetInstance()->GetRandomMgr()->GetInstance(0)->RandInt32(-1, 1);
    }
    
    GenerateTexture(Texture);
    return false;
}

void Perlin::GenerateTexture(sf::Texture& texture) const {
    sf::Image image;
    image.resize({SizeX,SizeY});
    for (unsigned i = 0; i < SizeX * SizeY; i++) {
        image.setPixel({i % SizeY, i / SizeY}, lerpColor(sf::Color::Black, sf::Color::White, (Cells[i] + 1.f) / 2.f));
    }
    texture.resize({SizeX,SizeY});
    texture.loadFromImage(image);
    texture.update(image);
}

sf::Sprite Perlin::Sprite() {
    sf::Sprite sprite(Texture);

    sprite.setOrigin({static_cast<float>(SizeX) / 2, static_cast<float>(SizeY) / 2});
    sprite.setScale({1.f,1.f});
    sprite.setPosition({800.f,200.f});
    return sprite;
}

sf::Color Perlin::lerpColor(const sf::Color color1, const sf::Color color2, float amount) const {
    amount = std::clamp(amount, 0.f, 1.f);

    return {
        static_cast<uint8_t>(std::lerp(color1.r, color2.r, amount)),
        static_cast<uint8_t>(std::lerp(color1.g, color2.g, amount)),
        static_cast<uint8_t>(std::lerp(color1.b, color2.b, amount)),
        static_cast<uint8_t>(std::lerp(color1.a, color2.a, amount))
    };
}

