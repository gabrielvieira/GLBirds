#pragma once

#if __cplusplus
extern "C" {
#endif

#include "utils.h"

typedef struct Material Material;
typedef struct MaterialContainer MaterialContainer;

struct Material
{
	vec4f ambient;
	vec4f diffuse;
	vec4f specular;
	float shininess;
	char *name;
};

struct MaterialContainer
{
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

