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
    return textureMgr;
}
