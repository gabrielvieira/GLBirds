#include "road.h"
#include "mesh.h"
#include "controls.h"
#include "material.h"
#include "utils.h"
#include "gl.h"

// This is all pretty much the same as the river. I've split it into two separate files since they'll each function
// slightly differently in assignment 3.

void initRoad(Road *road, MaterialContainer *container, Mesh *mesh, vec3f vel, vec3f min, vec3f max)
{
	road->min = min;
	road->max = max;
	road->mesh = mesh;
	road->vel = vel;
	road->material = getMaterial(container, "car");

	for (size_t i = 0; i < 20; i++)
		road->carPos[i] = cVec3f(getTRand(min.x, max.x), 0, getTRand(min.z, max.z));
}

void renderRoad(Road *road, DebugControls *controls)
{
	glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_COLOR_BUFFER_BIT);
	glDisable(GL_TEXTURE_2D);

	renderMaterial(road->material);

	for (size_t i = 0; i < 20; i++)
	{
		glPushMatrix();
		glTranslatef(road->carPos[i].x, road->carPos[i].y, road->carPos[i].z);

		glPushMatrix();
		glScalef(0.5f, 0.5f, 0.5f);
		renderMesh(road->mesh, controls);
		glPopMatrix();

		glPopMatrix();
	}

	glPopAttrib();
}

void updateRoad(Road *road, float dt)
{
	for (size_t i = 0; i < 20; i++)
	{
		road->carPos[i].y += road->vel.y * dt;
		road->carPos[i].x += road->vel.x * dt;
		road->carPos[i].z += road->vel.z * dt;

		if (road->carPos[i].x > road->max.x)
			road->carPos[i].x = road->min.x;
		if (road->carPos[i].y > road->max.y)
			road->carPos[i].y = road->min.y;
		if (road->carPos[i].z > road->max.z)
			road->carPos[i].z = road->min.z;
	}
}

