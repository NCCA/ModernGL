#include <memory>
#include <array>
#include <iostream>
#include <vector>
#include <random>
#include <GL/gl3w.h>
#include "GLFunctions.h"
#include <glm/glm.hpp>


std::vector< vertData > createPoints(float _width, float _depth, int _wP,  int _dP)
{
  // as our plane is centered on 0.0 we range from Width/2.0 and Depth/2.0
  float w2 = _width / 2.0f;
  float d2 = _depth / 2.0f;
  // calculate the steps for each quad / tri
  float wStep = _width / _wP;
  float dStep = _depth / _dP;
  // texture co-ords start at 0,0
  // texture steps
  float du = 0.9f / _wP;
  float dv = 0.9f / _dP;

  float u = 0.0f;
  float v = 0.0f;

  // a std::vector to store our verts, remember vector packs contiguously so we can use it
  std::vector< vertData > data;
  vertData vert;
  float d=-d2;
  for(size_t depth=0; depth<_dP; ++depth)
  {
    float w=-w2;
    for(size_t width=0; width<_wP; ++width)

    {
      // the normals are always the same so set them for d first
      vert.normal.x = 0.0f;
      vert.normal.y = 1.0f;
      vert.normal.z = 0.0f;
      // y is always 0 as in a plane
      vert.position.y  = 0.0f;
      vert.uv.x = u;
      vert.uv.y = v;
      vert.position.x = w;
      vert.position.z = d;
      data.push_back(vert);
      u += du;
      w+=wStep;
    } // end w loop
    d+=dStep;
    u = 0.0f;
    v += du;
  } // end d loop
  
  return data;
}

void drawPoints(GLuint _vao,  const std::vector<vertData> &_data)
{
  glBindVertexArray(_vao);
  glBufferData(GL_ARRAY_BUFFER,_data.size()*sizeof(vertData),&_data[0].position.x,GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertData), 0);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_POINTS,0,_data.size());
  glBindVertexArray(0);
}


void drawLines(GLuint _vao,  const std::vector<vertData> &_data,int _width, int _height)
{
  glBindVertexArray(_vao);
  std::vector<glm::vec3> points;

  auto getPoint=[&](int _x, int _y)
  {
    return _data[(_y*_width)+_x].position;
  };
  // draw width lines
  for(size_t y=0; y<_height; ++y)
  {
    for(size_t x=0; x<_width-1; ++x)
    {
      points.push_back(getPoint(x,y));
      points.push_back(getPoint(x+1,y));
    }
  }
    // draw depth lines
    for(size_t x=0; x<_width; ++x)
    {
      for(size_t y=0; y<_height-1; ++y)
      {
        points.push_back(getPoint(x,y));
        points.push_back(getPoint(x,y+1));
      }
    }
  
  glBufferData(GL_ARRAY_BUFFER,points.size()*sizeof(glm::vec3),&points[0].x,GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_LINES,0,points.size());
  glBindVertexArray(0);
}


void drawShaded(GLuint _vao,  const std::vector<vertData> &_data,int _width, int _height)
{
  glBindVertexArray(_vao);
  std::vector<vertData> points;

  auto getIndex=[&](int _x, int _y)
  {
    return _y*_width+_x;
  };

  for(size_t y=0; y<_height-1; ++y)
  {
    for(size_t x=0; x<_width-1; ++x)
    {
     // calculate triangles and normals
      // triangle 1
      points.push_back(_data[getIndex(x,y)]);
      points.push_back(_data[getIndex(x+1,y)]);
      points.push_back(_data[getIndex(x,y+1)]);
      // triangle 2
      points.push_back(_data[getIndex(x+1,y)]);
      points.push_back(_data[getIndex(x+1,y+1)]);
      points.push_back(_data[getIndex(x,y+1)]);
    }

  }

  // now process the data to calculate face normals
  for(size_t i=0; i<points.size(); i+=3)
  {
    // calculate the normal
    auto v1(points[i+1].position-points[i].position) ; 
    auto v2(points[i+2].position-points[i].position); 
    glm::vec3 normal=glm::normalize(glm::cross(v1,v2));
    points[i].normal=normal;
    points[i+1].normal=normal;
    points[i+2].normal=normal;
  }
/*
  // now calculate the average vertex normals
  for(size_t y=0; y<_height-1; ++y)
  {
    for(size_t x=0; x<_width-1; ++x)
    {
      // calculate the average normal
      glm::vec3 normal(0.0f);
      // triangle 1
      normal+=points[getIndex(x,y)].normal;
      normal+=points[getIndex(x+1,y)].normal;
      normal+=points[getIndex(x,y+1)].normal;
      // triangle 2
      normal+=points[getIndex(x+1,y)].normal;
      normal+=points[getIndex(x+1,y+1)].normal;
      normal+=points[getIndex(x,y+1)].normal;
      // now normalise
      normal=glm::normalize(normal);
      // now set the normals
      points[getIndex(x,y)].normal=normal;
      points[getIndex(x+1,y)].normal=normal;
      points[getIndex(x,y+1)].normal=normal;
      // triangle 2
      points[getIndex(x+1,y)].normal=normal;
      points[getIndex(x+1,y+1)].normal=normal;
      points[getIndex(x,y+1)].normal=normal;
    }

  }
*/
  glBufferData(GL_ARRAY_BUFFER,points.size()*sizeof(vertData),&points[0].position.x,GL_STATIC_DRAW);
  glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(vertData), static_cast< float * >(nullptr) + 0); 
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(vertData), static_cast< float * >(nullptr) + 3);   
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, sizeof(vertData), static_cast< float * >(nullptr) + 6); 
  glEnableVertexAttribArray(2);


  glDrawArrays(GL_TRIANGLES,0,points.size());
  glBindVertexArray(0);
}


void printInfoLog(const GLuint &_obj  , GLenum _mode=GL_COMPILE_STATUS)
{
  GLint infologLength = 0;
  GLint charsWritten  = 0;
  std::unique_ptr<char []>infoLog;

  glGetShaderiv(_obj, GL_INFO_LOG_LENGTH,&infologLength);
  std::cerr<<"info log length "<<infologLength<<"\n";
  if(infologLength > 0)
  {
    infoLog=std::make_unique<char []>(infologLength);
    glGetShaderInfoLog(_obj, infologLength, &charsWritten, &infoLog[0]);

    std::cerr<<infoLog.get()<<'\n';
    glGetShaderiv(_obj, _mode,&infologLength);
    if( infologLength == GL_FALSE)
    {
      if(_mode == GL_COMPILE_STATUS)
        std::cerr<<"Shader compile failed or had warnings \n";
      else 
        std::cerr<<"linker error\n";
      exit(EXIT_FAILURE);
    }
  }

}
GLuint loadShaderFromStrings( const std::string &_vertex, const std::string &_fragment)
{
 // here we create a program
 GLuint shaderID=glCreateProgram();
 
  // create a Vertex shader object
  GLuint vertexID=glCreateShader(GL_VERTEX_SHADER);
  // attatch the shader source we need to convert to GL format
  const char* source=_vertex.c_str();
  glShaderSource(vertexID,1,&source,NULL);
  // now compile the shader
  glCompileShader(vertexID);
  std::cerr<<"compiling vertex shader\n";
  printInfoLog(vertexID);

// now create a fragment shader
  GLuint fragmentID=glCreateShader(GL_FRAGMENT_SHADER);
  // attatch the shader source
  source=_fragment.c_str();
  glShaderSource(fragmentID,1,&source,NULL);
  // compile the shader
  std::cerr<<"compiling frag shader shader\n";
  glCompileShader(fragmentID);
  printInfoLog(fragmentID);
  // now attach to the program object
  glAttachShader(shaderID,vertexID);
  glAttachShader(shaderID,fragmentID);
  
  // link the program
  glLinkProgram(shaderID);
  printInfoLog(shaderID,GL_LINK_STATUS);
  // and enable it for use
  glUseProgram(shaderID);
  // now tidy up the shaders as we don't need them
  glDeleteShader(vertexID);
  glDeleteShader(fragmentID);
  return shaderID;
}




