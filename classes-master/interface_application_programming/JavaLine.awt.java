//
// JavaLine.java
// (c) Neil Gershenfeld  9/1/97
// demonstrates Java by animating sin(k*x)/k*x
//

import java.awt.Graphics;

public class JavaLine extends java.applet.Applet
   implements Runnable {

   Thread T;
   final int NPTS = 500;
   final int NSTEPS = 100;
   int point,step;
   int x[] = new int[NPTS];
   int y[] = new int[NPTS];

public void start() {
      if (T == null) {
         T = new Thread(this);
         T.start();
         }
      }
   public void stop() {
      if (T != null) {
         T = null;
         }
      }
   public void run() {
      double r;
      while (true) {
         for (step = 1; step < NSTEPS; ++step) {
            for (point = 0; point < (NPTS-1); ++point) {
	       r = 100 * (step*(point+0.5-NPTS/2))/(NPTS*NSTEPS);
	       x[point] = point;
	       y[point] = (int) ((NPTS/2) - (NPTS/2)*Math.sin(r)/r);
               }
            repaint();
            try {Thread.sleep(10);}
            catch (InterruptedException e) { }
	    }
         }
      }
   public void paint(Graphics g) {
      for (point = 1; point < (NPTS-1); ++point) {
         g.drawLine(x[point-1],y[point-1],x[point],y[point]);
	 }
      }
   }
