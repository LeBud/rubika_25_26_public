#include "DrunkardWalk.h"

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
    sf::Vector2<unsigned>* pos = new sf::Vector2<unsigned>(
        Globals::GetInstance()->GetRandomMgr()->GetInstance(0)->RandUInt(0,SizeX-1),
        Globals::GetInstance()->GetRandomMgr()->GetInstance(0)->RandUInt(0,SizeY-1));

    unsigned cell = pos->y * SizeY + pos->x;
    Cells[cell] = 1;

    for (unsigned i = 0; i < Iteration; i++) {
        unsigned dir = Globals::GetInstance()->GetRandomMgr()->GetInstance(0)->RandUInt(0,3);
        switch (dir) {
            case 0:
                pos->y++;
            break;
            case 1:
                pos->x++;
            break;
            case 2:
                pos->y--;
            break;
            case 3:
                pos->x--;
            break;
            default:
                return false;
        }
        cell = pos->y * SizeY + pos->x;
        Cells[cell] = 1;
    }
    
    return true;
}

void DrunkardWalk::GenerateTexture(sf::Texture& texture) const {
}
