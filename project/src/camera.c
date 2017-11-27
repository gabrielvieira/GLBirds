#include "camera.h"
#include "player.h"
#include "gl.h"
#include "utils.h"

void initCamera(Camera *camera, int left, int right, int height)
{
	// Whole bunch of initial values, feel free to pass some of these in as arguments, I couldn't be bothered.
	camera->left = left;
	camera->right = right;
	camera->height = height;
	camera->fov = 75.0f;
	camera->nearPlane = 0.1f;
	camera->farPlane = 100.0f;
	camera->sensitivity = 0.1f;
	camera->zoom = 10.0f;
	camera->perspectiveFlag = true;
	camera->pos = cVec3f(0, 0, 0);
	camera->rot = cVec2f(45, 180);
	camera->player = 0;

	uploadProjection(camera);
}

void uploadModelview(Camera *camera)
{
	// Uploads the camera's tansforms to the modelview matrix. Call this before doing any other rendering and you
	// can position your geometry in the world without having to worry about the camera at all.
	glLoadIdentity();
	if (camera->zoom > 0)
	{
		// Translation won't work if this isn't in perspective mode, in which case scale needs to be used instead.
		if (camera->perspectiveFlag)
			glTranslatef(0, 0, -camera->zoom);
		else
			glScalef(1.0f / camera->zoom, 1.0f / camera->zoom, 1.0f);
	}
	glTranslatef(-camera->pos.x, -camera->pos.y, -camera->pos.z);
	glRotatef(camera->rot.x, 1, 0, 0);
	glRotatef(camera->rot.y, 0, 1, 0);
	if (camera->player)
		glTranslatef(-camera->player->pos.x, -camera->player->pos.y, -camera->player->pos.z);
}

void uploadProjection(Camera *camera)
{
	// Fairly standard projection matrix, can be either perspective or orthographic.
	float aspect = (float) (camera->right - camera->left) / (float) camera->height;

	glViewport(camera->left, 0, camera->right - camera->left, camera->height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (camera->perspectiveFlag)
		gluPerspective(camera->fov, aspect, camera->nearPlane, camera->farPlane);
	else
		glOrtho((float) camera->left, (float) camera->right, 0, (float) camera->height, 0, camera->farPlane);
	glMatrixMode(GL_MODELVIEW);
}

void updateRotation(Camera *camera, int dx, int dy)
{
	// Rotates the camera using horizontal and vertical mouse movement.
	camera->rot.x += dx * camera->sensitivity;
	camera->rot.y += dy * camera->sensitivity;
}

void updateZoom(Camera *camera, int dy)
{
	// Zooms the camera using vertical mouse movement.
	camera->zoom -= dy * camera->zoom * camera->sensitivity * 0.1f;
	camera->zoom = clamp(camera->zoom, camera->nearPlane, camera->farPlane / 2.0f);
}

void attachCameraToPlayer(Camera *camera, Player *player)
{
	camera->player = player;
}

vec3f getViewPos(Camera *camera)
{
	// In case you want to get the actual position of the camera in the world, not needed for the assignment.
	vec3f pos = camera->pos;
	if (camera->player)
		pos = addVec3f(pos, camera->player->pos);

	pos.x += sinf(camera->rot.y * (float) M_PI / 180.0f) * camera->zoom;
	pos.z += cosf(camera->rot.y * (float) M_PI / 180.0f) * camera->zoom;
	pos.y += sinf(camera->rot.x * (float) M_PI / 180.0f) * camera->zoom;

	pos.x = -pos.x;

	return pos;
}

