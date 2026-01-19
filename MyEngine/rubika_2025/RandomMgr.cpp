#include "RandomMgr.h"

void RandomMgr::Init() {
    SetSeed(GenerateRandomSeed());
}

void RandomMgr::Shut() {
}

unsigned RandomMgr::RandUInt(unsigned min, unsigned max) const {
    std::uniform_int_distribution<unsigned> dist(min, max);

    return dist(RandomEng);
}

int RandomMgr::RandInt32(int min, int max) const {
    std::uniform_int_distribution<int> dist(min, max);

    return dist(RandomEng);
}

double RandomMgr::RandDouble(double min, double max) const {
    std::uniform_real_distribution<double> dist(min, max);

    return dist(RandomEng);
}

double RandomMgr::RandNormalDouble(double center, double disp) const {
    std::normal_distribution<double> dist(center, disp);

    return dist(RandomEng);
}

void RandomMgr::SetSeed(uint32_t seed) {
    Seed = seed;
    RandomEng.seed(seed);
}

uint32_t RandomMgr::GetSeed() const {
    return Seed;
}

uint32_t RandomMgr::GenerateRandomSeed() { //Generate once, there is case were we will need to regenerate a seed, but we try to limit it mostly
    std::random_device rd;
    return rd();
}
