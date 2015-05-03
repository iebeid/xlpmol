

#ifndef GL_GLOBALS_H_
#define GL_GLOBALS_H_


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <Windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
typedef unsigned int uint;
#define M_PI       3.14159265358979323846
#define M_WINDOWS 1
#endif

#include <stdio.h>


#ifndef NO_CHECK_GLERROR
#define CHECK_GLERROR() checkGLError(__FILE__, __LINE__)
#else
#define CHECK_GLERROR() GL_NO_ERROR
#endif

inline int checkGLError(const char* f, const int line)
{
  GLuint err = glGetError();
  if (err != GL_NO_ERROR)
    fprintf(stderr, "%s:%d: OpenGL Error: %s\n", f, line, gluErrorString(err));
  return err;
}

#endif
