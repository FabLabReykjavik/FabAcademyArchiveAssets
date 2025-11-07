/*
* psimage.c  Neil Gershenfeld  9/11/93
* demonstrates drawing PostScript images by animating sin(k*x)/k*x
*/

#include <math.h>
#include <stdio.h>

int i,j;
float x,y,r,z;
unsigned char grey;
FILE *outfile;

#define NPTS 100
#define PAGE_WIDTH 8.0

main() {
   outfile = fopen("psimage.eps","w");
   fprintf(outfile,"%%! psimage output\n");
   fprintf(outfile,"%%%%BoundingBox: 0 0 %f %f\n",
      72.0*PAGE_WIDTH,72.0*PAGE_WIDTH);
   fprintf(outfile,"%f %f scale\n",72.0*PAGE_WIDTH,72.0*PAGE_WIDTH);
   fprintf(outfile,"%d %d 8 [%d 0 0 %d 0 0] {<\n",NPTS,NPTS,NPTS,NPTS);
   for (i = 0; i < NPTS; ++i)
      for (j = 0; j < NPTS; ++j) {
        x = (2.0*i + 1.0 - NPTS)/NPTS;
        y = (2.0*j + 1.0 - NPTS)/NPTS;
        r = 20.0*sqrt(x*x + y*y);
        z = sin(r)/r;
        grey = (unsigned char) (255.0 * (z + 0.3)/1.3);
        fprintf(outfile,"%.2x",grey);
   }
   fprintf(outfile,">} image\n");
/* fprintf(outfile,"showpage\n");*/
   fclose(outfile);
}
