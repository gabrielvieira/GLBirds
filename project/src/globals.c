#include "globals.h"
#include "gl.h"
#include <stdlib.h>
#include <time.h>  

void initGlobals(Globals *globals)
{
	  /* initialize random seed: */
  	srand (time(NULL));

  /* generate secret number between 1 and 10: */
  	int randomX = rand() % 100 + 1;
	int randomZ = rand() % 100 + 1;
	
	if (randomX > 50) {
		randomX = randomX - 100;
	}

	if (randomZ > 50) {
		randomZ = randomZ - 100;
	}

	initPlayerControls(&globals->playerControls);
	initCameraControls(&globals->cameraControls);
	initDebugControls(&globals->debugControls);

	globals->sphere = makeSphere();
	globals->cube = makeCube();
	globals->cylinder = makeCylinder();
	globals->collision = 0;

	initMaterials(&globals->materials);
	initLight(&globals->light, cVec4f(100, 100, -100, 0), cVec4f(0.2f, 0.2f, 0.2f, 1), cVec4f(1, 1, 1, 1), cVec4f(0.1f, 0.1f, 0.1f, 1));

	GLuint grassTexture = loadTexture("../data/grass.jpg");
	//void initTerrain(Terrain *terrain, MaterialContainer *container, vec3f pos, float width, float height, size_t rows, size_t cols);
	initTerrain(&globals->terrain, &globals->materials, cVec3f(0, 0, 0), 100, 100, 8, 4,grassTexture);

	//
	initPlayer(&globals->player, &globals->materials, globals->sphere, cVec3f(0, 0.5, 0), 90, 0, 1);
	initParabola(&globals->parabola, &globals->player);
	attachCameraToPlayer(&globals->camera, &globals->player);

	// You could add this to the material if you prefer.
	// GLuint logTexture = loadTexture("../data/wood.jpg");
	//void initPig(Pig *pig, MaterialContainer *container, Mesh *mesh, vec3f pos);
	initPig(&globals->pig,&globals->materials,globals->sphere,cVec3f(randomX, 0.5, randomZ));

	initRoad(&globals->road, &globals->materials, globals->cube, cVec3f(4, 0, 0), cVec3f(-50, 0, -20), cVec3f(50, 0, 0));
	// initRiver(&globals->river, &globals->materials, globals->cylinder, logTexture, cVec3f(4, 0, 0), cVec3f(-50, 0, 10), cVec3f(50, 0, 35));

	initRendering();
}

void freeGlobals(Globals *globals)
{
	// Hopefully I didn't forget to free anything, although it doesn't matter that much since this will only be done
	// when the program exits, and the memory will be freed at that point automatically whether this function is called
	// or not.
	dMesh(globals->sphere);
	dMesh(globals->cube);
	dMesh(globals->cylinder);
	dMesh(globals->terrain.mesh);

	dMaterialContainer(&globals->materials);
}

