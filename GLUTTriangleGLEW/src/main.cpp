#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cstdlib>
#include "GLFunctions.h"
#include "Shaders.h"

GLuint gvaoID;
GLuint gshaderID;

static void draw(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindVertexArray(gvaoID);		// select first bind the array
  glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object

  glutSwapBuffers();

}

static void idle(void)
{
  glutPostRedisplay();
}

static void key(unsigned char k, int x, int y)
{
  switch (k) {
  case 27:  /* Escape */
    exit(0);
    break;
  case 'w' : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
  case 's' : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
  
  default:
    return;
  }
  glutPostRedisplay();
}


/* new window size or exposure */
static void reshape(int width, int height)
{
  glViewport(0, 0, (GLint) width, (GLint) height);
}



int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitContextVersion( 4, 1 );
  glutInitContextProfile( GLUT_CORE_PROFILE );
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(1024,720);
  glutCreateWindow("Modern GL Triangle");





  glutDisplayFunc(draw);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key);

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
  gvaoID=createTriangle(0.8f);
  gshaderID=loadShaderFromStrings(vertex,fragment);

  glClearColor(0.8f,0.8f,0.8f,1.0f);


  glutMainLoop();
  return EXIT_SUCCESS;
}
