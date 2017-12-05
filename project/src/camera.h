#pragma once

#if __cplusplus
extern "C" {
#endif

#include "utils.h"

typedef struct Camera Camera;
typedef struct Player Player;

struct Camera
{
	int left;
	int right;
	int height;
	float fov;
	float nearPlane;
	float farPlane;
	float sensitivity;
	float zoom;
	vec3f pos;
	vec2f rot;
	bool perspectiveFlag;
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

