#ifndef GLFUNCTIONS_H_
#define GLFUNCTIONS_H_
#include <string>
#ifndef __APPLE__
  #include <GL/glew.h>
#endif
// Generate an triangle  with colours
extern GLuint createTriangle(float _size);
extern GLuint loadShaderFromStrings( const std::string &_vertex, const std::string &_fragment);

#endif