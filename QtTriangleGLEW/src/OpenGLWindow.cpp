#ifndef __APPLE__
  #include <GL/glew.h>
#endif
#include <array>
#include "OpenGLWindow.h"
#include <QKeyEvent>
#include <QApplication>
#include <iostream>
#include "GLFunctions.h"


OpenGLWindow::OpenGLWindow()
{
  setTitle("First Shaders in OpenGL 4.1");
}

OpenGLWindow::~OpenGLWindow()
{
  glDeleteProgram(m_shaderID);
  glDeleteVertexArrays(1,&m_vaoID);
}



void OpenGLWindow::initializeGL()
{
#ifndef __APPLE__
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
  std::cerr<<"Error: "<<glewGetErrorString(err)<<'\n';
  QApplication::exit(EXIT_FAILURE);
  }
  std::cerr<<"Status: Using GLEW"<< glewGetString(GLEW_VERSION)<<'\n';
#endif
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background

  std::cerr << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;

  // create the triangle
  m_vaoID=createTriangle(0.8f);
const std::string vertex =R"(
#version 400 core

layout (location = 0) in vec3  inPosition;
layout (location = 1) in vec3 inColour;
out vec3 vertColour;
void main()
{
  gl_Position = vec4(inPosition, 1.0);
  vertColour = inColour;
}
)";

 // some source for our fragment shader
  const std::string fragment =R"(
#version 400 core
in vec3 vertColour;
out vec4 fragColour;
void main()
{
  fragColour = vec4(vertColour,1.0);
}
)";

  m_shaderID=loadShaderFromStrings(vertex,fragment);

}


void OpenGLWindow::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindVertexArray(m_vaoID);		// select first bind the array
  glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object
}


void OpenGLWindow::resizeGL(int _w , int _h)
{
}

void OpenGLWindow::keyPressEvent(QKeyEvent *_event)
{
  switch (_event->key())
  {
   case Qt::Key_Escape : QApplication::exit(EXIT_SUCCESS); break;
   case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
   case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;  
  }
  update();
}
