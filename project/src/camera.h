#pragma once

#if __cplusplus
extern "C" {
#endif

#include "utils.h"

typedef struct Camera Camera;
typedef struct Player Player;

struct Camera
{
	// You could simplify this to just width/height. I've used left/right so you can have multiple split-screen
	// cameras if you want.
	int left;
	int right;
	int height;

	// Standard field of view, near/far plane.
	float fov;
	float nearPlane;
	float farPlane;

	// How much the camera will respond to mouse movement.
	float sensitivity;

	// For positioning the camera in the world (or rather, moving everything in the world so it appears as though you
	// are positioning the camera).
	float zoom;
	vec3f pos;
	vec2f rot;

	// Determines if the camera will use a perspective or orthographic projection.
	bool perspectiveFlag;

	// So you can attach the camera to the player (in case you have multiple players?), it's almost like this camera
	// was originally written for a split-screen game from last year's I3D assignment...
	struct Player *player;
};

void initCamera(Camera *camera, int left, int right, int height);
void uploadModelview(Camera *camera);
void uploadProjection(Camera *camera);

void updateRotation(Camera *camera, int dx, int dy);
void updateZoom(Camera *camera, int dy);

void attachCameraToPlayer(Camera *camera, Player *player);

vec3f getViewPos(Camera *camera);

#if __cplusplus
}
#endif

