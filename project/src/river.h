#pragma once

#if __cplusplus
extern "C" {
#endif

#include "utils.h"
#include "gl.h"

typedef struct River River;
typedef struct Mesh Mesh;
typedef struct Material Material;
typedef struct MaterialContainer MaterialContainer;
typedef struct DebugControls DebugControls;

struct River
{
	vec3f min;
	vec3f max;
	
	vec3f vel;

	// I hard coded the number of logs, probably not the best idea.
	vec3f logPos[20];
	
	GLuint tex;
	
	Mesh *mesh;
	Material *material;
};

void initRiver(River *river, MaterialContainer *container, Mesh *mesh, GLuint tex, vec3f vel, vec3f min, vec3f max);
void renderRiver(River *river, DebugControls *controls);

void updateRiver(River *river, float dt);

#if __cplusplus
}
#endif

