#pragma once

#if __cplusplus
extern "C" {
#endif

#if _WIN32
#define bool char
#define true 1
#define false 0
#else
#include <stdbool.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#ifndef _WIN32
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#define clamp(x, a, b) min(max((x), (a)), (b))

#ifndef M_PI
#define M_PI 3.141592653589793238462643
#endif

#define UNUSED(x) (void)(x)

typedef struct DebugControls DebugControls;

typedef struct vec2f vec2f;
typedef struct vec3f vec3f;
typedef struct vec4f vec4f;

float getRand();
float getNRand();
float getTRand(float min, float max);

struct vec2f
{
	float x, y;
};

vec2f cVec2f(float x, float y);
vec2f addVec2f(vec2f v1, vec2f v2);
vec2f subVec2f(vec2f v1, vec2f v2);
vec2f mulVec2f(vec2f v1, float f);
vec2f divVec2f(vec2f v1, float f);
float dotVec2f(vec2f v1, vec2f v2);
float lenSqVec2f(vec2f v);
float lenVec2f(vec2f v);
vec2f normaliseVec2f(vec2f v);


struct vec3f
{
	float x, y, z;
};

vec3f cVec3f(float x, float y, float z);
vec3f addVec3f(vec3f v1, vec3f v2);
vec3f subVec3f(vec3f v1, vec3f v2);
vec3f mulVec3f(vec3f v1, float f);
vec3f divVec3f(vec3f v1, float f);
float dotVec3f(vec3f v1, vec3f v2);
float lenSqVec3f(vec3f v);
float lenVec3f(vec3f v);
vec3f normaliseVec3f(vec3f v);


struct vec4f
{
	float x, y, z, w;
};

vec4f cVec4f(float x, float y, float z, float w);
vec4f addVec4f(vec4f v1, vec4f v2);
vec4f subVec4f(vec4f v1, vec4f v2);
vec4f mulVec4f(vec4f v1, float f);
vec4f divVec4f(vec4f v1, float f);
float dotVec4f(vec4f v1, vec4f v2);
float lenSqVec4f(vec4f v);
float lenVec4f(vec4f v);
vec4f normaliseVec4f(vec4f v);

vec3f cross(vec3f v, vec3f u);

#if __cplusplus
}
#endif

