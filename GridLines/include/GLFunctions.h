#ifndef GLFUNCTIONS_H_
#define GLFUNCTIONS_H_
#include <string>
#include <GL/gl3w.h>
#include <utility>
// Generate an triangle  with colours
extern std::pair<GLuint,size_t> createGrid(float _w, float _h, int _rows, int _cols);
extern GLuint loadShaderFromStrings( const std::string &_vertex, const std::string &_fragment);

#endif
