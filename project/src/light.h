#pragma once

#if __cplusplus
extern "C" {
#endif

#include "utils.h"

typedef struct Light Light;

struct Light
{
	// Standard light stuff, just here to simplify things.
	vec4f pos;
	vec4f ambient;
	vec4f diffuse;
	vec4f specular;
};

void initLight(Light *light, vec4f pos, vec4f ambient, vec4f diffuse, vec4f specular);
void renderLight(Light *light);

#if __cplusplus
}
#endif

