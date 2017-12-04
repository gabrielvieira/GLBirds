#include "parabola.h"
#include "player.h"
#include "gl.h"
#include "controls.h"

/*
//   _____  _          _    _         ______                    _    _                    
//  /  ___|| |        | |  (_)        |  ___|                  | |  (_)                   
//  \ `--. | |_  __ _ | |_  _   ___   | |_  _   _  _ __    ___ | |_  _   ___   _ __   ___ 
//   `--. \| __|/ _` || __|| | / __|  |  _|| | | || '_ \  / __|| __|| | / _ \ | '_ \ / __|
//  /\__/ /| |_| (_| || |_ | || (__   | |  | |_| || | | || (__ | |_ | || (_) || | | |\__ \
//  \____/  \__|\__,_| \__||_| \___|  \_|   \__,_||_| |_| \___| \__||_| \___/ |_| |_||___/
//
*/
static vec3f getPosParametric(Parabola *parabola, vec3f dir, float t)
{
	// You could just calculate this in two dimensions and have glRotatef handle the rest if you prefer.
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
	// There's no need to include the pos.x here if you're using glTranslatef to position the parabola.
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
	// Again, no need to include pos if you're using glTranslatef.
	glColor3f(color.x, color.y, color.z);
	glVertex3f(pos.x + p1.x, pos.y + p1.y, pos.z + p1.z);
	glVertex3f(pos.x + p2.x, pos.y + p2.y, pos.z + p2.z);
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
void initParabola(Parabola *parabola, Player *player)
{
	parabola->pos = player->pos;
	parabola->player = player;
}

void renderParabola(Parabola *parabola, DebugControls *controls)
{
	// I've hard-coded the number of segments and gravity for simplicity. Doing this for the segments is fine but
	// ideally you would have a global physics struct/class where gravity is stored and can be accessed.
	static size_t segments = 8;
	static float gravity = 9.8f;

	vec3f dir = parabola->player->forward;
	float scale = getFlightTime(parabola);
	
	vec3f color = cVec3f(0, 0, 1);

	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	// For this you can simply use glTranslate to have the parabola render at the correct position, and use glRotate to
	// handle the rotations. This means your calculations can effectively still be in 2D as they were in assignment 1.
	// I've calculated this using 3 dimensions in case you wanted to see how that would work (it's not that different
	// really).
	glBegin(GL_LINES);
	for (size_t i = 0; i <= segments; i++)
	{
		float t1 = (float) i / (float) segments * scale;
		float t2 = (float) (i + 1) / (float) segments * scale;

		vec3f p1 = getPosParametric(parabola, dir, t1);
		vec3f p2 = getPosParametric(parabola, dir, t2);

		// Since the loop goes to final vertex to calculate the last normal/tangent, don't draw a line from there to
		// end + 1. Also, refer to comments on line 42 of utils.c if this gives you a compile error in Windows.
		if (i != segments)
			drawLine(parabola->pos, color, p1, p2);
		
		vec3f dydx = cVec3f(parabola->player->jumpSpeed * dir.x, parabola->player->jumpSpeed * dir.y - gravity * t1,
			parabola->player->jumpSpeed * dir.z);
		drawDebugLines(parabola->pos, controls, dydx, p1);
	}
	glEnd();

	glPopAttrib();
}

