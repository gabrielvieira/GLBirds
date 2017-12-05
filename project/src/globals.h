#pragma once

#if __cplusplus
extern "C" {
#endif

#include "controls.h"
#include "parabola.h"
#include "player.h"
#include "terrain.h"
#include "camera.h"
#include "material.h"
#include "mesh.h"
#include "utils.h"
#include "pig.h"

typedef struct Globals Globals;


struct Globals
{
	Camera camera;
	Terrain terrain;
	PlayerControls playerControls;
	CameraControls cameraControls;
	DebugControls debugControls;
	MaterialContainer materials;
	Player player;
	Pig pig;
	Parabola parabola;
	Mesh *sphere;
	int   collision;
};

void initGlobals(Globals *globals);

void freeGlobals(Globals *globals);

#if __cplusplus
}
#endif

