/*
* xline.c
* (c) Neil Gershenfeld  2/4/03
* demonstrates drawing X lines by animating sin(k*x)/k*x
*/

#include <X11/Xlib.h>
#include <math.h>
#include <unistd.h>

Display *D;
int     S,Loop,Point;
Window  W;
GC      Gc,GcRev;
XEvent  Event;
XPoint  PointBuf[1000],OldPointBuf[1000];
float   r;

#define NPTS 500
#define WIDTH NPTS
#define HEIGHT NPTS

main() {
   D = XOpenDisplay("");
   S = DefaultScreen(D);
   W = XCreateSimpleWindow(D, DefaultRootWindow(D),
       0, 0, WIDTH,HEIGHT, 1, WhitePixel(D,S), WhitePixel(D,S));
   XStoreName(D, W, "xline output");
   XMapRaised(D, W);
   Gc = XCreateGC (D, W, 0L, (XGCValues *) 0);
   XSetForeground(D, Gc, BlackPixel(D, S));
   XSetBackground(D, Gc, WhitePixel(D, S));
   XSetLineAttributes(D, Gc, 0, LineSolid, CapButt, JoinMiter);
   GcRev = XCreateGC (D, W, 0L, (XGCValues *) 0);
   XSetForeground(D, GcRev, WhitePixel(D, S));
   XSetBackground(D, GcRev, BlackPixel(D, S));
   XSetLineAttributes(D, GcRev, 0, LineSolid, CapButt, JoinMiter);
   for (Loop = 1; Loop <= NPTS; ++Loop) {
      for (Point = 0; Point < NPTS; ++Point) {
         r = 0.1 * (2.0*Point + 1.0 - NPTS)/NPTS;
         OldPointBuf[Point].x = PointBuf[Point].x;
         OldPointBuf[Point].y = PointBuf[Point].y;
         PointBuf[Point].x = Point;
         PointBuf[Point].y = (int) (NPTS * (1.0 - sin(Loop*r) /
            (Loop*r)) / 2.0);
         }
      XDrawLines(D, W, GcRev, OldPointBuf, NPTS, CoordModeOrigin);
      XDrawLines(D, W, Gc, PointBuf, NPTS, CoordModeOrigin);
      XFlush(D);
     usleep(10000);
   }
}
