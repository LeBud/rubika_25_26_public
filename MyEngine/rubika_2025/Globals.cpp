#include "Globals.h"

#include "TextureMgr.h"

Globals::Globals() {
}

Globals::~Globals() {
}

void Globals::Init() {
    textureMgr = new TextureMgr();
}

void Globals::Destroy() {
}

TextureMgr* Globals::GetTextureMgr() const {
    return textureMgr;
}
