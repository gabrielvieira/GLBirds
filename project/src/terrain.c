#include "terrain.h"
#include "mesh.h"
#include "material.h"
#include "gl.h"

void initTerrain(Terrain *terrain, MaterialContainer *container, vec3f pos, float width, float height, size_t rows, size_t cols, GLuint tex)
{
	// It's pretty debateable whether this should have its own file. I wouldn't necessarily say you're wrong if you
	// put this stuff in globals.
	terrain->pos = pos;
	terrain->width = width;
	terrain->height = height;
	terrain->rows = rows;
	terrain->cols = cols;
	terrain->tex = tex;

	terrain->mesh = makeGrid(width, height, rows, cols);
	terrain->material = getMaterial(container, "terrain");
}

void renderTerrain(Terrain *terrain, DebugControls *controls)
{
	glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_COLOR_BUFFER_BIT);

	renderMaterial(terrain->material);
	glBindTexture(GL_TEXTURE_2D, terrain->tex);

	glPushMatrix();

	glTranslatef(terrain->pos.x, terrain->pos.y, terrain->pos.z);
	renderMesh(terrain->mesh, controls);

	glPopMatrix();

	glPopAttrib();
}

