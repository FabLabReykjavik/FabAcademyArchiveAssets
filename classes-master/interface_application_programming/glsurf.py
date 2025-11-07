#
# glsurf.py
# (c) Neil Gershenfeld  2/10/08
# animate sin(r)/r surface with GLUT
#

from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
from numpy import *
from random import *
import sys

NGRID = 50
KMIN = 0.0
KMAX = 20.0
k=0.0
dk=0.2

pts = arange(-1.0,1.0,2.0/NGRID)
(x,y) = meshgrid(pts,pts)
r = sqrt(x*x + y*y)
z = ones(r.shape)

def display():
   global x,y,z
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
   glBegin(GL_QUADS)
   for i in range(NGRID-1):
      for j in range(NGRID-1):
	 (nx, ny, nz) = normal(x[i][j],y[i][j],z[i][j],
            x[i+1][j],y[i+1][j],z[i+1][j],
  	    x[i][j+1],y[i][j+1],z[i][j+1])
         glNormal3f(nx,ny,nz)
	 glVertex3f(x[i][j],y[i][j],z[i][j])
	 glVertex3f(x[i+1][j],y[i+1][j],z[i+1][j])
	 glVertex3f(x[i+1][j+1],y[i+1][j+1],z[i+1][j+1])
	 glVertex3f(x[i][j+1],y[i][j+1],z[i][j+1])
   glEnd()
   glFlush()

def idle():
   global k,dk,x,y,z,r
   if ((k > KMAX) | (k < KMIN)):
      dk = -dk
   k += dk
   z = sin(k*r)/(k*r)
   glMatrixMode(GL_MODELVIEW)
   rx = random()
   ry = random()
   rz = random()
   glRotatef(1.0,rz,ry,rz)
   glutSwapBuffers()
   glutPostRedisplay()

def mouse(*args):
   sys.exit()

def normal(x1, y1, z1, x2, y2, z2, x3, y3, z3):
   xn = (y2-y1)*(z3-z1) - (z2-z1)*(y3-y1)
   yn = (z2-z1)*(x3-x1) - (x2-x1)*(z3-z1)
   zn = (x2-x1)*(y3-y1) - (y2-y1)*(x3-x1)
   return (xn, yn, zn)

matl_ambient = (.25, .22, .06, 1.0)
matl_diffuse = (.35, .31, .09, 1.0)
matl_specular = (.80, .72, .21, 1.0)
light_ambient = (0.9, 0.9, 0.9, 1.0)
light_diffuse = (0.8, 0.8, 0.8, 1.0)
light_specular = (1.0, 1.0, 1.0, 1.0)
light_position = (0, 0, 1, 1.0)

glutInit(())
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH)
glutInitWindowSize(500, 500)
glutCreateWindow("GLUT sin(kr)/kr example")
glutDisplayFunc(display)
glutMouseFunc(mouse)
glutIdleFunc(idle)
glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,matl_ambient)
glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,matl_diffuse)
glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,matl_specular)
glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, 95.0)
glEnable(GL_LIGHTING)
glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0)
glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light_ambient)
glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient)
glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse)
glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular)
glLightfv(GL_LIGHT0,GL_POSITION,light_position)
glEnable(GL_LIGHT0)
glEnable(GL_DEPTH_TEST)
glEnable(GL_NORMALIZE)
glClearColor(1.0,1.0,1.0,1.0)
glMatrixMode(GL_PROJECTION)
glLoadIdentity()
glOrtho(-1.5,1.5,-1.5,1.5,-1.5,1.5)
glutMainLoop()
