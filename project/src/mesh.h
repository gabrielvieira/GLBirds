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
	// Almost everything you need in a vertex (and some things you don't): position, normal, texture coord, tangent,
	// binormal. The only thing not here that you might want to add would be colour.
	vec3f p;
	vec3f n;
	vec2f t;
	vec3f tan;
	vec3f bin;
};

struct Mesh
{
	// Mesh data is stored in arrays, separated between vertices and incdices.
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

// A few different procedural meshes for you to play around with.
Mesh *makeSquare();
Mesh *makeCube();
Mesh *makeCircle();
Mesh *makeCylinder();
Mesh *makeSphere();
Mesh *makeEllipsoid(float a, float b, float c);
Mesh *makeGrid(float width, float height, size_t rows, size_t cols);

void renderMesh(Mesh *mesh, DebugControls *controls);

#if __cplusplus
}
#endif

