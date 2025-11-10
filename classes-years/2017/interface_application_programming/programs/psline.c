/*
* psline.c   Neil Gershenfeld   9/11/93
* demonstrates drawing lines in PostScript by drawing sin(k*x)/k*x
*/

#include <math.h>
#include <stdio.h>

int i;
float x,y;
FILE *outfile;

#define NPTS 1000
#define PAGE_WIDTH 8
#define PAGE_HEIGHT 10

main() {
   outfile = fopen("psline.eps","w");
   fprintf(outfile,"%%! psline output\n");
   fprintf(outfile,"%%%%BoundingBox: 0 0 %f %f\n",
      72.0*PAGE_WIDTH,72.0*PAGE_HEIGHT);
   fprintf(outfile,"/l {lineto} def\n");
   fprintf(outfile,"%f %f scale\n",72.0*PAGE_WIDTH*0.5,72.0*PAGE_HEIGHT*0.5);
   fprintf(outfile,"90 rotate\n");
   fprintf(outfile,"1 -1 translate\n");
   fprintf(outfile,"0.5 setgray 0.02 setlinewidth\n");
   x = (1.0 - NPTS)/NPTS;
   y = sin(50.0*x)/(50.0*x);
   fprintf(outfile,"%.3f %.3f moveto\n",x,y);
   for (i = 1; i < NPTS; ++i) {
      x = (2.0*i + 1.0 - NPTS)/NPTS;
      y = sin(50.0*x)/(50.0*x);
      fprintf(outfile,"%.3f %.3f l\n",x,y);
   }
   fprintf(outfile,"stroke\n");
/* fprintf(outfile,"showpage\n"); */
   fclose(outfile);
}
