#include "CellularAutomata.h"

#include <complex.h>
#include <SFML/Graphics/Image.hpp>

#include "Globals.h"

CellularAutomata::CellularAutomata(unsigned sizeX, unsigned sizeY, double spawnPercent, unsigned threshold, unsigned iteration) {
    SizeX = sizeX;
    SizeY = sizeY;
    SpawnPercent = spawnPercent;
    Iteration = iteration;
    NeighborThreshold = threshold;

    Cells.resize(Iteration + 1);
    for (unsigned i = 0; i < Iteration + 1; i++) {
        Cells[i].resize(SizeY * SizeX);
        std::memset(Cells[i].data(), 0, SizeX * SizeY * sizeof(int));
    }
}

bool CellularAutomata::Generate() {
    Cells.reserve(Iteration + 1);
    
    for (unsigned i = 0; i < SizeX * SizeY; i++) { // boucle principale
        double rdm = Globals::GetInstance()->GetRandomMgr()->GetInstance(0)->RandDouble(0.f, 1.f);
        if (rdm < SpawnPercent) {
            Cells[0][i] = 1;
        }
    }

    Texture.reserve(Iteration + 1);
    Texture.emplace_back();
    GenerateTexture(Texture[0], 0);
    Sprites.reserve(Iteration + 1);
    Sprites.insert(Sprites.begin(),Sprite(Texture[0]));
    
    for (unsigned it = 0; it < Iteration; it++) {
        for (unsigned i = 0; i < SizeX * SizeY; i++) {
            if (Cells[it][i] == 1) {
                Cells[it + 1][i] = 1;
                continue;
            }

            sf::Vector2<int> pos (i % SizeY, i / SizeY);
            unsigned count = 0;
            for (int x = -1; x < 2; x++) {
                for (int y = -1; y < 2; y++) {
                    if (x == 0 && y == 0) {
                        continue;
                    }
                    
                    int oneD = (pos.y + y) * SizeX + pos.x + x;
                    
                    if (oneD < 0 || oneD >= SizeX * SizeY) {
                        count++;
                        continue;
                    }
                    
                    if (Cells[it][(pos.y + y) * SizeX + pos.x + x] == 1) {
                        count++;
                    }
                }
            }
            
            if (count >= NeighborThreshold) {
                Cells[it + 1][i] = 1;
            }
        }
        Texture.emplace_back();
        GenerateTexture(Texture[it + 1], it + 1);
        Sprites.insert(Sprites.begin() + it + 1,Sprite(Texture[it + 1]));
    }
    
    return false;
}

void CellularAutomata::GenerateTexture(sf::Texture& textures, unsigned iteration) const {
    sf::Image image;
    image.resize({SizeX,SizeY});
    for (unsigned i = 0; i < SizeX * SizeY; i++) {
        image.setPixel({i % SizeY, i / SizeY}, Cells[iteration][i] == 1 ? sf::Color::White : sf::Color::Black);
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
