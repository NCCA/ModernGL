#!/usr/bin/env python

import sdl2
import numpy as np
import sys
from OpenGL.GL import *

class SDLWindow :
  def __init__(self) :
    self.shaderID=0
    self.vaoID=0

    if sdl2.SDL_Init(sdl2.SDL_INIT_VIDEO) != 0:
        print(sdl2.SDL_GetError())
        return -1
    self.window = sdl2.SDL_CreateWindow(b"OpenGL demo",
                                   sdl2.SDL_WINDOWPOS_UNDEFINED,
                                   sdl2.SDL_WINDOWPOS_UNDEFINED, 1024, 720,
                                   sdl2.SDL_WINDOW_OPENGL)
    if not self.window:
        print(sdl2.SDL_GetError())
        return -1
    # Force OpenGL 4.1 'core' context.
    sdl2.video.SDL_GL_SetAttribute(sdl2.video.SDL_GL_CONTEXT_MAJOR_VERSION, 4)
    sdl2.video.SDL_GL_SetAttribute(sdl2.video.SDL_GL_CONTEXT_MINOR_VERSION, 1)
    sdl2.video.SDL_GL_SetAttribute(sdl2.video.SDL_GL_CONTEXT_PROFILE_MASK,sdl2.video.SDL_GL_CONTEXT_PROFILE_CORE)
    self.context = sdl2.SDL_GL_CreateContext(self.window)
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

    self.width=1024
    self.height=720

  def render(self) :
    try :
      glViewport( 0, 0, self.width, self.height )
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT )
      glBindVertexArray(self.vaoID)
      glDrawArrays(GL_TRIANGLES, 0, 3)


    except OpenGL.error.GLError :
      print ('error')
    sdl2.SDL_GL_SwapWindow(self.window)

  def cleanup(self) :
    sdl2.SDL_GL_DeleteContext(self.context)
    sdl2.SDL_DestroyWindow(self.window)
    sdl2.SDL_Quit()
    sys.exit(0)

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
    


def main() :
  window=SDLWindow() 
  event = sdl2.SDL_Event()
  running = True
  while running:
    while sdl2.SDL_PollEvent(ctypes.byref(event)) != 0:
      if event.type == sdl2.SDL_QUIT:
        running = False
      # on key up
      sym = event.key.keysym.sym
      if event.type == sdl2.SDL_KEYUP:
        if sym == sdl2.SDLK_w :
          glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
        elif sym == sdl2.SDLK_s :
          glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
      # on_key_press
      elif event.type == sdl2.SDL_KEYDOWN:
        if sym == sdl2.SDLK_ESCAPE :
          running = False
      window.render()
  window.cleanup()


if __name__ == "__main__":
    main()