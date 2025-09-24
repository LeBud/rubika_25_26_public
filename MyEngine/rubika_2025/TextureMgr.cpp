#include "TextureMgr.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

#include "Logger.h"

TextureMgr::TextureMgr() {
}

TextureMgr::~TextureMgr() {
    textures.clear();
}

bool TextureMgr::LoadTexture(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        Logger::Error("TextureMgr::LoadTexture: File doesn't exist ! : " + path.string());
        return false;
    }
    
    sf::Texture texture;
    
    if (!texture.loadFromFile(path.string())) {
        Logger::Error("TextureMgr::LoadTexture: Failed to load texture ! : " + path.string());
        return false;
    }
    
    std::string filename = path.stem().string();
    sTextureData data;
    data.texture = std::move(texture);
    //data.name = filename;
    //data.filepath = path.string();

    textures[filename] = std::move(data);
    Logger::Info("TextureMgr::LoadTexture: Loaded : " + filename);
    
    return true;
}

void TextureMgr::GetXMLFile(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path))
        return;

    std::string filename = path.string();
    
    rapidxml::file<> xmlFile(filename.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());

    auto parentNode = doc.first_node();
    auto childNode = parentNode->first_node("Animation");

    for (rapidxml::xml_node<> *node = childNode->first_node(); //défile a travers chaque node du même "niveau"
     node; node = node->next_sibling()){
        std::cout << "Node Name " << node->name() << " ";
        std::cout << "with value " << node->value() << "\n" << std::endl;
    }
}

const sTextureData& TextureMgr::GetTextureData(const std::string& name) const {
    const auto& it = textures.find(name);
    if (it == textures.end()) {
        Logger::Error("TextureMgr::GetTextureData: Texture not found ! " + name);

        sf::Image img({1,1}, sf::Color::Magenta);
        sf::Texture blankText;
        blankText.loadFromImage(img);

        const auto& blank = textures.find("Blank");
        if (blank == textures.end()) {
            sTextureData blankData;
            //blankData.name = "Blank";
            blankData.texture = blankText;

            return blankData; //c'est du code de merde
        }
        
    }

    return it->second;
}
