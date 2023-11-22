#ifndef GLFUNCTIONS_H_
#define GLFUNCTIONS_H_
#include <string>
#include <GL/gl3w.h>
// Generate an triangle  with colours
extern GLuint createPoints(size_t _ammount);
extern GLuint loadShaderFromStrings( const std::string &_vertex, const std::string &_fragment);

#endif
