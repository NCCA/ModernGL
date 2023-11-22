#ifndef GLFUNCTIONS_H_
#define GLFUNCTIONS_H_
#include <string>
#ifndef __APPLE__
  #include <GL/glew.h>
#endif
// Generate an triangle  with colours
extern GLuint createPoints(size_t _ammount);
extern GLuint loadShaderFromStrings( const std::string &_vertex, const std::string &_fragment);

#endif