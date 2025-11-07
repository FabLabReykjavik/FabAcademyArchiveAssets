//
// JavaImage.java
// (c) Neil Gershenfeld  2/6/03
// demonstrates Java by animating sin(k*r)/k*r
//


import java.awt.*;
import java.awt.image.*;
import javax.swing.*;
//import java.awt.image.BufferedImage;

import java.awt.geom.*;

public class JavaImage extends JApplet implements Runnable {
   
   static final int NPTS = 100;
   static final int NSTEPS = 100;
   static final int NGRAYS = 100;
   Thread T;
   WritableRaster R;
   BufferedImage I;
   DataBufferInt B;
   Graphics G;
   Color ColorTable[] = new Color[NGRAYS];
   
   public static void main(String args[]) {
	  JavaImage J = new JavaImage();
	  JFrame F = new JFrame("JavaImage");
	  F.setSize(2*NPTS,2*NPTS);
	  J.init();
	  F.getContentPane().add(J);
	  F.setVisible(true);
	  J.start();
   }
   
   class ImagePanel extends JPanel {
	   public void paintComponent(Graphics g) {
		   super.paintComponent(g);
		   Graphics2D g2d = (Graphics2D) g;
//		   ColorModel model = I.getColorModel();
//		   Color imagecolor = Color.red;
//		   int argb = imagecolor.getRGB();
//		   Object colordata = model.getDataElements(argb, null);
//		   int[] bleu = { 123, 234, 255};
		   // rects?
		   double xr, yr, r;
		   int gray;
		   int step;
		   for (step = 1; step < NSTEPS; ++step) {
			   System.out.println(step);
			   for (int i = 0; i < NPTS; i++) {
				   for (int j = 0; j < NPTS; j++) {
					   xr = (2.0*i + 1.0 - NPTS)/NPTS;
					   yr = (2.0*j + 1.0 - NPTS)/NPTS;
					   r = step* 20.0 * Math.sqrt(xr*xr + yr*yr)/NSTEPS;
					   gray = (int) (255*(0.5+Math.sin(r)/r)/1.5);
					   g2d.setColor(new Color(gray, gray, gray));
					   g2d.drawRect(2*i,2*j,2,2);
				   }
			   }
		   }
//		   g2d.drawImage(I, 0, 0, null);		   
	   }
   }
//				   R.setPixel(i,j,(byte) 100);
//			   R.setPixel( i, j, bleu);
//				   R.setDataElements(i,j,colordata);
//				   R.setDataElements(i,j,bleu);
		   //R.setPixel(11,21,128);
   
   public void init() {
	   Container C = getContentPane();
	   C.add(new ImagePanel());
	   I = new BufferedImage(NPTS,NPTS,BufferedImage.TYPE_BYTE_GRAY);
	   R = I.getRaster();
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
	   while (true) {
		   repaint();
		   try {Thread.sleep(10);}
		   catch (InterruptedException e) { };
	   }
   }

/*      
   public synchronized void paint(Graphics g) {
      double r, xr, yr;
      int gray, i, j, step;
      Graphics2D g2d = (Graphics2D) g;
//      BufferedImage I = new BufferedImage(TYPE_BYTE_GRAY
      BufferedImage I = (BufferedImage) createImage(100,100);
      //bufferedimage raster data write writableraster databuffer
      //getraster
Graphics2D Ig = I.createGraphics();
        //Ig.drawImage(I, 0, 0, null);
	 Ig.setFont(new Font("Dialog", Font.PLAIN, 10));
   Ig.setColor(Color.black);
   Ig.drawString("BufferedImage", 4, 12);
        for (step = 1; step < NSTEPS; ++step) {
            for (i = 0; i < NPTS; ++i) {
               for (j = 0; j < NPTS; ++j) {
                  xr = (2.0*i + 1.0 - NPTS)/NPTS;
                  yr = (2.0*j + 1.0 - NPTS)/NPTS;
                  r = step * 20.0 * Math.sqrt(xr*xr + yr*yr)/NSTEPS;
                  gray = (int) (NGRAYS*100*(0.5+Math.sin(r)/r)/1.5);
		  //I.setRGB(i,j,0xffff3412);
		  I.setRGB(i,j,gray);		  
                  }
               }
        g2d.drawImage(I, 0, 0, this); 
       }
   }
*/
/*       
   public void update(Graphics g) {
      paint(g);
      }
*/      
   }
