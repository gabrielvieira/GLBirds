#include "controls.h"
#include "gl.h"

void initPlayerControls(PlayerControls *controls)
{
	controls->up = false;
	controls->down = false;
	controls->left = false;
	controls->right = false;
	controls->upSpeed = false;
	controls->downSpeed = false;
	controls->upRot = false;
	controls->downRot = false;
}

void initCameraControls(CameraControls *controls)
{
	controls->rotating = false;
	controls->zooming = false;
}

void initDebugControls(DebugControls *controls)
{
	controls->wireframeFlag = false;
	controls->textureFlag = true;
}
