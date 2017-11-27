#pragma once

#if __cplusplus
extern "C" {
#endif

#include "utils.h"

typedef struct Parabola Parabola;
typedef struct Player Player;
typedef struct DebugControls DebugControls;

struct Parabola
{
	// Initial position of the parabola, so it doesn't follow the player during a jump.
	vec3f pos;

	// Since the Parabola is really a debug tool for the Player, it made sense to me to organise things this way, which means
	// the parabola just uses the information stored in player for rendering.
	Player *player;
};

void initParabola(Parabola *parabola, Player *player);
void renderParabola(Parabola *parabola, DebugControls *controls);

#if __cplusplus
}
#endif

