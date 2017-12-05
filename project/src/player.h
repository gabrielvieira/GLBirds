#pragma once

#if __cplusplus
extern "C" {
#endif

#include "utils.h"
#include "gl.h"

typedef struct Player Player;
typedef struct PlayerControls PlayerControls;
typedef struct DebugControls DebugControls;
typedef struct Mesh Mesh;
typedef struct Material Material;
typedef struct MaterialContainer MaterialContainer;

struct Player
{
	vec3f pos;
	vec3f vel;
	float heading;
	vec3f forward;
	float turnSpeed;
	float jumpRot;
	float jumpSpeed;
	GLuint tex;
	bool jumpFlag;
	Mesh *mesh;
	Material *material;
};

void initPlayer(Player *player, MaterialContainer *container, Mesh *mesh, vec3f pos, float turnSpeed, float jumpRot, float jumpSpeed, GLuint tex);
void renderPlayer(Player *player);

void updatePlayer(Player *player, PlayerControls *controls, float dt);

void jump(Player *player);

#if __cplusplus
}
#endif

