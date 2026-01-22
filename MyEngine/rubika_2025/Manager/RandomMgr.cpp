#include "RandomMgr.h"

#include "Procedural/CellularAutomata.h"
#include "Procedural/RandomInstance.h"

void RandomMgr::Init() {
}

void RandomMgr::Shut() {
    instanceMap.clear();
}

RandomMgr::InstanceId RandomMgr::CreateInstance() {
    RandomInstance* instance = new RandomInstance();
    instance->SetSeed(GenerateRandomSeed());
    instanceMap.try_emplace(idValue, instance);
    idValue++;
    return idValue;
}

void RandomMgr::DestroyInstance(InstanceId instanceId) {
    RandomInstance* it = instanceMap.at(instanceId);
    instanceMap.erase(instanceId);
    std::destroy_at(it);
}

RandomInstance* RandomMgr::GetInstance(InstanceId instanceId) const {
    auto it = instanceMap.find(instanceId);
    if (it == instanceMap.cend()) {
        return nullptr;
    }
    return it->second;
}

uint32_t RandomMgr::GenerateRandomSeed() { //Generate once, there is case were we will need to regenerate a seed, but we try to limit it mostly
    std::random_device rd;
    return rd();
}

void RandomMgr::GenerateDrunkardWalk(unsigned sizeX, unsigned sizeY, unsigned iteration, unsigned distance, unsigned spawnNumber) {
    if (instanceMap.empty()) {
        CreateInstance();
    }
    drunk = new DrunkardWalk(sizeX,sizeY,iteration,distance,spawnNumber);
    drunk->Generate();
}

void RandomMgr::GenerateCellularAutomata(unsigned sizeX, unsigned sizeY, double spawnPercent, unsigned threshold,unsigned iteration) {
    if (instanceMap.empty()) {
        CreateInstance();
    }
    cellular = new CellularAutomata(sizeX, sizeY, spawnPercent, threshold, iteration);
    cellular->Generate();
}
