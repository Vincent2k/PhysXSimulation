#include "ModelProvider.h"

#define SUN_TEXTURE_ID 1
#define SUN_MATERIAL_ID 1

#define PLANET_TEXTURE_ID 2
#define PLANET_MATERIAL_ID 2

#define RINGS 16
#define SLICES 16

Model ModelProvider::loadSun(int id, float radius)
{
	return this->loadPlanet(id, radius, "resources/sun.png", SUN_TEXTURE_ID, SUN_MATERIAL_ID);
}

Model ModelProvider::loadPlanet(int id, float radius)
{
	return this->loadPlanet(id, radius, "resources/planet.png", PLANET_TEXTURE_ID, PLANET_MATERIAL_ID);
}

Model ModelProvider::loadPlanet(int id, float radius, const char* path, int textureId, int materialId)
{
	if (!this->models.contains(id))
	{
		auto planetTexture = this->textureProvider.loadTexture(textureId, path);
		auto planetMaterial = this->createMaterialIfNotExist(materialId, planetTexture);

		int rings = 16;
		int slices = 16;
		Model sunModel = LoadModelFromMesh(GenMeshSphere(radius, RINGS, SLICES));
		sunModel.materials[0] = planetMaterial;

		this->models.insert(std::pair{ id, sunModel });
	}

	return this->models.at(id);
}

Material ModelProvider::createMaterialIfNotExist(int id, const Texture2D texture)
{
	if (!this->textureMaterials.contains(id))
	{
		Material material = LoadMaterialDefault();
		material.maps[MATERIAL_MAP_ALBEDO].texture = texture;

		this->textureMaterials.insert(std::pair{ id, material });
	}

	return this->textureMaterials.at(id);
}

void ModelProvider::unloadAll()
{
	for (auto &models : this->models)
	{
		models.second.materials[0].maps = NULL;
		UnloadModel(models.second);
	}

	this->models.clear();

	for (auto& material : this->textureMaterials)
	{
		UnloadMaterial(material.second);
	}

	this->textureProvider.unloadAll();

	this->textureMaterials.clear();
}
