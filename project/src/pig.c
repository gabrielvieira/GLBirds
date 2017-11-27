#include "pig.h"
#include "mesh.h"
#include "material.h"
#include "gl.h"

void initPig(Pig *pig, MaterialContainer *container, Mesh *mesh, vec3f pos, GLuint tex)
{
	pig->pos = pos;
	pig->mesh = mesh;
	pig->tex = tex;
	pig->material = getMaterial(container, "green");
}

void renderPig(Pig *pig, DebugControls *controls)
{
	glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_COLOR_BUFFER_BIT);
	// glDisable(GL_TEXTURE_2D);

	renderMaterial(pig->material);
	glBindTexture(GL_TEXTURE_2D, pig->tex);
	glPushMatrix();

	glTranslatef(pig->pos.x, pig->pos.y, pig->pos.z);
	glPushMatrix();
	glScalef(0.5f, 0.5f, 0.5f);
	renderMesh(pig->mesh, controls);
	glPopMatrix();

	glPopMatrix();

	glPopAttrib();
}


