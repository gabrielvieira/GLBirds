#include "light.h"
#include "gl.h"

void initLight(Light *light, vec4f pos, vec4f ambient, vec4f diffuse, vec4f specular)
{
	light->pos = pos;
	light->ambient = ambient;
	light->diffuse = diffuse;
	light->specular = specular;
}

void renderLight(Light *light)
{
	glLightfv(GL_LIGHT0, GL_POSITION, (const GLfloat*) &(light->pos));
	glLightfv(GL_LIGHT0, GL_AMBIENT, (const GLfloat*) &(light->ambient));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, (const GLfloat*) &(light->diffuse));
	glLightfv(GL_LIGHT0, GL_SPECULAR, (const GLfloat*) &(light->specular));
}

