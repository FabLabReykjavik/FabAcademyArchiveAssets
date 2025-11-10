/*
* ximage.c
* (c) Neil Gershenfeld  2/4/03
* demonstrates drawing X images by animating sin(k*x)/k*x
*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>
#include <stdio.h>

#define NPTS 250
#define WIDTH NPTS
#define HEIGHT NPTS
#define NCOLORS 256
#define DEPTH 24

Display  *D;
Visual   *V;
int      S,Loop,Point,i,j;
Window   W;
GC       Gc;
XImage   *I;
XVisualInfo Info;
char     Data[HEIGHT][WIDTH][4];
float    x,y,r,z;

main() {
   D = XOpenDisplay("");
   S = DefaultScreen(D);
   if (XMatchVisualInfo(D, S, DEPTH, TrueColor, &Info) == 0) {
      printf("Display does not support %d bit TrueColor Visual\n",DEPTH);
      return;
   }
   V = Info.visual;
   W = XCreateSimpleWindow(D, DefaultRootWindow(D), 0, 0, WIDTH, HEIGHT, 0, 0, 0);       
   XStoreName(D, W, "ximage output");
   XMapRaised(D, W);
   Gc = XCreateGC (D, W, 0L, (XGCValues *) 0);
   I = XCreateImage(D, V, DEPTH, ZPixmap, 0, (char *) Data, WIDTH, HEIGHT, 8, 0);
   for (Loop = 1; Loop <= NPTS; ++Loop) {
      for (i = 0; i < WIDTH; ++i)
         for (j = 0; j < HEIGHT; ++j) {
            x = (2.0*i + 1.0 - WIDTH)/WIDTH;
            y = (2.0*j + 1.0 - HEIGHT)/HEIGHT;
            r = Loop*20.0*sqrt(x*x + y*y)/NPTS;
            z = sin(r)/r;
            Data[j][i][0] = (unsigned char) (NCOLORS * (z + 0.3)/1.3);         	
            Data[j][i][1] = (unsigned char) (NCOLORS * (z + 0.3)/1.3);         	
            Data[j][i][2] = (unsigned char) (NCOLORS * (z + 0.3)/1.3);  
          }
      XPutImage(D, W, Gc, I, 0, 0, 0, 0, WIDTH, HEIGHT);
   }
}
