#include <gl/glew.h>
#include <SFML/Window.hpp>
#include <cstdlib>
#include <iostream>
#include "GLFunctions.h"
int main()
{
    sf::ContextSettings settings(41);
    settings.depthBits=24;
    sf::Window window(sf::VideoMode(1024, 720), "SFML Triangle GLEW", sf::Style::Default,settings );
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);
 
    #ifndef __APPLE__
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
  std::cerr<<"Error: "<<glewGetErrorString(err)<<'\n';
  exit(EXIT_FAILURE);
  }
  std::cerr<<"Status: Using GLEW"<< glewGetString(GLEW_VERSION)<<'\n';
#endif
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
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


  // run the main loop
    bool running = true;
    while (running)
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        // key processing
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
          glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
          glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
          running = false;
        }


        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vaoID);		// select first bind the array
        glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object




        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

  glDeleteProgram(shaderID);
  glDeleteVertexArrays(1,&vaoID);

    return EXIT_SUCCESS;
}