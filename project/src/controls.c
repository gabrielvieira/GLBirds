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
	controls->lightingFlag = true;
	controls->wireframeFlag = false;
	controls->textureFlag = true;
	controls->normalFlag = false;
	controls->axisFlag = false;
}

void toggleWireframe(DebugControls *controls)
{
	if (controls->wireframeFlag)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void toggleLighting(DebugControls *controls)
{
	if (controls->lightingFlag)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
}

void toggleTexturing(DebugControls *controls)
{
	if (controls->textureFlag)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
}

