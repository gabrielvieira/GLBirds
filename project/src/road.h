#pragma once

#if __cplusplus
extern "C" {
#endif

#include "utils.h"

typedef struct Road Road;
typedef struct Mesh Mesh;
typedef struct Material Material;
typedef struct MaterialContainer MaterialContainer;
typedef struct DebugControls DebugControls;

struct Road
{
	vec3f min;
	vec3f max;

	vec3f vel;
	vec3f carPos[20];
	
	Mesh *mesh;
	Material *material;
};

void initRoad(Road *road, MaterialContainer *container, Mesh *mesh, vec3f vel, vec3f min, vec3f max);
void renderRoad(Road *road, DebugControls *controls);

void updateRoad(Road *road, float dt);

#if __cplusplus
}
#endif

