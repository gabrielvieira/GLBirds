#pragma once

#if __cplusplus
extern "C" {
#endif

#include "utils.h"
#include "gl.h"

typedef struct Terrain Terrain;

typedef struct Mesh Mesh;
typedef struct Material Material;
typedef struct MaterialContainer MaterialContainer;
typedef struct DebugControls DebugControls;

struct Terrain
{
	vec3f pos;

	float width;
	float height;

	size_t rows;
	size_t cols;

	GLuint tex;

	Mesh *mesh;
	Material *material;
};

void initTerrain(Terrain *terrain, MaterialContainer *container, vec3f pos, float width, float height, size_t rows, size_t cols, GLuint tex);

void renderTerrain(Terrain *terrain);

#if __cplusplus
}
#endif

