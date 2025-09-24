#pragma once

#include <filesystem>
#include <SFML/Graphics/Texture.hpp>
#include <unordered_map>

struct sAnimationData {

	int StartX;
	int StartY;
	//etc...
	
};

/* Struct that contains any data linked to a texture.
 * From the texture itself to its metadata.
 */ 
struct sTextureData
{
	sf::Texture texture;
	std::unordered_map<std::string, sAnimationData> animations;
};

class TextureMgr final
{
public:

	TextureMgr();
	~TextureMgr();
	
	std::unordered_map<std::string, sTextureData> textures;
	
	/*
	 * Loads the texture using the given path, including its metadata.
	 * Obviously, this function will also store the loaded data
	 * If errors occur during the loading process, this function must return false.
	 * Return true otherwise.
	*/
	bool LoadTexture(const std::filesystem::path& path);

	void GetXMLFile(const std::filesystem::path& path);
	
	/*
	 * Returns the corresponding TextureData using the name of the texture
	 */
	const sTextureData& GetTextureData(const std::string& name) const;
};
