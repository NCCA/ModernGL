#ifndef __APPLE__
  #include <GL/glew.h>
#endif
#include <GL/freeglut.h>
#include <iostream>
#include <cstdlib>

static void draw(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
  glutCreateWindow("Modern GL Triangle");

  glutDisplayFunc(draw);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key);

  glutMainLoop();
  return EXIT_SUCCESS;
}