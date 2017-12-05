#include "parabola.h"
#include "player.h"
#include "gl.h"
#include "controls.h"

static vec3f getPosParametric(Parabola *parabola, vec3f dir, float t)
{
	static float gravity = 9.8f;
	return cVec3f(parabola->player->jumpSpeed * t * dir.x, parabola->player->jumpSpeed * t * dir.y -
		(0.5f * gravity * t * t), parabola->player->jumpSpeed * t * dir.z);
}

 float getFlightTime(Parabola *parabola)
{
	static float gravity = 9.8f;
	return (2.0f * parabola->player->jumpSpeed * sinf(parabola->player->jumpRot)) / gravity;
}

 void drawDebugVector(vec3f pos, vec3f color, vec3f start, vec3f end)
{
	glColor3f(color.x, color.y, color.z);
	glVertex3f(pos.x + start.x, pos.y + start.y, pos.z + start.z);
	glVertex3f(pos.x + start.x + end.x, pos.y + start.y + end.y, pos.z + start.z + end.z);
}

 void drawDebugLines(vec3f pos, DebugControls *controls, vec3f dydx, vec3f p)
{
	static float lineSize = 0.5f;

	if (!controls->normalFlag)
		return;

	vec3f nColor = cVec3f(1, 1, 0);

	if (controls->normalFlag)
	{
		vec3f n = mulVec3f(normaliseVec3f(cVec3f(dydx.x, dydx.y, dydx.z)), lineSize);
		drawDebugVector(pos, nColor, p, n);
	}
}

 void drawLine(vec3f pos, vec3f color, vec3f p1, vec3f p2)
{
	glColor3f(color.x, color.y, color.z);
	glVertex3f(pos.x + p1.x, pos.y + p1.y, pos.z + p1.z);
	glVertex3f(pos.x + p2.x, pos.y + p2.y, pos.z + p2.z);
}

void initParabola(Parabola *parabola, Player *player)
{
	parabola->pos = player->pos;
	parabola->player = player;
}

void renderParabola(Parabola *parabola, DebugControls *controls)
{
	static size_t segments = 8;
	static float gravity = 9.8f;

	vec3f dir = parabola->player->forward;
	float scale = getFlightTime(parabola);
	
	vec3f color = cVec3f(0, 0, 1);

	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	for (size_t i = 0; i <= segments; i++)
	{
		float t1 = (float) i / (float) segments * scale;
		float t2 = (float) (i + 1) / (float) segments * scale;

		vec3f p1 = getPosParametric(parabola, dir, t1);
		vec3f p2 = getPosParametric(parabola, dir, t2);
		if (i != segments)
			drawLine(parabola->pos, color, p1, p2);
		
		vec3f dydx = cVec3f(parabola->player->jumpSpeed * dir.x, parabola->player->jumpSpeed * dir.y - gravity * t1,
			parabola->player->jumpSpeed * dir.z);
		drawDebugLines(parabola->pos, controls, dydx, p1);
	}
	glEnd();

	glPopAttrib();
}

