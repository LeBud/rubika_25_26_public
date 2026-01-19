#include "RandomMgr.h"

void RandomMgr::Init() {
    
}

void RandomMgr::Shut() {
}

RandomMgr::InstanceId RandomMgr::CreateInstance() {
    
}

void RandomMgr::DestroyInstance(InstanceId instanceId) {
    
}

RandomInstance* RandomMgr::GetInstance(InstanceId instanceId) const {
    
}

uint32_t RandomMgr::GenerateRandomSeed() { //Generate once, there is case were we will need to regenerate a seed, but we try to limit it mostly
    std::random_device rd;
    return rd();
}
