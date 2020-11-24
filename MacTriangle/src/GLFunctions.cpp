#include <memory>
#include <array>
#include <iostream>
#include <OpenGL/gl3.h>



GLuint createTriangle(float _size)
{
  GLuint vaoID;
  // allocate a VertexArray
  glGenVertexArrays(1, &vaoID);
  // now bind a vertex array object for our verts
  glBindVertexArray(vaoID);
    // a simple triangle
  std::array<float,9> vert;	// vertex array
  vert[0] =-_size; vert[1] =  -_size; vert[2] =0.0f;
  vert[3] = 0; vert[4] =   _size; vert[5] =0.0f;
  vert[6] = _size; vert[7] =  -_size; vert[8]= 0.0f;
  // now we are going to bind this to our vbo
  GLuint vboID;
  glGenBuffers(1, &vboID);
  // now bind this to the VBO buffer
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  // allocate the buffer data
  glBufferData(GL_ARRAY_BUFFER, vert.size()*sizeof(float), &vert[0], GL_STATIC_DRAW);
  // now fix this to the attribute buffer 0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  // enable and bind this attribute (will be inPosition in the shader)
  glEnableVertexAttribArray(0);

  // Now for the colour

  std::array<float,9> colour={1.0f,0.0f,0.0f,
                              0.0f,1.0f,0.0f,
                              0.0f,0.0f,1.0f};

  GLuint colourvboID;
  glGenBuffers(1, &colourvboID);
  // now bind this to the VBO buffer
  glBindBuffer(GL_ARRAY_BUFFER, colourvboID);
  // allocate the buffer data
  glBufferData(GL_ARRAY_BUFFER, colour.size()*sizeof(float), &colour[0], GL_STATIC_DRAW);
  // now fix this to the attribute buffer 1
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  // enable and bind this attribute (will be inColour in the shader)
  glEnableVertexAttribArray(1);
  // this basically switches off the current Vertex array object
  glBindVertexArray(0);
  return vaoID;
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

    std::cerr<<infoLog<<'\n';
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




