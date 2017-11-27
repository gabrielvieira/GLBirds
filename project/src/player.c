#include "player.h"
#include "mesh.h"
#include "controls.h"
#include "material.h"
#include "gl.h"

/*
//   _____  _          _    _         ______                    _    _                    
//  /  ___|| |        | |  (_)        |  ___|                  | |  (_)                   
//  \ `--. | |_  __ _ | |_  _   ___   | |_  _   _  _ __    ___ | |_  _   ___   _ __   ___ 
//   `--. \| __|/ _` || __|| | / __|  |  _|| | | || '_ \  / __|| __|| | / _ \ | '_ \ / __|
//  /\__/ /| |_| (_| || |_ | || (__   | |  | |_| || | | || (__ | |_ | || (_) || | | |\__ \
//  \____/  \__|\__,_| \__||_| \___|  \_|   \__,_||_| |_| \___| \__||_| \___/ |_| |_||___/
//
*/
static void renderDir(Player *player)
{
	static float scale = 0.5f;

	// If we're not moving, draw the direction vector for the start of the jump, otherwise draw current velocity.
	// This is a *little* bit hacky and you don't need to do it, but I wanted to see the velocity change during the jump.
	vec3f dir = mulVec3f(player->forward, player->jumpSpeed);
	if (player->vel.x != 0 || player->vel.y != 0)
		dir = player->vel;

	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	// You could use the player's translation/rotation to handle this instead of using pos/dir.
	glColor3f(1, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(player->pos.x, player->pos.y, player->pos.z);
	glVertex3f(player->pos.x + dir.x * scale, player->pos.y + dir.y * scale, player->pos.z + dir.z * scale);
	glEnd();

	glPopAttrib();
}

static void updateControls(Player *player, PlayerControls *controls, float dt)
{
	// This stuff used to be in an update function in globals.c, but I've moved it here. It's purely for setting the
	// values of heading/jumpRot/jumpSpeed based on what keys are being pressed.
	static float rotSpeed = 45.0f * M_PI / 180.0f;
	static float speedSpeed = 10.0f;

	// I figured I'd let the player turn while in the air. This isn't that noticeable with a sphere, but it's much
	// easier to see when you add a frog model.
	float turn = 0;
	if (controls->left)
		turn += player->turnSpeed;
	if (controls->right)
		turn -= player->turnSpeed;
	player->heading += turn *dt;

	// Don't change rot/speed if we're already jumping.
	if (player->jumpFlag)
		return;

	float rot = 0;
	float speed = 0;

	if (controls->downRot)
		rot -= rotSpeed * dt;
	if (controls->upRot)
		rot += rotSpeed * dt;
	if (controls->upSpeed)
		speed += speedSpeed * dt;
	if (controls->downSpeed)
		speed -= speedSpeed * dt;

	player->jumpRot += rot;
	player->jumpSpeed += speed;
}

static void updatePlayerPos(Player *player, float dt)
{
	// We only use numerical integration now since it's much simpler and can be used to handle any kind of movement
	// (not just jumping).
	static float gravity = 9.8f;

	// If we've reached the end of our jump (or aren't jumping), don't integrate.
	if (player->pos.y <= 0 && player->vel.y <= 0)
		return;

	player->vel.y -= gravity * dt;

	player->pos.y += player->vel.y * dt + 0.5f * gravity * dt * dt;
	player->pos.x += player->vel.x * dt + 0.5f * dt * dt;
	player->pos.z += player->vel.z * dt + 0.5f * dt * dt;
}

static void orientPlayer(Player *player)
{
	// Only calculate a new forward vector if not jumping.
	if (player->jumpFlag)
		return;

	player->forward.x = sinf(player->heading * (float) M_PI / 180.0f) * cosf(player->jumpRot);
	player->forward.z = cosf(player->heading * (float) M_PI / 180.0f) * cosf(player->jumpRot);
	player->forward.y = sinf(player->jumpRot);
}


/*
//   _ ______              _  _   ______                    _    _                    
//  ( )| ___ \            | |( )  |  ___|                  | |  (_)                   
//  |/ | |_/ / ___   __ _ | ||/   | |_  _   _  _ __    ___ | |_  _   ___   _ __   ___ 
//     |    / / _ \ / _` || |     |  _|| | | || '_ \  / __|| __|| | / _ \ | '_ \ / __|
//     | |\ \|  __/| (_| || |     | |  | |_| || | | || (__ | |_ | || (_) || | | |\__ \
//     \_| \_|\___| \__,_||_|     \_|   \__,_||_| |_| \___| \__||_| \___/ |_| |_||___/
//
*/
void initPlayer(Player *player, MaterialContainer *container, Mesh *mesh, vec3f pos, float turnSpeed, float jumpRot, float jumpSpeed)
{
	player->forward = cVec3f(0, 0, 1);
	player->pos = pos;
	player->heading = 0;
	player->jumpSpeed = jumpSpeed;
	player->jumpRot = jumpRot;
	player->turnSpeed = turnSpeed;
	player->vel = cVec3f(0, 0, 0);
	player->mesh = mesh;
	player->material = getMaterial(container, "red");
}

void renderPlayer(Player *player, DebugControls *controls)
{
	glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_COLOR_BUFFER_BIT);
	glDisable(GL_TEXTURE_2D);

	renderMaterial(player->material);

	glPushMatrix();

	glTranslatef(player->pos.x, player->pos.y + 0.5, player->pos.z);
	glRotatef(player->heading, 0, 1, 0);

	// Can also have these rotations, but not necessary for the assignment.
	//glRotatef(player->rot.z, 0, 0, 1);
	//glRotatef(player->rot.x, 1, 0, 0);

	glPushMatrix();
	glScalef(0.5f, 0.5f, 0.5f);
	renderMesh(player->mesh, controls);
	glPopMatrix();

	glPopMatrix();

	glPopAttrib();

	renderDir(player);
}

void updatePlayer(Player *player, PlayerControls *controls, float dt)
{
	updateControls(player, controls, dt);
	updatePlayerPos(player, dt);
	orientPlayer(player);

	// Finished the jump? Reset everything.
	if (player->pos.y <= 0)
	{
		player->vel = cVec3f(0, 0, 0);
		player->pos.y = 0;
		player->jumpFlag = false;
	}
}

void jump(Player *player)
{
	// Already jumping? Well sadly double jump is not implemented... yet (seriously feel free to add it in, it'll be awesome).
	if (player->pos.y > 0)
		return;

	player->vel = mulVec3f(player->forward, player->jumpSpeed);
	player->jumpFlag = true;
}

