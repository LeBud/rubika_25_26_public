#include "CellularAutomata.h"

#include <SFML/Graphics/Image.hpp>

#include "Globals.h"

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
    for (unsigned i = 0; i < SizeX * SizeY; i++) { // boucle principale
        if (i % SizeY == 0 || i % SizeY == SizeX - 1) {
            Cells[i] = 1;
            continue;
        }
        if (i / SizeY == 0 || i / SizeY == SizeY - 1) {
            Cells[i] = 1;
            continue;
        }
        double rdm = Globals::GetInstance()->GetRandomMgr()->GetInstance(0)->RandDouble(0.f, 1.f);
        if (rdm < SpawnPercent) {
            Cells[i] = 1;
        }
    }

    Texture.emplace_back();
    GenerateTexture(Texture[0]);
    Sprites.push_back(Sprite(Texture[0]));
    
    for (unsigned it = 0; it < Iteration; it++) {
        for (unsigned i = 0; i < SizeX * SizeY; i++) {
            if (i % SizeY == 0 || i % SizeY == SizeX - 1) {
                continue;
            }
            if (i / SizeY == 0 || i / SizeY == SizeY - 1) {
                continue;
            }
            if (Cells[i] == 1) {
                continue;
            }

            sf::Vector2<unsigned> pos (i % SizeY, i / SizeY);
            unsigned count = 0;
            if (Cells[(pos.y + 1) * SizeY + pos.x] == 1)
                count++;
            if (Cells[(pos.y + 1) * SizeY + pos.x + 1] == 1)
                count++;
            if (Cells[(pos.y + 1) * SizeY + pos.x - 1] == 1)
                count++;
            if (Cells[pos.y * SizeY + pos.x - 1] == 1)
                count++;
            if (Cells[pos.y * SizeY + pos.x + 1] == 1)
                count++;
            if (Cells[(pos.y - 1) * SizeY + pos.x - 1] == 1)
                count++;
            if (Cells[(pos.y - 1) * SizeY + pos.x] == 1)
                count++;
            if (Cells[(pos.y - 1) * SizeY + pos.x + 1] == 1)
                count++;
            if (count >= NeighborThreshold) {
                Cells[i] = 1;
            }
        }
        Texture.emplace_back();
        GenerateTexture(Texture[it + 1]);
        Sprites.push_back(Sprite(Texture[it + 1]));
    }

    /*GenerateTexture(Texture);
    Sprites.push_back(Sprite());*/
    
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

sf::Sprite CellularAutomata::Sprite(sf::Texture& tex) const {
    sf::Sprite sprite(tex);

    sprite.setOrigin({static_cast<float>(SizeX) / 2, static_cast<float>(SizeY) / 2});
    sprite.setScale({1.f,1.f});
    sprite.setPosition({800.f,200.f});
    return sprite;
}
