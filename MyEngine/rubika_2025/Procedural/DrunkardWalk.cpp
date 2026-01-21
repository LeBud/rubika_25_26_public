#include "DrunkardWalk.h"

#include <SFML/Graphics/Image.hpp>

#include "Globals.h"

DrunkardWalk::DrunkardWalk(unsigned sizeX, unsigned sizeY, unsigned iteration, unsigned distance,
                           unsigned spawnNumber) {
    SizeX = sizeX;
    SizeY = sizeY;
    Iteration = iteration;
    Distance = distance;
    SpawnNumber = spawnNumber;
    
    Cells.resize(SizeX * SizeY);
    
    std::memset(Cells.data(), 0, SizeX * SizeY * sizeof(int));
}

bool DrunkardWalk::Generate() {
    for (unsigned i = 0; i < Iteration; i++) {
        sf::Vector2<unsigned> originalPos(
            Globals::GetInstance()->GetRandomMgr()->GetInstance(0)->RandUInt(0,SizeX-1),
            Globals::GetInstance()->GetRandomMgr()->GetInstance(0)->RandUInt(0,SizeY-1));
        
        for (unsigned i = 0; i < SpawnNumber; i++) {
            sf::Vector2<unsigned> pos = originalPos;
            unsigned cell = pos.y * SizeY + originalPos.x;
            Cells.at(cell) = 1;

            for (unsigned i = 0; i < Distance; i++) {
                unsigned dir = Globals::GetInstance()->GetRandomMgr()->GetInstance(0)->RandUInt(0,3);
                switch (dir) {
                    case 0:
                        pos.y++;
                    break;
                    case 1:
                        pos.x++;
                    break;
                    case 2:
                        pos.y--;
                    break;
                    case 3:
                        pos.x--;
                    break;
                    default:
                        return false;
                }
                if (pos.y >= SizeY) {
                    Distance--;
                    continue;
                }
                if (pos.x >= SizeX) {
                    Distance--;
                    continue;
                }
                cell = pos.y * SizeY + pos.x;
                Cells[cell] = 1;
            }
        }
    }

    GenerateTexture(Texture);
    Generated = true;
    return true;
}

void DrunkardWalk::GenerateTexture(sf::Texture& texture) const {
    //std::vector<sf::> pixels;
    sf::Image image;
    image.resize({SizeX,SizeY});
    for (unsigned i = 0; i < SizeX * SizeY; i++) {
        image.setPixel({i % SizeY, i / SizeY}, Cells[i] == 1 ? sf::Color::White : sf::Color::Red);
    }
    texture.resize({SizeX,SizeY});
    texture.loadFromImage(image);
    texture.update(image);
}

sf::Sprite DrunkardWalk::Sprite() {
    sf::Sprite sprite(Texture);

    sprite.setOrigin({static_cast<float>(SizeX) / 2, static_cast<float>(SizeY) / 2});
    sprite.setScale({1.f,1.f});
    sprite.setPosition({800.f,200.f});
    return sprite;
}
