#ifndef GLFUNCTIONS_H_
#define GLFUNCTIONS_H_
#include <string>
#include <GL/gl3w.h>
#include <utility>

struct vertData
{
  float x;
  float y;
  float z;
  float nx;
  float ny;
  float nz;
  float u;
  float v;
};


// Generate an triangle  with colours
extern std::pair<GLuint,size_t> createTrianglePlane( float _width, float _depth, int _wP,  int _dP, float nx=0.0f, float ny=1.0f,float nz=0.0f) ;
extern GLuint loadShaderFromStrings( const std::string &_vertex, const std::string &_fragment);

#endif
