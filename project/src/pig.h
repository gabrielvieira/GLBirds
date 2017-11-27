#pragma once

#if __cplusplus
extern "C" {
#endif

#include "utils.h"
#include "gl.h"

typedef struct Pig Pig;
typedef struct DebugControls DebugControls;
typedef struct Mesh Mesh;
typedef struct Material Material;
typedef struct MaterialContainer MaterialContainer;

struct Pig
{
	// These are current position/velocity, the player does not store initial values for these. Heading represents
	// rotation of the player around the y axis.
	vec3f pos;
	// Current forward vector of the player, which is a combination of the jumpRot/jumpSpeed and the heading.
	vec3f forward;
	GLuint tex;
	// Now we have a mesh and a material that is used to render the player.
	Mesh *mesh;
	Material *material;
};

void initPig(Pig *pig, MaterialContainer *container, Mesh *mesh, vec3f pos, GLuint tex);
void renderPig(Pig *pig, DebugControls *controls);

#if __cplusplus
}
#endif

