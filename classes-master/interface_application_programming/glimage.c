/*
* glimage.c
* (c) Neil Gershenfeld  5/18/99
* example of GL and GLUT to drawing sin(kr)/kr array
*/

#include <GL/glut.h>
#include <math.h>

#define NGRID 100
#define KMIN 0.0
#define KMAX 20.0
float k=0.0, dk=0.2;

#define X(j) (2.0*((float) j + 0.5)/NGRID - 1.0)
#define Y(i) (2.0*((float) i + 0.5)/NGRID - 1.0)
#define r(x,y) (k*sqrt(x*x+y*y))
#define height(x,y) (sin(r(x,y))/r(x,y))

float z[NGRID][NGRID];

void display(void) {
   int i,j;
   for (i = 0; i < (NGRID-1); ++i) {
	  glBegin(GL_QUAD_STRIP);
 	  for (j = 0; j < NGRID; ++j) {
		 glColor3f(z[i][j],z[i][j],z[i][j]);
		 glVertex3f(X(j),Y(i),0);
		 glColor3f(z[i+1][j],z[i+1][j],z[i+1][j]);
		 glVertex3f(X(j),Y(i+1),0);
	  }
	  glEnd();
	}
  glFlush();
  }

void idle(void) {
	int i,j;
	if ((k > KMAX) | (k < KMIN))
		dk = -dk;
	k += dk;
    for (i = 0; i < NGRID; ++i)
	  for (j = 0; j < NGRID; ++j) {
		 z[i][j] = height(X(j),Y(i));
	  }
 	glutSwapBuffers();
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	exit(0);
}

void main(int argc, char **argv) {
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutCreateWindow("glimage");
   glutDisplayFunc(display);
   glutMouseFunc(mouse);
   glutIdleFunc(idle);
   glShadeModel(GL_SMOOTH);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-1.0,1.0,-1.0,1.0,-1.0,1.0);
   glutMainLoop();
}
