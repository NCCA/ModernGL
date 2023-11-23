#include <memory>
#include <array>
#include <iostream>
#include <vector>
#include <random>
#include <GL/gl3w.h>
#include "GLFunctions.h"



std::pair<GLuint,size_t> createTrianglePlane( float _width, float _depth, int _wP,  int _dP, float _nx, float _ny,float _nz) 
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

  for(float d = -d2; d < d2; d += dStep)
  {
    for(float w = -w2; w < w2; w += wStep)
    {
      /* tri 1
      // counter clock wise
      3
      | \
      |  \
      |   \
      1____2
      */
      // the normals are always the same so set them for d first
      vert.nx = _nx;
      vert.ny = _ny;
      vert.nz = _nz;
      // y is always 0 as in a plane
      vert.y = 0.0f;
      // now for the per vert stuff
      vert.u = u;
      vert.v = v + dv;
      vert.x = w;
      vert.z = d + dStep;
      data.push_back(vert);
      // 2
      vert.u = u + du;
      vert.v = v + dv;
      vert.x = w + wStep;
      vert.z = d + dStep;
      data.push_back(vert);
      // 3
      vert.u = u;
      vert.v = v;
      vert.x = w;
      vert.z = d;
      data.push_back(vert);

      /* tri 2 w,0,d
      // counter clock wise
      3_____2
      \    |
      \  |
      \ |
      \|
      1

      */
      vert.u = u + du;
      vert.v = v + dv;
      vert.x = w + wStep;
      vert.z = d + dStep;
      data.push_back(vert);
      // 2
      vert.u = u + du;
      vert.v = v;
      vert.x = w + wStep;
      vert.z = d;
      data.push_back(vert);
      // 3
      vert.u = u;
      vert.v = v;
      vert.x = w;
      vert.z = d;
      data.push_back(vert);
      u += du;
    } // end w loop
    u = 0.0f;
    v += du;
  } // end d loop

  GLuint vaoID;
  // allocate a VertexArray
  glGenVertexArrays(1, &vaoID);
  // now bind a vertex array object for our verts
  glBindVertexArray(vaoID);

  GLuint vboID;
  glGenBuffers(1, &vboID);
  // now bind this to the VBO buffer
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  // allocate the buffer data
  glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(vertData), &data[0].x, GL_STATIC_DRAW);
  // now fix this to the attribute buffer 0
  // enable and bind this attribute (will be inPosition in the shader)
  glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(vertData), static_cast< float * >(nullptr) + 0); 
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(vertData), static_cast< float * >(nullptr) + 3);   
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, sizeof(vertData), static_cast< float * >(nullptr) + 6); 
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  return std::pair(vaoID,data.size());
}






std::pair<GLuint,size_t> createGrid(float _w, float _h, int _rows, int _cols)
{
  GLuint vaoID;
  // allocate a VertexArray
  glGenVertexArrays(1, &vaoID);
  // now bind a vertex array object for our verts
  glBindVertexArray(vaoID);
  // going to generate points with the center at 0,0,0
  // using half extents
  std::vector<float> points;
  // step values for x,y
  float colStep=_w/_cols;
  float rowStep=_h/_rows;

  float x= _w*0.5f;
  float z= -(_h*0.5f);

  for(size_t i=0; i<_rows+1; ++i)
  {
    points.push_back(-x);
    points.push_back(0.0f);
    points.push_back(z);
    points.push_back(x);
    points.push_back(0.0f);
    points.push_back(z);
    z+=rowStep;
  }
  z= (_h*0.5f);
  x= -(_w*0.5f);

  for(size_t i=0; i<_cols+1; ++i)
  {
    points.push_back(x);
    points.push_back(0.0f);
    points.push_back(-z);
    points.push_back(x);
    points.push_back(0.0f);
    points.push_back(z);
    x+=colStep;
  }

  std::cout<<"rows "<<_rows<<" cols "<<_cols<<" points "<<points.size()/3<<"\n";
  // we are going to allocate 3 buffers this time one for verts, colours and normals
  GLuint vboID;
  glGenBuffers(1, &vboID);
  // now bind this to the VBO buffer
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  // allocate the buffer data
  glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(float), &points[0], GL_STATIC_DRAW);
  // now fix this to the attribute buffer 0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  // enable and bind this attribute (will be inPosition in the shader)
  glEnableVertexAttribArray(0);


  // this basically switches off the current Vertex array object
  glBindVertexArray(0);
  return std::pair(vaoID,points.size()/3);
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




