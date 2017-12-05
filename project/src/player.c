#include "player.h"
#include "mesh.h"
#include "controls.h"
#include "material.h"
#include "gl.h"

static void renderDir(Player *player)
{
	static float scale = 0.5f;
	vec3f dir = mulVec3f(player->forward, player->jumpSpeed);
	if (player->vel.x != 0 || player->vel.y != 0)
		dir = player->vel;

	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(1, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(player->pos.x, player->pos.y, player->pos.z);
	glVertex3f(player->pos.x + dir.x * scale, player->pos.y + dir.y * scale, player->pos.z + dir.z * scale);
	glEnd();

	glPopAttrib();
}

static void updateControls(Player *player, PlayerControls *controls, float dt)
{
	static float rotSpeed = 45.0f * M_PI / 180.0f;
	static float speedSpeed = 10.0f;

	float turn = 0;
	if (controls->left)
		turn += player->turnSpeed;
	if (controls->right)
		turn -= player->turnSpeed;
	player->heading += turn *dt;
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
	static float gravity = 9.8f;

	if (player->pos.y <= 0 && player->vel.y <= 0)
		return;

	player->vel.y -= gravity * dt;

	player->pos.y += player->vel.y * dt + 0.5f * gravity * dt * dt;
	player->pos.x += player->vel.x * dt + 0.5f * dt * dt;
	player->pos.z += player->vel.z * dt + 0.5f * dt * dt;
}

static void orientPlayer(Player *player)
{
	if (player->jumpFlag)
		return;

	player->forward.x = sinf(player->heading * (float) M_PI / 180.0f) * cosf(player->jumpRot);
	player->forward.z = cosf(player->heading * (float) M_PI / 180.0f) * cosf(player->jumpRot);
	player->forward.y = sinf(player->jumpRot);
}

void initPlayer(Player *player, MaterialContainer *container, Mesh *mesh, vec3f pos, float turnSpeed, float jumpRot, float jumpSpeed,  GLuint tex)
{
	player->forward = cVec3f(0, 0, 1);
	player->pos = pos;
	player->heading = 0;
	player->jumpSpeed = jumpSpeed;
	player->jumpRot = jumpRot;
	player->turnSpeed = turnSpeed;
	player->vel = cVec3f(0, 0, 0);
	player->tex = tex;
	player->mesh = mesh;
	player->material = getMaterial(container, "red");
}

void renderPlayer(Player *player, DebugControls *controls)
{
	glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_COLOR_BUFFER_BIT);
	// glDisable(GL_TEXTURE_2D);

	renderMaterial(player->material);
	glBindTexture(GL_TEXTURE_2D, player->tex);

	glPushMatrix();

	glTranslatef(player->pos.x, player->pos.y + 0.5, player->pos.z);
	glRotatef(player->heading, 0, 1, 0);

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

	if (player->pos.y <= 0)
	{
		player->vel = cVec3f(0, 0, 0);
		player->pos.y = 0;
		player->jumpFlag = false;
	}
}

void jump(Player *player)
{
	if (player->pos.y > 0)
		return;

	player->vel = mulVec3f(player->forward, player->jumpSpeed);
	player->jumpFlag = true;
}

