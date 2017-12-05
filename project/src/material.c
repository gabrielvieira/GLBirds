#include "material.h"
#include "gl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 255
#define TAGS ", \n\r"

// Materials contain a lot of vec4 values, loading them is a lot easier using this.
static vec4f getVec4f()
{
	char *xc = strtok(0, TAGS);
	char *yc = strtok(0, TAGS);
	char *zc = strtok(0, TAGS);
	char *wc = strtok(0, TAGS);
	return cVec4f(strtof(xc, 0), strtof(yc, 0), strtof(zc, 0), strtof(wc, 0));
}



void initMaterials(MaterialContainer *container)
{
	container->nMaterials = 0;
	container->capacity = 1;
	container->materials = (Material*) malloc(sizeof(Material));
	loadMaterials(container, "../data/materials.txt");
}

void renderMaterial(Material *material)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (const GLfloat*) &(material->ambient));
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (const GLfloat*) &(material->diffuse));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (const GLfloat*) &(material->specular));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->shininess);
}

void dMaterialContainer(MaterialContainer *container)
{
	for (int i = 0; i < container->nMaterials; i++)
		free(container->materials[i].name);
	free(container->materials);
}

Material *addMaterial(MaterialContainer *container, const char *name)
{
	if (container->nMaterials >= container->capacity)
	{
		container->capacity *= 2;
		container->materials = (Material*) realloc(container->materials, sizeof(Material) * container->capacity);
	}

	container->materials[container->nMaterials].name = (char*) calloc(strlen(name) + 1, sizeof(char));
	strcpy(container->materials[container->nMaterials].name, name);
	container->nMaterials++;
	return &container->materials[container->nMaterials - 1];
}

Material *getMaterial(MaterialContainer *container, const char *name)
{
	for (int i = 0; i < container->nMaterials; i++)
	{
		if (strcmp(container->materials[i].name, name) != 0)
			continue;
		return &container->materials[i];
	}
	return 0;
}

void loadMaterials(MaterialContainer *container, const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file)
	{
		fprintf(stderr, "Unable to open file %s\n", filename);
		exit(EXIT_SUCCESS);
	}

	bool commentFlag = false;
	Material *material = 0;

	char line[MAXLEN];
	while (fgets(line, MAXLEN, file))
	{
		char *token = strtok(line, TAGS);
		if (!token || token[0] == '#')
			continue;

		if (strcmp(token, "/*") == 0)
		{
			commentFlag = true;
			continue;
		}
		else if (strcmp(token, "*/") == 0)
		{
			commentFlag = false;
			continue;
		}

		if (commentFlag)
			continue;

		if (strcmp(token, "material") == 0)
			material = addMaterial(container, strtok(0, TAGS));
		else if (strcmp(token, "ambient") == 0)
			material->ambient = getVec4f();
		else if (strcmp(token, "diffuse") == 0)
			material->diffuse = getVec4f();
		else if (strcmp(token, "specular") == 0)
			material->specular = getVec4f();
		else if (strcmp(token, "shininess") == 0)
			material->shininess = strtof(strtok(0, TAGS), 0);
	}

	fclose(file);
}

