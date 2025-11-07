#
# glimage.py
# (c) Neil Gershenfeld  2/10/08
# animate sin(r)/r image with GLUT
#

from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
from numpy import *
import sys

NGRID = 50
KMIN = 0.0
KMAX = 20.0
k=0.0
dk=0.2

def X(j): return (2.0*(j + 0.5)/NGRID - 1.0)
def Y(i): return (2.0*(i + 0.5)/NGRID - 1.0)

pts = arange(-1.0,1.0,2.0/NGRID)
(x,y) = meshgrid(pts,pts)
r = sqrt(x*x + y*y)
z = ones(r.shape)

def display():
   global x,y,z
   for i in range(NGRID-1):
      glBegin(GL_QUAD_STRIP)
      for j in range(NGRID):
         glColor3f(z[i][j],z[i][j],z[i][j])
         glVertex3f(X(j),Y(i),0)
         glColor3f(z[i+1][j],z[i+1][j],z[i+1][j])
         glVertex3f(X(j),Y(i+1),0)
      glEnd()
   glFlush()

def idle():
   global k,dk,x,y,z,r
   if ((k > KMAX) | (k < KMIN)):
      dk = -dk
   k += dk
   z = sin(k*r)/(k*r)
   glutSwapBuffers()
   glutPostRedisplay()

def mouse(*args):
   sys.exit()

glutInit(())
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH)
glutCreateWindow("glimage")
glutFullScreen()
glutDisplayFunc(display)
glutMouseFunc(mouse)
glutIdleFunc(idle)
glShadeModel(GL_SMOOTH)
glMatrixMode(GL_PROJECTION)
glLoadIdentity()
glOrtho(-1.0,1.0,-1.0,1.0,-1.0,1.0)
glutMainLoop()

