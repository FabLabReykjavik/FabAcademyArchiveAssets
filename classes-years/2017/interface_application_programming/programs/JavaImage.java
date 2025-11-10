//
// JavaImage.java
// (c) Neil Gershenfeld  2/6/03
// demonstrates Java by animating sin(k*r)/k*r
//


import java.awt.*;
import java.awt.image.*;
import javax.swing.*;

import java.awt.geom.*;

public class JavaImage extends JApplet implements Runnable {

    static final int NPTS = 200;
    static final int NSTEPS = 100;
    static final int XBORDER = 5;
    static final int YBORDER = 20;
    boolean stopped = false;
    Thread T;
    WritableRaster R;
    BufferedImage I;

    public static void main(String args[]) {
	JavaImage J = new JavaImage();
	JFrame F = new JFrame("JavaImage");
	F.setSize(NPTS+XBORDER,NPTS+YBORDER);
	J.init();
	F.getContentPane().add(J);
	F.setVisible(true);
	J.start();
    }

    class ImagePanel extends JPanel {
	public void paintComponent(Graphics g) {
	    super.paintComponent(g);
	    Graphics2D g2d = (Graphics2D) g;
	    g2d.drawImage(I, 0, 0, this);
	}
    }

    public void init() {
	Container C = getContentPane();
	C.add(new ImagePanel());
	I = new BufferedImage(NPTS,NPTS,BufferedImage.TYPE_INT_RGB);
	R = I.getRaster();
    }

    public void start() {
	if (T == null) {
	    T = new Thread(this);
	    T.start();
	}
    }

    public void stop() {
	stopped = true;
    }

    public void run() {
	double xr, yr, r;
	int gray;
	int step;
	while (stopped == false) {
	    for (step = 1; step < NSTEPS; ++step) {
		for (int i = 0; i < NPTS; i++) {
		    for (int j = 0; j < NPTS; j++) {
			xr = (2.0*i + 1.0 - NPTS)/NPTS;
			yr = (2.0*j + 1.0 - NPTS)/NPTS;
			r = step* 20.0 * Math.sqrt(xr*xr + yr*yr)/NSTEPS;
			gray = (int) (255*(0.5+Math.sin(r)/r)/1.5);
			int[] color = {gray, gray, gray};
			R.setPixel(i,j,color);
		    }
		}
		repaint();
		try {Thread.sleep(10);}
		catch (InterruptedException e) { };
	    }
	}
    }
}
