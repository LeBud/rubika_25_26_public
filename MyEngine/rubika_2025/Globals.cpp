#include "Globals.h"

#include "GameMgr.h"
#include "TextureMgr.h"

Globals::Globals() {
}

Globals::~Globals() {
}

void Globals::Init() {
    textureMgr = new TextureMgr();
    gameMgr = new GameMgr();
}

void Globals::Destroy() {
}

TextureMgr* Globals::GetTextureMgr() const {
    if (textureMgr != nullptr)
        return textureMgr;

    return nullptr;
}

GameMgr* Globals::GetGameMgr() const {
    if (gameMgr != nullptr)
        return gameMgr;
    
    return nullptr;
}
