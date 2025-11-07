//
// JavaLine.java
// (c) Neil Gershenfeld  2/6/03
// demonstrates Java by animating sin(k*x)/k*x
//

import java.awt.*;
import java.awt.geom.*;
import javax.swing.*;

public class JavaLine extends JApplet implements Runnable {
	Thread T;
	final int NPTS = 500;
	final int NSTEPS = 100;
	int point,step;
	int x[] = new int[NPTS];
	int y[] = new int[NPTS];
	
	class LinePanel extends JPanel {
		public void paintComponent(Graphics g) {
			super.paintComponent(g);
			super.setBackground(Color.white);
			Graphics2D g2d = (Graphics2D) g;
			GeneralPath path = new GeneralPath(GeneralPath.WIND_EVEN_ODD,NPTS);
			path.moveTo(x[0],y[0]);
			for (point = 1; point < (NPTS-1); ++point) {
				path.lineTo(x[point],y[point]);
			}
			g2d.draw(path);  
		}
	}
	
	public void init() {
		Container C = getContentPane();
		C.add(new LinePanel());
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
}

