#include "raylib.h"
#include "TextureProvider.h"
#include <map> 

#ifndef MODEL_PROVIDER
#define MODEL_PROVIDER

class ModelProvider
{
private: 
	std::map<int, Material> textureMaterials;
	std::map<int, Model> models;
	TextureProvider textureProvider;

	Material createMaterialIfNotExist(int id, const Texture2D texture);

public:
	Model loadSun(int id, float radius);
	Model loadPlanet(int id, float radius);
	Model loadPlanet(int id, float radius, const char* path, int textureId, int materialId);

	void unloadAll();
};

#endif // !MODEL_PROVIDER