#pragma once

// TODO: don't return pointers

#if __cplusplus
extern "C" {
#endif

#include "utils.h"
#include "gl.h"

typedef struct Vertex Vertex;
typedef struct Mesh Mesh;
typedef struct DebugControls DebugControls;

struct Vertex
{
	vec3f p;
	vec3f n;
	vec2f t;
	vec3f tan;
	vec3f bin;
};

struct Mesh
{
	size_t nVerts;
	Vertex *vertices;

	size_t nIndices;
	int *indices;

	GLuint obj;
};

Mesh *cMesh(size_t nVerts, size_t nIndices);
void dMesh(Mesh *mesh);

void calcTangents(Mesh *mesh);
void calcNormals(Mesh *mesh);
Mesh *makeSphere();
Mesh *makeEllipsoid(float a, float b, float c);
Mesh *makeGrid(float width, float height, size_t rows, size_t cols);

void renderMesh(Mesh *mesh);

#if __cplusplus
}
#endif

