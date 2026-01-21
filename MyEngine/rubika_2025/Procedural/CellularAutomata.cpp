#include "CellularAutomata.h"

CellularAutomata::CellularAutomata(unsigned sizeX, unsigned sizeY, double spawnPercent, unsigned threshold,
    unsigned iteration) {
    SizeX = sizeX;
    SizeY = sizeY;
    SpawnPercent = spawnPercent;
    Iteration = iteration;
    NeighborThreshold = threshold;
}

bool CellularAutomata::Generate() {
    return false;
}

void CellularAutomata::GenerateTexture(sf::Texture& textures) const {
}
