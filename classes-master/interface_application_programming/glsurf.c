/*
* glsurf.c
* (c) Neil Gershenfeld  2/4/03
* example of GL and GLUT, drawing sin(kr)/kr surface
*/

#include <GL/glut.h>
#include <math.h>
#include <unistd.h>

void normal(GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,
			GLfloat,GLfloat,GLfloat,GLfloat*,GLfloat*,GLfloat*);

#define NGRID 100
#define KMIN 0.0
#define KMAX 20.0
float k=0.0, dk=0.2;

#define r(x,y) (k*sqrt(x*x+y*y))
#define height(x,y) (sin(r(x,y))/r(x,y))

GLfloat x[NGRID][NGRID],y[NGRID][NGRID],z[NGRID][NGRID];

void display(void) {
   int i,j;
   GLfloat nx,ny,nz;
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glBegin(GL_QUADS);
   for (i = 0; i < (NGRID-1); ++i) {
 	  for (j = 0; j < (NGRID-1); ++j) {
         normal(x[i][j],y[i][j],z[i][j],
			    x[i+1][j],y[i+1][j],z[i+1][j],
				x[i][j+1],y[i][j+1],z[i][j+1],
				&nx,&ny,&nz);
         glNormal3f(nx,ny,nz);
		 glVertex3f(x[i][j],y[i][j],z[i][j]);
		 glVertex3f(x[i+1][j],y[i+1][j],z[i+1][j]);
		 glVertex3f(x[i+1][j+1],y[i+1][j+1],z[i+1][j+1]);
		 glVertex3f(x[i][j+1],y[i][j+1],z[i][j+1]);
		}
	}
  glEnd();
  glFlush();
  }

void idle(void) {
	GLfloat rx,ry,rz;
	int i,j;
	if ((k > KMAX) | (k < KMIN))
		dk = -dk;
	k += dk;
   for (i = 0; i < NGRID; ++i)
	  for (j = 0; j < NGRID; ++j) {
	     x[i][j] = 2.0*((float) j + 0.5)/NGRID - 1.0;
         y[i][j] = 2.0*((float) i + 0.5)/NGRID - 1.0;
		 z[i][j] = height(x[i][j],y[i][j]);
	  }
	glMatrixMode(GL_MODELVIEW);
	rx = rand();
	ry = rand();
	rz = rand();
	glRotatef(1.0,rz,ry,rz);
 	glutSwapBuffers();
	glutPostRedisplay();
	usleep(100);
}

void mouse(int button, int state, int x, int y) {
	exit(0);
}

void normal(GLfloat x1, GLfloat y1, GLfloat z1,
			GLfloat x2, GLfloat y2, GLfloat z2,
			GLfloat x3, GLfloat y3, GLfloat z3,
			GLfloat *xn, GLfloat *yn, GLfloat *zn) {
   *xn = (y2-y1)*(z3-z1) - (z2-z1)*(y3-y1);
   *yn = (z2-z1)*(x3-x1) - (x2-x1)*(z3-z1);
   *zn = (x2-x1)*(y3-y1) - (y2-y1)*(x3-x1);
}

void main(int argc, char **argv) {
   GLfloat matl_ambient[] = {.25, .22, .06, 1.0};
   GLfloat matl_diffuse[] = {.35, .31, .09, 1.0};
   GLfloat matl_specular[] = {.80, .72, .21, 1.0};
   GLfloat light_ambient[] = {0.9, 0.9, 0.9, 1.0};
   GLfloat light_diffuse[] = {0.8, 0.8, 0.8, 1.0};
   GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
   GLfloat light_position[] = {0,0,1, 1.0};

   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutCreateWindow("GLUT sin(kr)/kr example");
   glutDisplayFunc(display);
   glutMouseFunc(mouse);
   glutIdleFunc(idle);
   glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,matl_ambient);
   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,matl_diffuse);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,matl_specular);
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, 95.0);
   glEnable(GL_LIGHTING);
   glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light_ambient);
   glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
   glLightfv(GL_LIGHT0,GL_POSITION,light_position);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glClearColor(1.0,1.0,1.0,1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-1.5,1.5,-1.5,1.5,-1.5,1.5);
   glutMainLoop();
}
