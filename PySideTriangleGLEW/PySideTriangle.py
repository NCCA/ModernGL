#!/usr/bin/python
from PySide2.QtGui import QOpenGLWindow,QSurfaceFormat
from PySide2.QtWidgets import QApplication
from  PySide2.QtCore import *
import numpy as np
import sys
from OpenGL.GL import *

class MainWindow(QOpenGLWindow) :
  
  def createTriangle(self,size) :
    # allocate a VertexArray
    self.vaoID=glGenVertexArrays(1)
    # now bind a vertex array object for our verts
    glBindVertexArray(self.vaoID)
    #  a simple triangle not a numpy array would be good here but can use other methods too
    vert=np.array([-size,-size,0.0,0.0,size,0.0,size,-size,0.0],dtype='float32')
    #  now we are going to bind this to our vbo

    vboID=glGenBuffers(1)
    #  now bind this to the VBO buffer
    glBindBuffer(GL_ARRAY_BUFFER, vboID)
    #  allocate the buffer data
    glBufferData(GL_ARRAY_BUFFER, vert, GL_STATIC_DRAW)
    #  now fix this to the attribute buffer 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, None)
    #  enable and bind this attribute (will be inPosition in the shader)
    glEnableVertexAttribArray(0)

    # // Now for the colour
    colours=np.array([1,0,0,0,1,0,0,0,1],dtype='float32')
    colourvboID=glGenBuffers(1)
    #  now bind this to the VBO buffer
    glBindBuffer(GL_ARRAY_BUFFER, colourvboID)
    #  allocate the buffer data
    glBufferData(GL_ARRAY_BUFFER, colours, GL_STATIC_DRAW)
    #  now fix this to the attribute buffer 0
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, None)
    #  enable and bind this attribute (will be inPosition in the shader)
    glEnableVertexAttribArray(1)
    glBindVertexArray(0);
    

  def loadShaderFromStrings(self,vertex,fragment) :
    # here we create a program
    self.shaderID=glCreateProgram()

    # create a Vertex shader object
    vertexID=glCreateShader(GL_VERTEX_SHADER)
    # attatch the shader source we need to convert to GL format
    
    glShaderSource(vertexID,vertex)
    # now compile the shader
    glCompileShader(vertexID)
    
    #printInfoLog(vertexID)

    # now create a fragment shader
    fragmentID=glCreateShader(GL_FRAGMENT_SHADER)
    # attatch the shader source
    glShaderSource(fragmentID,fragment)
    # compile the shader
    glCompileShader(fragmentID)
    # printInfoLog(fragmentID);
    # now attach to the program object
    glAttachShader(self.shaderID,vertexID)
    glAttachShader(self.shaderID,fragmentID)

    # link the program
    glLinkProgram(self.shaderID)
    #printInfoLog(shaderID,GL_LINK_STATUS);
    # and enable it for use
    glUseProgram(self.shaderID)
    # now tidy up the shaders as we don't need them
    glDeleteShader(vertexID)
    glDeleteShader(fragmentID)
    

  def __init__(self, parent=None):
    super(MainWindow, self).__init__(parent)
    self.shaderID=0
    self.vaoID=0

  def initializeGL(self) :
    self.makeCurrent()
    glClearColor( 0.4, 0.4, 0.4, 1.0 ) 
    glEnable( GL_DEPTH_TEST )
    glEnable( GL_MULTISAMPLE )
    self.createTriangle(0.8)
    vertex="""#version 400 core

layout (location = 0) in vec3  inPosition;
layout (location = 1) in vec3 inColour;
out vec3 vertColour;
void main()
{
  gl_Position = vec4(inPosition, 1.0);
  vertColour = inColour;
}"""
    fragment="""#version 400 core
in vec3 vertColour;
out vec4 fragColour;
void main()
{
  fragColour = vec4(vertColour,1.0);
}
"""
    self.loadShaderFromStrings(vertex,fragment)
    
  def paintGL(self):
    try :
      self.makeCurrent()
      glViewport( 0, 0, self.width, self.height )
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT )
      glBindVertexArray(self.vaoID)
      glDrawArrays(GL_TRIANGLES, 0, 3)


    except OpenGL.error.GLError :
      print 'error'

  def resizeGL(self, w,h) :
    self.width=int(w* self.devicePixelRatio())
    self.height=int(h* self.devicePixelRatio())


  def keyPressEvent(self, event) :
    key=event.key()
    if key==Qt.Key_Escape :
      exit()
    elif key==Qt.Key_W :
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE)
    elif key==Qt.Key_S :
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL)
    
    self.update()

  
if __name__ == '__main__':
  app = QApplication(sys.argv)
  format=QSurfaceFormat()
  format.setSamples(4) 
  format.setMajorVersion(4) 
  format.setMinorVersion(1) 
  format.setProfile(QSurfaceFormat.CoreProfile) 
  # now set the depth buffer to 24 bits
  format.setDepthBufferSize(24) 
  # set that as the default format for all windows
  QSurfaceFormat.setDefaultFormat(format) 

  window = MainWindow()
  window.setFormat(format)
  window.resize(1024,720)
  window.show()
  sys.exit(app.exec_())
