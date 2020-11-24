#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <iostream>
#include <OpenGL/gl3.h>

class GLWindow
{
  public:
    GLWindow();
    void update();
    ~GLWindow();
    void setWireframe();
    void setSolid();
  private:
      GLuint  m_shaderID;
      GLuint  m_vaoID;
      bool m_wireframe=false;

};

#endif 