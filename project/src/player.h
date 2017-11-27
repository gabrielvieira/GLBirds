#pragma once

#if __cplusplus
extern "C" {
#endif

#include "utils.h"

typedef struct Player Player;
typedef struct PlayerControls PlayerControls;
typedef struct DebugControls DebugControls;
typedef struct Mesh Mesh;
typedef struct Material Material;
typedef struct MaterialContainer MaterialContainer;

struct Player
{
	// These are current position/velocity, the player does not store initial values for these. Heading represents
	// rotation of the player around the y axis.
	vec3f pos;
	vec3f vel;
	float heading;

	// Current forward vector of the player, which is a combination of the jumpRot/jumpSpeed and the heading.
	vec3f forward;

	// How quickly the player can turn, in degrees per second.
	float turnSpeed;

	// Rot/speed are initial values, initial velocity can be determined using them. Note that rot represents rotation
	// around the z axis.
	float jumpRot;
	float jumpSpeed;

	// Flag to determine if the player is jumping.
	bool jumpFlag;

	// Now we have a mesh and a material that is used to render the player.
	Mesh *mesh;
	Material *material;
};

void initPlayer(Player *player, MaterialContainer *container, Mesh *mesh, vec3f pos, float turnSpeed, float jumpRot, float jumpSpeed);
void renderPlayer(Player *player, DebugControls *controls);

void updatePlayer(Player *player, PlayerControls *controls, float dt);

void jump(Player *player);

#if __cplusplus
}
#endif

