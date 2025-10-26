#include "raylib.h"
#include <map>

#ifndef TEXTURE_PROVIDER
#define TEXTURE_PROVIDER

class TextureProvider
{
private:
	std::map<int, Texture2D> textures;

public:
	const Texture2D loadTexture(int id, const char* path);
	void unloadAll();
};

#endif