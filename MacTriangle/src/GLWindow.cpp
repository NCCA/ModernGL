#include "GLWindow.hpp"
#include "GLFunctions.h"
#include "Shaders.h"

GLWindow::GLWindow()
{

  // create the triangle
m_vaoID=createTriangle(0.8f);

  m_shaderID=loadShaderFromStrings(vertex,fragment);


  // now clear the screen and swap whilst NGL inits (which may take time)
  glClearColor(0.8f,0.8f,0.8f,1.0f);

}

void GLWindow::update()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindVertexArray(m_vaoID);		// select first bind the array
  if(m_wireframe)
  {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  }
  else 
  {
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }
  glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object

}


GLWindow::~GLWindow()
{
  glDeleteProgram(m_shaderID);
  glDeleteVertexArrays(1,&m_vaoID);

}

void GLWindow::setWireframe() { m_wireframe=true;}
void GLWindow::setSolid() { m_wireframe=false;}
