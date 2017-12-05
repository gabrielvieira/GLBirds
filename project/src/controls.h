#pragma once

#if __cplusplus
extern "C" {
#endif

#include "utils.h"

typedef struct PlayerControls PlayerControls;
typedef struct CameraControls CameraControls;
typedef struct DebugControls DebugControls;

struct PlayerControls
{
	bool up;
	bool down;
	bool left;
	bool right;
	bool upSpeed;
	bool downSpeed;
	bool upRot;
	bool downRot;
};

void initPlayerControls(PlayerControls *controls);


struct CameraControls
{
	bool rotating;
	bool zooming;
};

void initCameraControls(CameraControls *controls);


struct DebugControls
{
	bool wireframeFlag;
	bool textureFlag;
};

void initDebugControls(DebugControls *controls);

#if __cplusplus
}
#endif

