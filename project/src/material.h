#pragma once

#if __cplusplus
extern "C" {
#endif

#include "utils.h"

typedef struct Material Material;
typedef struct MaterialContainer MaterialContainer;

struct Material
{
	// Standard material stuff, just here to simplify things.
	vec4f ambient;
	vec4f diffuse;
	vec4f specular;
	float shininess;

	// So the material can be uniquely identified.
	char *name;
};

struct MaterialContainer
{
	// A vector of materials. This isn't strictly necessary but it made things easier for me.
	int capacity;
	int nMaterials;
	Material *materials;
};

void initMaterials(MaterialContainer *container);

void renderMaterial(Material *material);

void dMaterialContainer(MaterialContainer *container);
Material *addMaterial(MaterialContainer *container, const char *name);
Material *getMaterial(MaterialContainer *container, const char *name);

void loadMaterials(MaterialContainer *container, const char *filename);

#if __cplusplus
}
#endif

