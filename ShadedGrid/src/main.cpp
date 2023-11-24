#ifdef WIN32
    #define SDL_MAIN_HANDLED
#endif
#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include "GLFunctions.h"
#include "Shaders.h"

/// @brief function to quit SDL with error message
/// @param[in] _msg the error message to send
void SDLErrorExit(const std::string &_msg);

/// @brief initialize SDL OpenGL context
SDL_GLContext createOpenGLContext( SDL_Window *window);

void updatePoints(std::vector<vertData> &_data);

int main()
{
  // Initialize SDL's Video subsystem
  if (SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    // Or die on error
    SDLErrorExit("Unable to initialize SDL");
  }

  // now create our window
  SDL_Window *window=SDL_CreateWindow("SDL2 OpenGL4.x Triangle Plane",
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      1024,
                                      720,
                                      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
                                     );
  // check to see if that worked or exit
  if (!window)
  {
    SDLErrorExit("Unable to create window");
  }

  // Create our opengl context and attach it to our window

   SDL_GLContext glContext=createOpenGLContext(window);
   if(!glContext)
   {
     SDLErrorExit("Problem creating OpenGL context ");
   }
   // make this our current GL context (we can have more than one window but in this case not)
   SDL_GL_MakeCurrent(window, glContext);
  /* This makes our buffer swap syncronized with the monitor's vertical refresh */
    SDL_GL_SetSwapInterval(1);

   if (gl3wInit()) 
   {
    std::cerr<<"failed to initialize OpenGL\n";
    exit(EXIT_FAILURE); 
   }
   if (!gl3wIsSupported(4, 1)) 
   {
        fprintf(stderr, "OpenGL 4.1 not supported\n");
        exit(EXIT_FAILURE);
    }
      
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  GLuint vboID;
  glGenBuffers(1,&vboID);
  glBindBuffer(GL_ARRAY_BUFFER,vboID);
  // map the above buffer object to the array buffer
  enum class DrawType : GLuint{POINTS=0,LINES=1,TRIANGLES=2};
  DrawType drawMode=DrawType::POINTS;
  constexpr int width=40;
  constexpr int height=60;
  auto points= createPoints(12.0f,12.0f,width,height);

  auto diffuseShader=loadShaderFromStrings(vertex,fragment);
  auto colourShader=loadShaderFromStrings(colourvertex,colourfragment);
  // we will store uniform locations here as it is expensive to look up each time
  // First MVP
  glUseProgram(diffuseShader);
  auto MVP=glGetUniformLocation(diffuseShader,"MVP");
  auto model=glGetUniformLocation(diffuseShader,"model");
  auto lightPos=glGetUniformLocation(diffuseShader,"lightPos");
  glUseProgram(colourShader);
  auto MVPColour=glGetUniformLocation(colourShader,"MVP");
  
  // now set the things that are not going to change much
  glUniform3f(lightPos,0.0f,8.0f,0.0f);

  // now clear the screen and swap whilst NGL inits (which may take time)
  glClearColor(0.8f,0.8f,0.8f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  SDL_GL_SwapWindow(window);
  // flag to indicate if we need to exit
  bool quit=false;
  float rotation=0.0f;
  // sdl event processing data structure
  SDL_Event event;
  glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 6.0f, 10.0f), 
                               glm::vec3(0.0f, 0.0f, 0.0f), 
                               glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 project = glm::perspective(45.0f, static_cast<float> (1024)/720, 0.1f, 100.0f);
  glEnable(GL_DEPTH_TEST);
  bool update=true;
  bool rotate=true;
  
  while(!quit)
  {

    while ( SDL_PollEvent(&event) )
    {
      switch (event.type)
      {
        // this is the window x being clicked.
        case SDL_QUIT : quit = true; break;
        // if the window is re-sized pass it to the ngl class to change gl viewport
        // note this is slow as the context is re-create by SDL each time
        case SDL_WINDOWEVENT :
          int w,h;
          // get the new window size
          SDL_GL_GetDrawableSize(window,&w,&h);
          glViewport(0,0,w,h);
          project = glm::perspective(45.0f, static_cast<float> (w)/h, 0.01f, 200.0f);
        break;

        // now we look for a keydown event
        case SDL_KEYDOWN:
        {
          switch( event.key.keysym.sym )
          {
            // if it's the escape key quit
            case SDLK_ESCAPE :  quit = true; break;
            case SDLK_w : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
            case SDLK_s : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
            case SDLK_g : SDL_SetWindowFullscreen(window,SDL_FALSE); break;
            case SDLK_p : drawMode=DrawType::POINTS; break;
            case SDLK_l : drawMode=DrawType::LINES; break;
            case SDLK_t : drawMode=DrawType::TRIANGLES; break;
            case SDLK_u : update^=true; break;
            case SDLK_r : rotate^=true; break;
            
            
            default : break;
          } // end of key process
        } // end of keydown

        break;
      default : break;
      } // end of event switch
    } // end of poll events
    glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
    if(rotate)
      rotation+=1.0f;
    // create a rotation matrix around the y axis note the conversion to radians
    auto rotY=glm::rotate(glm::mat4(1.0f),glm::radians(rotation),glm::vec3(0.0f,1.0f,0.0f));
    // now set the MVP matrix of the triangle
    auto transform = project * view * rotY; 
    // note the use of glm::value_ptr to convert to gl pointer.
    switch (drawMode)
    {
      case DrawType::POINTS :
        glUseProgram(colourShader);
        glPointSize(4.0f);    
        glUniformMatrix4fv(MVPColour,1,GL_FALSE,glm::value_ptr(transform));
        drawPoints(vao,points);
      break;
      case DrawType::LINES : 
      
        glUseProgram(colourShader);
        glUniformMatrix4fv(MVPColour,1,GL_FALSE,glm::value_ptr(transform));
        drawLines(vao,points,width,height);

      
      break;
      case DrawType::TRIANGLES : 
        glUseProgram(diffuseShader); 
        glUniformMatrix4fv(model,1,GL_FALSE,glm::value_ptr(view * rotY));
        glUniformMatrix4fv(MVP,1,GL_FALSE,glm::value_ptr(transform));
        drawShaded(vao,points,width,height);
  
        break;
    }

    if(update)
      updatePoints(points);
    // swap the buffers 
    SDL_GL_SwapWindow(window);

  }
  glDeleteProgram(colourShader);
  glDeleteProgram(diffuseShader);
  
  glDeleteVertexArrays(1,&vao);

  // now tidy up and exit SDL
 SDL_Quit();
 // whilst this code will never execute under windows we need to have a return from
 // SDL_Main!
 return EXIT_SUCCESS;
}


SDL_GLContext createOpenGLContext(SDL_Window *window)
{
  // Note you may have to change this depending upon the driver (Windows is fussy)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  // set multi sampling else we get really bad graphics that alias
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,4);
  // Turn on double buffering with a 24bit Z buffer.
  // You may need to change this to 16 or 32 for your system
  // on mac up to 32 will work but under linux centos build only 16
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  // enable double buffering (should be on by default)
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  //
  return SDL_GL_CreateContext(window);

}

void SDLErrorExit(const std::string &_msg)
{
  std::cerr<<_msg<<'\n';
  std::cerr<<SDL_GetError()<<'\n';
  SDL_Quit();
  exit(EXIT_FAILURE);
}


void updatePoints(std::vector<vertData> &_data)
{
  static float offset = 0.0f;

  // now copy data to ptr
  for(auto &vert : _data)
  {
    vert.position.y=sin(( vert.position.x + offset)) + cos( vert.position.x - offset);
    vert.position.y+=sin(( vert.position.z + offset)) + cos( vert.position.z - offset);
  }
 
  offset += 0.01f;
}
