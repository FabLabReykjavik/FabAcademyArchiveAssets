/*
* stl.c
* (c) Neil Gershenfeld  2/6/11
* example of sin(kr)/kr STL output
*/

#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define N 100
#define dz .1
#define scale 20.0

void write_triangle(float x0, float x1, float x2,
   /*
   * write triangle
   */
   float y0, float y1, float y2,
   float z0, float z1, float z2,
   FILE *output_file) {
   float zero = 0;
   uint16_t attribute;
   fwrite(&zero,4,1,output_file);
   fwrite(&zero,4,1,output_file);
   fwrite(&zero,4,1,output_file);
   fwrite(&x0,4,1,output_file);
   fwrite(&y0,4,1,output_file);
   fwrite(&z0,4,1,output_file);
   fwrite(&x1,4,1,output_file);
   fwrite(&y1,4,1,output_file);
   fwrite(&z1,4,1,output_file);
   fwrite(&x2,4,1,output_file);
   fwrite(&y2,4,1,output_file);
   fwrite(&z2,4,1,output_file);
   fwrite(&attribute,2,1,output_file);
   }

main() {
   int i,j;
   float x0,x1,y0,y1,r00,r01,r10,r11,
      z00,z01,z10,z11;
   char header[80];
   uint32_t count;
   FILE *output_file;
   output_file = fopen("out.stl","wb");
   fwrite(header,1,80,output_file);
   count = 2*2*(N-1)*(N-1) + 2*2*2*(N-1);
   fwrite(&count,4,1,output_file);
   for (j = 0; j < (N-1); ++j) {
      x0 = (2.0*0 + 1 - N)/N;
      x1 = (2.0*(N-1) + 1 - N)/N;
      y0 = (2.0*j + 1 - N)/N;
      y1 = (2.0*(j+1) + 1 - N)/N;
      r00 = scale*sqrt(x0*x0 + y0*y0);
      r01 = scale*sqrt(x0*x0 + y1*y1);
      r10 = scale*sqrt(x1*x1 + y0*y0);
      r11 = scale*sqrt(x1*x1 + y1*y1);
      z00 = sin(r00)/r00;
      z01 = sin(r01)/r01;
      z10 = sin(r10)/r10;
      z11 = sin(r11)/r11;
      /*
      * left
      */
      write_triangle(x0,x0,x0,
         y0,y1,y1,
         z00-dz,z10,z00-dz,
         output_file);
      write_triangle(x0,x0,x0,
         y0,y0,y1,
         z00-dz,z10,z10,
         output_file);
      /*
      * right
      */
      write_triangle(x1,x1,x1,
         y0,y1,y1,
         z01-dz,z01-dz,z11,
         output_file);
      write_triangle(x1,x1,x1,
         y0,y1,y0,
         z01-dz,z11,z11,
         output_file);
      }
   for (i = 0; i < (N-1); ++i) {
      x0 = (2.0*i + 1 - N)/N;
      x1 = (2.0*(i+1) + 1 - N)/N;
      y0 = (2.0*0 + 1 - N)/N;
      y1 = (2.0*(N-1) + 1 - N)/N;
      r00 = scale*sqrt(x0*x0 + y0*y0);
      r01 = scale*sqrt(x0*x0 + y1*y1);
      r10 = scale*sqrt(x1*x1 + y0*y0);
      r11 = scale*sqrt(x1*x1 + y1*y1);
      z00 = sin(r00)/r00;
      z01 = sin(r01)/r01;
      z10 = sin(r10)/r10;
      z11 = sin(r11)/r11;
      /*
      * front
      */
      write_triangle(x0,x1,x0,
         y0,y0,y0,
         z00-dz,z10-dz,z00,
         output_file);
      write_triangle(x0,x1,x1,
         y0,y0,y0,
         z00,z10-dz,z10,
         output_file);
      /*
      * back
      */
      write_triangle(x0,x0,x1,
         y1,y1,y1,
         z01-dz,z01,z11-dz,
         output_file);
      write_triangle(x0,x1,x1,
         y1,y1,y1,
         z01,z11,z11-dz,
         output_file);
      for (j = 0; j < (N-1); ++j) {
         x0 = (2.0*i + 1 - N)/N;
         x1 = (2.0*(i+1) + 1 - N)/N;
         y0 = (2.0*j + 1 - N)/N;
         y1 = (2.0*(j+1) + 1 - N)/N;
         r00 = scale*sqrt(x0*x0 + y0*y0);
         r01 = scale*sqrt(x0*x0 + y1*y1);
         r10 = scale*sqrt(x1*x1 + y0*y0);
         r11 = scale*sqrt(x1*x1 + y1*y1);
         z00 = sin(r00)/r00;
         z01 = sin(r01)/r01;
         z10 = sin(r10)/r10;
         z11 = sin(r11)/r11;
         /*
         * top
         */
         write_triangle(x0,x1,x0,
            y0,y0,y1,
            z00,z10,z01,
            output_file);
         write_triangle(x0,x1,x1,
            y1,y0,y1,
            z01,z10,z11,
            output_file);
         /*
         * bottom
         */
         write_triangle(x0,x0,x1,
            y0,y1,y0,
            z00-dz,z01-dz,z10-dz,
            output_file);
         write_triangle(x0,x1,x1,
            y1,y1,y0,
            z01-dz,z11-dz,z10-dz,
            output_file);
         }
      }
   fclose(output_file);
   }
