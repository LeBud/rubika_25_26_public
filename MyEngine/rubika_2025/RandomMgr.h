#pragma once
#include <cstdint>
#include <random>

class RandomMgr {
public:
    void Init();
    void Shut();

    unsigned RandUInt(unsigned min, unsigned max) const;
    int RandInt32(int min, int max) const;
    double RandDouble(double min, double max) const;
    double RandNormalDouble(double center, double disp) const;

    void SetSeed(uint32_t seed);
    uint32_t GetSeed() const;

    static uint32_t GenerateRandomSeed();

private:
    std::default_random_engine RandomEng;
    uint32_t Seed;
};
