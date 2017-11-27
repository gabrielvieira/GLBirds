#pragma once

#if __cplusplus
extern "C" {
#endif

#include <GL/glew.h>

#if __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  if _WIN32
#    include <Windows.h>
#  endif
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

GLuint loadTexture(const char *filename);

void initRendering();

#if __cplusplus
}
#endif

