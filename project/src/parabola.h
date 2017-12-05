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
	vec3f pos;
	Player *player;
};

void initParabola(Parabola *parabola, Player *player);
void renderParabola(Parabola *parabola, DebugControls *controls);

#if __cplusplus
}
#endif

