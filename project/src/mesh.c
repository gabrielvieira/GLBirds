#include "mesh.h"
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
static void makeGridIndices(Mesh *mesh, size_t stacks, size_t slices)
{
	// Creates indices for two triangles that make up one cell in the grid. Hopefully I've wound these correctly so
	// that back-face culling won't break everything. If you prefer you could arrange your mesh into triangle strips
	// which would reduce the number of indices required, which would also mean needing to draw the mesh in separate
	// strips rather than all at once. It's up to you which way you would rather do it and I'd encourage you to compare
	// the performance of each approach, but you probably don't have time for that and it's not expected.
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
	// Axis rendering of a mesh, for debugging purposes.
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
	// Standard debug rendering stuff. I've included tangents and binormals here since I'm calculating those too, but
	// you don't have to.
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


/*
//   _ ______              _  _   ______                    _    _                    
//  ( )| ___ \            | |( )  |  ___|                  | |  (_)                   
//  |/ | |_/ / ___   __ _ | ||/   | |_  _   _  _ __    ___ | |_  _   ___   _ __   ___ 
//     |    / / _ \ / _` || |     |  _|| | | || '_ \  / __|| __|| | / _ \ | '_ \ / __|
//     | |\ \|  __/| (_| || |     | |  | |_| || | | || (__ | |_ | || (_) || | | |\__ \
//     \_| \_|\___| \__,_||_|     \_|   \__,_||_| |_| \___| \__||_| \___/ |_| |_||___/
//
*/
Mesh *cMesh(size_t nVerts, size_t nIndices)
{
	// Gotta allocate that memory.
	Mesh *mesh = (Mesh*) malloc(sizeof(Mesh));
	mesh->nVerts = nVerts;
	mesh->nIndices = nIndices;
	mesh->vertices = (Vertex*) calloc(mesh->nVerts, sizeof(Vertex));
	mesh->indices = (int*) calloc(mesh->nIndices, sizeof(int));
	return mesh;
}

void dMesh(Mesh *mesh)
{
	// And I guess we should free it too.
	if (!mesh)
		return;
	free(mesh->vertices);
	free(mesh->indices);
	free(mesh);
}

void calcTangents(Mesh *mesh)
{
	// Note: approximation only, don't rely on this if you plan on implementing normal mapping (don't plan on
	// implementing that). If I magically did this correctly for normal mapping I'd be really *really* surprised.
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
		//vec3f t = tan1[i];
		//vec3f n = mesh->vertices[i].n;
		//vec3f tan = normaliseVec3f(mulVec3f(subVec3f(t, n), dotVec3f(n, t)));
		//float w = (dotVec3f(cross(n, t), tan2[i]) < 0) ? -1 : 1;

		// I'm pretty sure this binormal will be wrong without doing the above calculation as well, but I didn't check
		// that.
		mesh->vertices[i].tan = normaliseVec3f(tan1[i]);
		mesh->vertices[i].bin = normaliseVec3f(tan2[i]);
	}

	free(tan1);
}

void calcNormals(Mesh *mesh)
{
	// Sometimes normal vectors cannot be calculated analytically, in which case we need to do it the old fashioned
	// way. This function will give approximately correct normals for any arbitrary mesh (provided it has been wound
	// correctly, if it is wound incorrectly normals may face the opposite direction).
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

	// Alternate approximation (untested):
	//calcTangents(mesh);
	//for (size_t i = 0; i < mesh->nVerts; i++)
	//	mesh->vertices[i].n = normaliseVec3f(cross(mesh->vertices[i].bin, mesh->vertices[i].tan));
}

Mesh *makeSquare()
{
	Mesh *mesh = cMesh(4, 6);

	mesh->vertices[0].p = cVec3f(0, 0, 0); mesh->vertices[0].t = cVec2f(0, 0); mesh->vertices[0].n = cVec3f(0, 0, 1);
	mesh->vertices[1].p = cVec3f(1, 0, 0); mesh->vertices[1].t = cVec2f(1, 0); mesh->vertices[1].n = cVec3f(0, 0, 1);
	mesh->vertices[2].p = cVec3f(1, 1, 0); mesh->vertices[2].t = cVec2f(1, 1); mesh->vertices[2].n = cVec3f(0, 0, 1);
	mesh->vertices[3].p = cVec3f(0, 1, 0); mesh->vertices[3].t = cVec2f(0, 1); mesh->vertices[3].n = cVec3f(0, 0, 1);

	mesh->indices[0] = 0;
	mesh->indices[1] = 1;
	mesh->indices[2] = 2;
	mesh->indices[3] = 0;
	mesh->indices[4] = 2;
	mesh->indices[5] = 3;

	calcTangents(mesh);

	return mesh;
}

Mesh *makeCube()
{
	Mesh *mesh = cMesh(24, 36);

	// Front
	mesh->vertices[0].p  = cVec3f( 1, -1,  1); mesh->vertices[3].t  = cVec2f(1, 0); mesh->vertices[3].n  = cVec3f( 0,  0,  1);
	mesh->vertices[1].p  = cVec3f( 1,  1,  1); mesh->vertices[2].t  = cVec2f(1, 1); mesh->vertices[2].n  = cVec3f( 0,  0,  1);
	mesh->vertices[2].p  = cVec3f(-1,  1,  1); mesh->vertices[1].t  = cVec2f(0, 1); mesh->vertices[1].n  = cVec3f( 0,  0,  1);
	mesh->vertices[3].p  = cVec3f(-1, -1,  1); mesh->vertices[0].t  = cVec2f(0, 0); mesh->vertices[0].n  = cVec3f( 0,  0,  1);

	// Back
	mesh->vertices[4].p  = cVec3f( 1, -1, -1); mesh->vertices[4].t  = cVec2f(0, 0); mesh->vertices[4].n  = cVec3f( 0,  0, -1);
	mesh->vertices[5].p  = cVec3f(-1, -1, -1); mesh->vertices[5].t  = cVec2f(0, 1); mesh->vertices[5].n  = cVec3f( 0,  0, -1);
	mesh->vertices[6].p  = cVec3f(-1,  1, -1); mesh->vertices[6].t  = cVec2f(1, 1); mesh->vertices[6].n  = cVec3f( 0,  0, -1);
	mesh->vertices[7].p  = cVec3f( 1,  1, -1); mesh->vertices[7].t  = cVec2f(1, 0); mesh->vertices[7].n  = cVec3f( 0,  0, -1);

	// Left
	mesh->vertices[8].p  = cVec3f(-1, -1, -1); mesh->vertices[8].t  = cVec2f(0, 0); mesh->vertices[8].n  = cVec3f(-1,  0,  0);
	mesh->vertices[9].p  = cVec3f(-1, -1,  1); mesh->vertices[9].t  = cVec2f(0, 1); mesh->vertices[9].n  = cVec3f(-1,  0,  0);
	mesh->vertices[10].p = cVec3f(-1,  1,  1); mesh->vertices[10].t = cVec2f(1, 1); mesh->vertices[10].n = cVec3f(-1,  0,  0);
	mesh->vertices[11].p = cVec3f(-1,  1, -1); mesh->vertices[11].t = cVec2f(1, 0); mesh->vertices[11].n = cVec3f(-1,  0,  0);

	// Right
	mesh->vertices[12].p = cVec3f( 1, -1, -1); mesh->vertices[15].t = cVec2f(1, 0); mesh->vertices[15].n = cVec3f( 1,  0,  0);
	mesh->vertices[13].p = cVec3f( 1,  1, -1); mesh->vertices[14].t = cVec2f(1, 1); mesh->vertices[14].n = cVec3f( 1,  0,  0);
	mesh->vertices[14].p = cVec3f( 1,  1,  1); mesh->vertices[13].t = cVec2f(0, 1); mesh->vertices[13].n = cVec3f( 1,  0,  0);
	mesh->vertices[15].p = cVec3f( 1, -1,  1); mesh->vertices[12].t = cVec2f(0, 0); mesh->vertices[12].n = cVec3f( 1,  0,  0);

	// Top
	mesh->vertices[16].p = cVec3f(-1,  1, -1); mesh->vertices[17].t = cVec2f(0, 1); mesh->vertices[17].n = cVec3f( 0,  1,  0);
	mesh->vertices[17].p = cVec3f(-1,  1,  1); mesh->vertices[16].t = cVec2f(0, 0); mesh->vertices[16].n = cVec3f( 0,  1,  0);
	mesh->vertices[18].p = cVec3f( 1,  1,  1); mesh->vertices[19].t = cVec2f(1, 0); mesh->vertices[19].n = cVec3f( 0,  1,  0);
	mesh->vertices[19].p = cVec3f( 1,  1, -1); mesh->vertices[18].t = cVec2f(1, 1); mesh->vertices[18].n = cVec3f( 0,  1,  0);

	// Bottom
	mesh->vertices[20].p = cVec3f(-1, -1, -1); mesh->vertices[20].t = cVec2f(0, 0); mesh->vertices[20].n = cVec3f( 0, -1,  0);
	mesh->vertices[21].p = cVec3f( 1, -1, -1); mesh->vertices[21].t = cVec2f(0, 1); mesh->vertices[21].n = cVec3f( 0, -1,  0);
	mesh->vertices[22].p = cVec3f( 1, -1,  1); mesh->vertices[22].t = cVec2f(1, 1); mesh->vertices[22].n = cVec3f( 0, -1,  0);
	mesh->vertices[23].p = cVec3f(-1, -1,  1); mesh->vertices[23].t = cVec2f(1, 0); mesh->vertices[23].n = cVec3f( 0, -1,  0);

	for (int i = 0; i < 6; i++)
	{
		mesh->indices[i * 6] = i * 4;
		mesh->indices[i * 6 + 1] = i * 4 + 1;
		mesh->indices[i * 6 + 2] = i * 4 + 2;
		mesh->indices[i * 6 + 3] = i * 4;
		mesh->indices[i * 6 + 4] = i * 4 + 2;
		mesh->indices[i * 6 + 5] = i * 4 + 3;
	}

	calcTangents(mesh);

	return mesh;
}

Mesh *makeCircle()
{
	static size_t slices = 8;
	static float scaleU = 2.0f * M_PI;
	
	Mesh *mesh = cMesh((slices + 1), (slices - 1) * 3);

	for (size_t slice = 0; slice <= slices; slice++)
	{
		float u = (slice / (float) slices) * scaleU;
		float x = cosf(u);
		float z = sinf(u);
		float y = 0.0f;
		size_t pos = slice;
		mesh->vertices[pos].p = cVec3f(x, y, z);
		mesh->vertices[pos].t = cVec2f(x * 0.5f + 0.5f, y * 0.5f + 0.5f);
		mesh->vertices[pos].n = cVec3f(0, 1, 0);
	}

	size_t i = 0;
	for (size_t slice = 1; slice < slices; slice++)
	{
		mesh->indices[i++] = 0;
		mesh->indices[i++] = slice + 1;
		mesh->indices[i++] = slice;
	}

	return mesh;
}

Mesh *makeCylinder()
{
	static size_t stacks = 1, slices = 8;
	static float scaleU = 2.0f * M_PI;
	static float offsetU = 0.0, offsetV = -0.5f;
	static float r = 1.0f;

	Mesh *mesh = cMesh((stacks + 1) * (slices + 1) + ((slices + 1) * 2), stacks * slices * 6 + slices * 2 * 6);

	for (size_t stack = 0; stack <= stacks; stack++)
	{
		for (size_t slice = 0; slice <= slices; slice++)
		{
			float u = (slice / (float) slices) * scaleU + offsetU;
			vec3f p = cVec3f(r * cosf(u), r * sinf(u), 1.0 - stack / (float) stacks + offsetU);
			size_t pos = stack * (slices + 1) + slice;
			mesh->vertices[pos].p = p;
			mesh->vertices[pos].t = cVec2f(1.0f - (float) slice / (float) slices, 1.0f - (float) stack / (float) stacks);
			mesh->vertices[pos].n = normaliseVec3f(cVec3f(p.x, p.y, 0.0));
		}
	}

	// Cylinder ends go here

	makeGridIndices(mesh, stacks, slices);
	
	return mesh;
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

Mesh *makeEllipsoid(float a, float b, float c)
{
	static size_t stacks = 11, slices = 11;
	static float scaleU = M_PI, scaleV = 2.0f * M_PI;
	static float offsetU = -M_PI * 0.5f, offsetV = -M_PI;

	Mesh *mesh = cMesh(stacks * slices, (stacks - 1) * (slices - 1) * 6);

	for (size_t stack = 0; stack < stacks; stack++)
	{
		float u = (stack / (float) (stacks - 1)) * scaleU + offsetU;

		for (size_t slice = 0; slice < slices; slice++)
		{
			float v = (slice / (float) (slices - 1)) * scaleV + offsetV;
			vec3f p = cVec3f(a * cosf(u) * cosf(v), b * cosf(u) * sinf(v), c * sinf(u));
			size_t pos = slice * slices + stack;
			mesh->vertices[pos].p = p;
			mesh->vertices[pos].t = cVec2f(1.0f - (float) slice / (float) slices, 1.0f - (float) stack / (float) stacks);
			mesh->vertices[pos].n = normaliseVec3f(cVec3f(2.0f * p.x / (a * a), 2.0f * p.y / (b * b), 2.0f * p.z / (c * c)));
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
	// I'm rendering these using vertex arrays. They're quite a bit outdated, but still faster than immediate mode
	// rendering. You could extend this to use vertex buffer objects or vertex array objects fairly easily, which
	// you will do if you go on to do Realtime Rendering next semester.
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), mesh->vertices);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), &mesh->vertices[0].n);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &mesh->vertices[0].t);

	glDrawElements(GL_TRIANGLES, mesh->nIndices, GL_UNSIGNED_INT, mesh->indices);

	// Here's the immediate mode version for comparison. After you throw in a framerate counter, try comparing the
	// two techniques to see if there's a performance difference.
#if 0
	glBegin(GL_TRIANGLES);

	for (size_t i = 0; i < mesh->nIndices; i++)
	{
		int j = mesh->indices[i];
		glTexCoord2f(mesh->vertices[j].t.x, mesh->vertices[j].t.y);
		glNormal3f(mesh->vertices[j].n.x, mesh->vertices[j].n.y, mesh->vertices[j].n.z);
		glVertex3f(mesh->vertices[j].p.x, mesh->vertices[j].p.y, mesh->vertices[j].p.z);
	}

	glEnd();
#endif

	glPopClientAttrib();

	if (controls->axisFlag)
		renderAxis();
	renderLines(mesh, controls->normalFlag, false, false);
}

