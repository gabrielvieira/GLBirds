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
	// In reality you probably shouldn't use strtok. Unfortunately I'm not a very good programmer so I've used it.
	char *xc = strtok(0, TAGS);
	char *yc = strtok(0, TAGS);
	char *zc = strtok(0, TAGS);
	char *wc = strtok(0, TAGS);
	return cVec4f(strtof(xc, 0), strtof(yc, 0), strtof(zc, 0), strtof(wc, 0));
}



void initMaterials(MaterialContainer *container)
{
	// I've stored all the materials in a materials.txt file to make things easier for you. If you want to add more
	// materials just throw them in there.
	container->nMaterials = 0;
	container->capacity = 1;
	container->materials = (Material*) malloc(sizeof(Material));
	loadMaterials(container, "../data/materials.txt");
}

void renderMaterial(Material *material)
{
	// Nothing special, just applying values stored in the material.
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (const GLfloat*) &(material->ambient));
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (const GLfloat*) &(material->diffuse));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (const GLfloat*) &(material->specular));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->shininess);
}

void dMaterialContainer(MaterialContainer *container)
{
	// Always have to remember to free memory that you've allocated.
	for (int i = 0; i < container->nMaterials; i++)
		free(container->materials[i].name);
	free(container->materials);
}

Material *addMaterial(MaterialContainer *container, const char *name)
{
	// Fairly basic push function for adding to the tail of a vector.
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
	// Retrieves a material from the container that matches the given name, if no such material exists you get nothing.
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
	// And finally we need to load the materials from a file. You may think all this is a bit overkill but it only
	// takes a few minutes to set it all up and in return I get a nice easy way to add new materials and use them.
	// This is a similar approach to what I used in the assignment 3 implementation for loading the frog model and its
	// jump/land/ribbit animations (although I'm not expecting you to load those from files if you don't want to).
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

