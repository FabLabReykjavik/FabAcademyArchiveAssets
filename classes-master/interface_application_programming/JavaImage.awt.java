//
// JavaImage.java
// (c) Neil Gershenfeld  9/1/97
// demonstrates Java by animating sin(k*r)/k*r
//

import java.awt.Graphics;
import java.awt.Color;
import java.awt.Image;
import java.awt.Frame;

public class JavaImage extends java.applet.Applet
   implements Runnable {
   
   static final int NPTS = 100;
   static final int NSTEPS = 100;
   static final int NGRAYS = 100;
   Thread T;
   Image I;
   Graphics G;
   Color ColorTable[] = new Color[NGRAYS];

  public static void main(String args[]) {
      Frame F = new Frame("JavaImage");
      F.setSize(2*NPTS,2*NPTS);
      F.show();
      JavaImage J = new JavaImage();
      F.add("Center",J);
      J.setSize(2*NPTS,2*NPTS);
      J.init();
      J.start();
      }
   public void init() {
      int i,gray;
      I = createImage(2*NPTS,2*NPTS);
      G = I.getGraphics();
      for (i = 0; i < NGRAYS; ++i) {
         gray = (int) (255.0 * ((double) i)/NGRAYS);
         ColorTable[i] = new Color(gray,gray,gray);
         }
      }
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
      double r, xr, yr;
      int gray, i, j, step;
      while (true) {
         for (step = 1; step < NSTEPS; ++step) {
            for (i = 0; i < NPTS; ++i) {
               for (j = 0; j < NPTS; ++j) {
                  xr = (2.0*i + 1.0 - NPTS)/NPTS;
                  yr = (2.0*j + 1.0 - NPTS)/NPTS;
                  r = step * 20.0 * Math.sqrt(xr*xr + yr*yr)/NSTEPS;
                  gray = (int) (NGRAYS*(0.5+Math.sin(r)/r)/1.5);
                  G.setColor(ColorTable[gray]);
                  G.fillRect(2*i,2*j,2,2);
                  }
               }
            repaint();
            try {Thread.sleep(10);}
            catch (InterruptedException e) { }
            }
         }
      }
   public synchronized void paint(Graphics g) {
      g.drawImage(I,0,0,this);
      }
   public void update(Graphics g) {
      paint(g);
      }
   }
