#include <cstdlib>
#include <iostream>
#ifndef __APPLE__
  #include <GL/glew.h>
#else
  #include <OpenGL/gl3.h>
#endif
#include "GLFunctions.h"
#include <GLFW/glfw3.h>
#include <memory>

// key callback
void keyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);


int main()
{
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
  {
      std::cout<<"Failed to init GLFW\n";
      return EXIT_FAILURE;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(1024, 720, "GLFW Triangle", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return EXIT_FAILURE;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  // set the key callback
  glfwSetKeyCallback(window, keyCallback);

  #ifndef __APPLE__
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      std::cerr<<"Error: "<<glewGetErrorString(err)<<'\n';
      exit(EXIT_FAILURE);
    }
    std::cerr<<"Status: Using GLEW"<< glewGetString(GLEW_VERSION)<<'\n';
  #endif


  // create the triangle
  auto vaoID=createTriangle(0.8f);
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

  auto shaderID=loadShaderFromStrings(vertex,fragment);


  // now clear the screen and swap whilst NGL inits (which may take time)
  glClearColor(0.8f,0.8f,0.8f,1.0f);


  // resize the ngl to set the screen size and camera stuff
  // Note we need to get this for retinal display else we will get smaller
  // window due to pixel size see http://www.glfw.org/docs/latest/window.html#window_fbsize
  int width, height;
  glfwGetFramebufferSize  (window,&width,&height);
  glViewport(0,0,width,height);
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
  // now clear the screen and swap whilst NGL inits (which may take time)
  glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vaoID);		// select first bind the array
    glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();

  }
  glDeleteProgram(shaderID);
  glDeleteVertexArrays(1,&vaoID);

  glfwTerminate();

}

void keyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
    if (_key == GLFW_KEY_ESCAPE && _action == GLFW_PRESS)
    {
      exit(EXIT_SUCCESS);
    }
    if (_key == GLFW_KEY_W && _action == GLFW_PRESS)
    {
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    if (_key == GLFW_KEY_S && _action == GLFW_PRESS)
    {
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }

}
