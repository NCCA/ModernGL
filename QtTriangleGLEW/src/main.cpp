/****************************************************************************
basic OpenGL demo modified from http://qt-project.org/doc/qt-5.0/qtgui/openglwindow.html
****************************************************************************/
#include "OpenGLWindow.h"

#include <QtGui/QGuiApplication>
#include <iostream>
#include "OpenGLWindow.h"



int main(int argc, char **argv)
{
  QGuiApplication app(argc, argv);
  // create an OpenGL format specifier
  QSurfaceFormat format;
  // set the number of samples for multisampling
  // will need to enable glEnable(GL_MULTISAMPLE); once we have a context
  format.setSamples(4);
  #if defined( DARWIN)
    // at present mac osx Mountain Lion only supports GL3.2
    // the new mavericks will have GL 4.x so can change
    format.setMajorVersion(4);
    format.setMinorVersion(1);
  #else
    // with luck we have the latest GL version so set to this
    format.setMajorVersion(4);
    format.setMinorVersion(5);
  #endif
  // now we are going to set to CoreProfile OpenGL so we can't use and old Immediate mode GL
  format.setProfile(QSurfaceFormat::CoreProfile);
  // now set the depth buffer to 24 bits
  format.setDepthBufferSize(24);
  QSurfaceFormat::setDefaultFormat(format);
  // now we are going to create our scene window
  OpenGLWindow window;
  // set the window size
  window.resize(1024, 720);
  // and finally show

  window.show();

  return app.exec();
}



