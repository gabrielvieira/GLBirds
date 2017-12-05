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
	globals->collision = 0;

	initMaterials(&globals->materials);

	GLuint grassTexture = loadTexture("../data/grass.jpg");
	initTerrain(&globals->terrain, &globals->materials, cVec3f(0, 0, 0), 100, 100, 8, 4,grassTexture);

	GLuint birdTexture = loadTexture("../data/RedBird.png");
	//
	initPlayer(&globals->player, &globals->materials, globals->sphere, cVec3f(0, 0.5, 0), 90, 0, 1,birdTexture);
	initParabola(&globals->parabola, &globals->player);
	attachCameraToPlayer(&globals->camera, &globals->player);
	GLuint pigTexture = loadTexture("../data/pig.png");
	initPig(&globals->pig,&globals->materials,globals->sphere,cVec3f(randomX, 0.5, randomZ), pigTexture);
	initRendering();
}

void freeGlobals(Globals *globals)
{
	dMesh(globals->sphere);
	dMesh(globals->terrain.mesh);
	dMaterialContainer(&globals->materials);
}

