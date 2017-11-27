#include "utils.h"
#include "gl.h"
#include "controls.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

float getRand()
{
	return (float) rand() / (float) RAND_MAX;
}

float getNRand()
{
	return getRand() * 2.0f - 1.0f;
}

float getTRand(float min, float max)
{
	return getRand() * (max - min) + min;
}



// If you're using C++ for your vector math then lucky you... also I hate you.
vec2f cVec2f(float x, float y)
{
	vec2f v;
	v.x = x;
	v.y = y;
	return v;
}

vec2f addVec2f(vec2f v1, vec2f v2)
{
	vec2f v;
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	return v;
}

vec2f subVec2f(vec2f v1, vec2f v2)
{
	vec2f v;
	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	return v;
}

vec2f mulVec2f(vec2f v1, float f)
{
	vec2f v;
	v.x = v1.x * f;
	v.y = v1.y * f;
	return v;
}

vec2f divVec2f(vec2f v1, float f)
{
	vec2f v;
	v.x = v1.x / f;
	v.y = v1.y / f;
	return v;
}

float dotVec2f(vec2f v1, vec2f v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

float lenSqVec2f(vec2f v)
{
	return v.x * v.x + v.y * v.y;
}

float lenVec2f(vec2f v)
{
	return sqrtf(lenSqVec2f(v));
}

vec2f normaliseVec2f(vec2f v)
{
	float l = lenVec2f(v);
	if (l == 0)
		return v;
	v.x /= l;
	v.y /= l;
	return v;
}




vec3f cVec3f(float x, float y, float z)
{
	vec3f v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

vec3f addVec3f(vec3f v1, vec3f v2)
{
	vec3f v;
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;
	return v;
}

vec3f subVec3f(vec3f v1, vec3f v2)
{
	vec3f v;
	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	v.z = v1.z - v2.z;
	return v;
}

vec3f mulVec3f(vec3f v1, float f)
{
	vec3f v;
	v.x = v1.x * f;
	v.y = v1.y * f;
	v.z = v1.z * f;
	return v;
}

vec3f divVec3f(vec3f v1, float f)
{
	vec3f v;
	v.x = v1.x / f;
	v.y = v1.y / f;
	v.z = v1.z / f;
	return v;
}

float dotVec3f(vec3f v1, vec3f v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float lenSqVec3f(vec3f v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

float lenVec3f(vec3f v)
{
	return sqrtf(lenSqVec3f(v));
}

vec3f normaliseVec3f(vec3f v)
{
	float l = lenVec3f(v);
	if (l == 0)
		return v;
	v.x /= l;
	v.y /= l;
	v.z /= l;
	return v;
}




vec4f cVec4f(float x, float y, float z, float w)
{
	vec4f v;
	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;
	return v;
}

vec4f addVec4f(vec4f v1, vec4f v2)
{
	vec4f v;
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;
	v.w = v1.w + v2.w;
	return v;
}

vec4f subVec4f(vec4f v1, vec4f v2)
{
	vec4f v;
	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	v.z = v1.z - v2.z;
	v.w = v1.w - v2.w;
	return v;
}

vec4f mulVec4f(vec4f v1, float f)
{
	vec4f v;
	v.x = v1.x * f;
	v.y = v1.y * f;
	v.z = v1.z * f;
	v.w = v1.w * f;
	return v;
}

vec4f divVec4f(vec4f v1, float f)
{
	vec4f v;
	v.x = v1.x / f;
	v.y = v1.y / f;
	v.z = v1.z / f;
	v.w = v1.w / f;
	return v;
}

float dotVec4f(vec4f v1, vec4f v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

float lenSqVec4f(vec4f v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

float lenVec4f(vec4f v)
{
	return sqrtf(lenSqVec4f(v));
}

vec4f normaliseVec4f(vec4f v)
{
	float l = lenVec4f(v);
	if (l == 0)
		return v;
	v.x /= l;
	v.y /= l;
	v.z /= l;
	v.w /= l;
	return v;
}



vec3f cross(vec3f v, vec3f u)
{
	vec3f t;
	t.x = v.y * u.z - v.z * u.y;
	t.y = v.z * u.x - v.x * u.z;
	t.z = v.x * u.y - v.y * u.x;
	return t;
}

