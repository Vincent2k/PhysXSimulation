#include "TextureProvider.h"
#include "raylib.h"

const Texture2D TextureProvider::loadTexture(int id, const char* path)
{
	if (!this->textures.contains(id))
	{
		Texture2D newTexture = LoadTexture(path);
		this->textures.insert(std::pair{ id, newTexture });
	}

	return this->textures.at(id);
}

void TextureProvider::unloadAll()
{
	for (auto const& texture : this->textures)
	{
		UnloadTexture(texture.second);
	}

	this->textures.clear();
}