#ifndef GLFUNCTIONS_H_
#define GLFUNCTIONS_H_
#include <string>
#include <GL/gl3w.h>
#include <utility>
#include <glm/glm.hpp>

struct vertData
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;  
};


// Generate an triangle  with colours
extern  std::vector<vertData> createPoints(float _width, float _depth, int _wP,  int _dP);
extern GLuint loadShaderFromStrings( const std::string &_vertex, const std::string &_fragment);
extern void drawPoints(GLuint _vao,  const std::vector<vertData> &_data);
extern void drawLines(GLuint _vao,  const std::vector<vertData> &_data, int _width, int _height);
extern void drawShaded(GLuint _vao,  const std::vector<vertData> &_data, int _width, int _height);

#endif
