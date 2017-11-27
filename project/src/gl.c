#include "gl.h"

// Program uses the Simple OpenGL Image Library for loading textures: http://www.lonesock.net/soil.html
#include <SOIL/SOIL.h>

GLuint loadTexture(const char *filename)
{
	glPushAttrib(GL_TEXTURE_BIT);
	GLuint tex = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
	glPopAttrib();
	return tex;
}

void initRendering()
{
	glewInit();

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glClearColor(0, 0, 0, 0);

	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glEnable(GL_LIGHT0);
}

