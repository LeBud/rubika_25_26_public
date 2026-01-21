#pragma once
#include <cstdint>
#include <unordered_map>

#include "Procedural/RandomInstance.h"

class RandomMgr {
public:
    //Dans l'idée je créer avec ces fonctions mes RandomInstance
    //Et je les stock dans une unordered_map
    //Mais CreateInstance, je return quoi ?
    
    using InstanceId = unsigned;

    void Init();
    void Shut();

    InstanceId CreateInstance();
    void DestroyInstance(InstanceId instanceId);
    RandomInstance* GetInstance(InstanceId instanceId) const;

    static uint32_t GenerateRandomSeed();

    InstanceId idValue = 0;
    std::unordered_map<InstanceId, RandomInstance*> instanceMap;
};
