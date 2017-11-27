#include "river.h"
#include "mesh.h"
#include "controls.h"
#include "material.h"
#include "gl.h"
#include "utils.h"

void initRiver(River *river, MaterialContainer *container, Mesh *mesh, GLuint tex, vec3f vel, vec3f min, vec3f max)
{
	river->min = min;
	river->max = max;
	river->mesh = mesh;
	river->tex = tex;
	river->vel = vel;
	river->material = getMaterial(container, "white");
	
	// Generate logs at random positions within min/max, don't use the y value. If you wanted to be a little fancier
	// You could restrict the logs to 'lanes' and have different lanes move in different directions.
	for (size_t i = 0; i < 20; i++)
		river->logPos[i] = cVec3f(getTRand(min.x, max.x), 0, getTRand(min.z, max.z));
}

void renderRiver(River *river, DebugControls *controls)
{
	glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);

	renderMaterial(river->material);
	
	glBindTexture(GL_TEXTURE_2D, river->tex);
	
	for (size_t i = 0; i < 20; i++)
	{
		glPushMatrix();
		glTranslatef(river->logPos[i].x, river->logPos[i].y, river->logPos[i].z);
		glRotatef(90, 0, 1, 0);

		glPushMatrix();
		glScalef(1, 1, 10);
		renderMesh(river->mesh, controls);
		glPopMatrix();

		glPopMatrix();
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopAttrib();
}

void updateRiver(River *river, float dt)
{
	// Standard numerical integration. When a log reaches the end of the river, reset its position.
	for (size_t i = 0; i < 20; i++)
	{
		river->logPos[i].y += river->vel.y * dt;
		river->logPos[i].x += river->vel.x * dt;
		river->logPos[i].z += river->vel.z * dt;

		if (river->logPos[i].x > river->max.x)
			river->logPos[i].x = river->min.x;
		if (river->logPos[i].y > river->max.y)
			river->logPos[i].y = river->min.y;
		if (river->logPos[i].z > river->max.z)
			river->logPos[i].z = river->min.z;
	}
}

