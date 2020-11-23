#ifndef GLFUNCTIONS_H_
#define GLFUNCTIONS_H_
#include <string>
#include <GL/glew.h>

// Generate an triangle  with colours
extern GLuint createTriangle(float _size);
extern GLuint loadShaderFromStrings( const std::string &_vertex, const std::string &_fragment);

#endif