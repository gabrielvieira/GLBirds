#include "mesh.h"
#include "controls.h"

static void makeGridIndices(Mesh *mesh, size_t stacks, size_t slices)
{

	size_t i = 0;
	for (size_t stack = 0; stack < stacks; stack++)
	{
		for (size_t slice = 0; slice < slices; slice++)
		{
		  mesh->indices[i++] = stack * (slices + 1) + slice;
		  mesh->indices[i++] = (stack + 1) * (slices + 1) + slice;
		  mesh->indices[i++] = (stack + 1) * (slices + 1) + slice + 1;
		  mesh->indices[i++] = stack * (slices + 1) + slice;
		  mesh->indices[i++] = (stack + 1) * (slices + 1) + slice + 1;
		  mesh->indices[i++] = stack * (slices + 1) + slice + 1;
		}
	}
}

static void renderAxis()
{
	glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINES);

	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0); glVertex3f(5, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0); glVertex3f(0, 5, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0); glVertex3f(0, 0, 5);

	glEnd();

	glPopAttrib();
}

static void renderLines(Mesh *mesh, bool normalFlag, bool tangentFlag, bool binormalFlag)
{
	glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINES);

	for (size_t i = 0; i < mesh->nIndices; i++)
	{
		int j = mesh->indices[i];

		if (normalFlag)
		{
			glColor3f(1, 1, 0);
			glVertex3f(mesh->vertices[j].p.x, mesh->vertices[j].p.y, mesh->vertices[j].p.z);
			glVertex3f(mesh->vertices[j].p.x + mesh->vertices[j].n.x, mesh->vertices[j].p.y + mesh->vertices[j].n.y,
				mesh->vertices[j].p.z + mesh->vertices[j].n.z);
		}
		if (tangentFlag)
		{
			glColor3f(0, 1, 1);
			glVertex3f(mesh->vertices[j].p.x, mesh->vertices[j].p.y, mesh->vertices[j].p.z);
			glVertex3f(mesh->vertices[j].p.x + mesh->vertices[j].tan.x, mesh->vertices[j].p.y + mesh->vertices[j].tan.y,
				mesh->vertices[j].p.z + mesh->vertices[j].tan.z);
		}
		if (binormalFlag)
		{
			glColor3f(1, 0, 1);
			glVertex3f(mesh->vertices[j].p.x, mesh->vertices[j].p.y, mesh->vertices[j].p.z);
			glVertex3f(mesh->vertices[j].p.x + mesh->vertices[j].bin.x, mesh->vertices[j].p.y + mesh->vertices[j].bin.y,
				mesh->vertices[j].p.z + mesh->vertices[j].bin.z);
		}
	}

	glEnd();

	glPopAttrib();
}

Mesh *cMesh(size_t nVerts, size_t nIndices)
{
	Mesh *mesh = (Mesh*) malloc(sizeof(Mesh));
	mesh->nVerts = nVerts;
	mesh->nIndices = nIndices;
	mesh->vertices = (Vertex*) calloc(mesh->nVerts, sizeof(Vertex));
	mesh->indices = (int*) calloc(mesh->nIndices, sizeof(int));
	return mesh;
}

void dMesh(Mesh *mesh)
{
	if (!mesh)
		return;
	free(mesh->vertices);
	free(mesh->indices);
	free(mesh);
}

void calcTangents(Mesh *mesh)
{
	vec3f *tan1 = (vec3f*) calloc(mesh->nVerts * 2, sizeof(vec3f));
	vec3f *tan2 = tan1 + mesh->nVerts;

	for (size_t i = 0; i < mesh->nIndices; i += 3)
	{
		size_t i1 = mesh->indices[i];
		size_t i2 = mesh->indices[i + 1];
		size_t i3 = mesh->indices[i + 2];

		vec3f p1 = mesh->vertices[i1].p;
		vec3f p2 = mesh->vertices[i2].p;
		vec3f p3 = mesh->vertices[i3].p;

		vec2f w1 = mesh->vertices[i1].t;
		vec2f w2 = mesh->vertices[i2].t;
		vec2f w3 = mesh->vertices[i3].t;

		vec3f v1 = cVec3f(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
		vec3f v2 = cVec3f(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);

		vec2f s = cVec2f(w2.x - w1.x, w2.y - w1.y);
		vec2f t = cVec2f(w3.x - w1.x, w3.y - w1.y);

		float r = 1.0f / (s.x * t.y - t.x * s.y);

		vec3f sdir = cVec3f((t.y * v1.x - s.y * v2.x) * r, (t.y * v1.y - s.y * v2.y) * r, (t.y * v1.z - s.y * v2.z) * r);
		vec3f tdir = cVec3f((s.x * v2.x - t.x * v1.x) * r, (s.x * v2.y - t.x * v1.y) * r, (s.x * v2.z - t.x * v1.z) * r);

		tan1[i1] = addVec3f(tan1[i1], sdir);
		tan1[i2] = addVec3f(tan1[i2], sdir);
		tan1[i3] = addVec3f(tan1[i3], sdir);

		tan2[i1] = addVec3f(tan2[i1], tdir);
		tan2[i2] = addVec3f(tan2[i2], tdir);
		tan2[i3] = addVec3f(tan2[i3], tdir);
	}

	for (size_t i = 0; i < mesh->nVerts; i++)
	{
		mesh->vertices[i].tan = normaliseVec3f(tan1[i]);
		mesh->vertices[i].bin = normaliseVec3f(tan2[i]);
	}

	free(tan1);
}

void calcNormals(Mesh *mesh)
{
	for (size_t i = 0; i < mesh->nIndices / 3; i++)
	{
		size_t t0 = mesh->indices[i * 3 + 0];
		size_t t1 = mesh->indices[i * 3 + 1];
		size_t t2 = mesh->indices[i * 3 + 2];

		vec3f u = subVec3f(mesh->vertices[t1].p, mesh->vertices[t0].p);
		vec3f v = subVec3f(mesh->vertices[t2].p, mesh->vertices[t0].p);
		vec3f n = cross(u, v);

		mesh->vertices[t0].n = addVec3f(mesh->vertices[t0].n, n);
		mesh->vertices[t1].n = addVec3f(mesh->vertices[t1].n, n);
		mesh->vertices[t2].n = addVec3f(mesh->vertices[t2].n, n);
	}

	for (size_t i = 0; i < mesh->nVerts; i++)
		mesh->vertices[i].n = normaliseVec3f(mesh->vertices[i].n);
}

Mesh *makeSphere()
{
	static size_t stacks = 8, slices = 8;
	static float scaleU = M_PI, scaleV = 2.0f * M_PI;
	Mesh *mesh = cMesh((stacks + 1) * (slices + 1), stacks * slices * 6);

	for (size_t stack = 0; stack <= stacks; stack++)
	{
		float u = (stack / (float) stacks) * scaleU;

		for (size_t slice = 0; slice <= slices; slice++)
		{
			size_t pos = stack * (slices + 1) + slice;
			float v = (slice / (float) slices) * scaleV;
			vec3f p = cVec3f(sinf(u) * cosf(v), sinf(u) * sinf(v), cosf(u));
			mesh->vertices[pos].p = p;
			mesh->vertices[pos].t = cVec2f(1.0f - (float) slice / (float) slices, 1.0f - (float) stack / (float) stacks);
			mesh->vertices[pos].n = normaliseVec3f(p);
		}
	}

	makeGridIndices(mesh, stacks, slices);
	calcTangents(mesh);

	return mesh;
}

Mesh *makeGrid(float width, float height, size_t rows, size_t cols)
{
	float startX = width / 2.0f, startY = height / 2.0f;
	Mesh *mesh = cMesh((rows + 1) * (cols + 1), rows * cols * 6);

	for (size_t row = 0; row <= rows; row++)
	{
		float z = (float) row / (float) rows * height - startY;
		for (size_t col = 0; col <= cols; col++)
		{
			float x = (float) col / (float) cols * width - startX;
			size_t pos = row * (cols + 1) + col;
			mesh->vertices[pos].t = cVec2f((float) col / (float) cols, (float) row / (float) rows);
			mesh->vertices[pos].p = cVec3f(x, 0, z);
			mesh->vertices[pos].n = cVec3f(0, 1, 0);
		}
	}

	makeGridIndices(mesh, rows, cols);
	calcTangents(mesh);

	return mesh;
}

void renderMesh(Mesh *mesh, DebugControls *controls)
{
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), mesh->vertices);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), &mesh->vertices[0].n);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &mesh->vertices[0].t);

	glDrawElements(GL_TRIANGLES, mesh->nIndices, GL_UNSIGNED_INT, mesh->indices);
	glPopClientAttrib();

	if (controls->axisFlag)
		renderAxis();
	renderLines(mesh, controls->normalFlag, false, false);
}

