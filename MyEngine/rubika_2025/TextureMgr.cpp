#include "TextureMgr.h"
#include <SFML/Graphics.hpp>

#include "Logger.h"

TextureMgr::TextureMgr() {
}

TextureMgr::~TextureMgr() {
}

bool TextureMgr::LoadTexture(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        Logger::Error("TextureMgr::LoadTexture: File doesn't exist ! : " + path.string());
        return false;
    }

    sf::Texture texture;
    
    if (!sf::Texture().loadFromFile(path.string())) {
        Logger::Error("TextureMgr::LoadTexture: Failed to load texture ! : " + path.string());
        return false;
    }

    std::string filename = path.stem().string();
    sTextureData data;
    data.texture = std::move(texture);
    data.name = filename;
    data.filepath = path.string();

    textures[filename] = std::move(data);
    Logger::Info("TextureMgr::LoadTexture: Loaded : " + filename);
    
    return true;
}

const sTextureData& TextureMgr::GetTextureData(const std::string& name) const {
    auto it = textures.find(name);
    if (it == textures.end()) {
        Logger::Error("TextureMgr::GetTextureData: Texture not found ! " + name);
        throw std::runtime_error("Texture not found !");
    }

    return it->second;
}
