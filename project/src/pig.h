#pragma once

#if __cplusplus
extern "C" {
#endif

#include "utils.h"
#include "gl.h"

typedef struct Pig Pig;
typedef struct Mesh Mesh;
typedef struct Material Material;
typedef struct MaterialContainer MaterialContainer;

struct Pig
{
	vec3f pos;
	vec3f forward;
	GLuint tex;
	Mesh *mesh;
	Material *material;
};

void initPig(Pig *pig, MaterialContainer *container, Mesh *mesh, vec3f pos, GLuint tex);
void renderPig(Pig *pig);

#if __cplusplus
}
#endif

