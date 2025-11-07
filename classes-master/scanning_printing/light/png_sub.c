//
// png_sub.c
//
// Neil Gershenfeld 1/21/14
// (c) Massachusetts Institute of Technology 2014
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge the fab modules project. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all 
// liability.
//
//
/*
gcc png_sub.c -o png_sub -lm -lpng
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <png.h>
#include <inttypes.h>

struct vars {
   unsigned int nx,ny,nz;
   unsigned int bit_depth;
   uint32_t **array;
   png_bytep *row_pointers;
   png_structp png_ptr;
   png_infop info_ptr;
   };

void png_array_K(struct vars *v) {
   //
   // copy PNG image to grayscale array
   //
   int intensity,value,x,y;
   int bit, byte;
   png_byte *ptr;
   png_colorp palette;
   png_color color;
   //png_uint_32 num_palette;
   int num_palette;
   if (png_get_bit_depth(v->png_ptr, v->info_ptr) == 1) {
      if (png_get_color_type(v->png_ptr, v->info_ptr) == PNG_COLOR_TYPE_PALETTE)
         for (y = 0; y < v->ny; ++y)
            for (x = 0; x < v->nx; ++x) {
               byte = x / 8;
               bit = 7 - x % 8;
               ptr = &(v->row_pointers[y][byte]);
               value = (ptr[0] >> bit) & 1;
               png_get_PLTE(v->png_ptr, v->info_ptr, &palette, &num_palette);
               color = palette[value];
               intensity = (color.red + color.green + color.blue)/3.0;
         	   v->array[y][x] = intensity;
         	   }
      else if (png_get_color_type(v->png_ptr, v->info_ptr) == PNG_COLOR_TYPE_GRAY)
         for (y = 0; y < v->ny; ++y)
            for (x = 0; x < v->nx; ++x) {
               byte = x / 8;
               bit = 7 - x % 8;
         		ptr = &(v->row_pointers[y][byte]);
               value = (ptr[0] >> bit) & 1;
         	   v->array[y][x] = value;
         	   }
      else {
         printf("oops 3 -- image type not supported\n");
         exit(-1);
         }
      }
   else if (png_get_bit_depth(v->png_ptr, v->info_ptr) == 8) {
      if (png_get_color_type(v->png_ptr, v->info_ptr) == PNG_COLOR_TYPE_GRAY)
         for (y = 0; y < v->ny; ++y)
            for (x = 0; x < v->nx; ++x) {
         		ptr = &(v->row_pointers[y][x*1]);
         	   v->array[y][x] = ptr[0];
         	   }
      else if (png_get_color_type(v->png_ptr, v->info_ptr) == PNG_COLOR_TYPE_GRAY_ALPHA)
         for (y = 0; y < v->ny; ++y)
            for (x = 0; x < v->nx; ++x) {
         		ptr = &(v->row_pointers[y][x*2]);
         	   v->array[y][x] = (int) ((1.0-ptr[1]/255.0)*255 + (0.0+(ptr[1]/255.0))*ptr[0]);
         	   }
      else if (png_get_color_type(v->png_ptr, v->info_ptr) == PNG_COLOR_TYPE_RGB)
         for (y = 0; y < v->ny; ++y)
            for (x = 0; x < v->nx; ++x) {
         		ptr = &(v->row_pointers[y][x*3]);
         	   v->array[y][x] = (int) ((ptr[0] + ptr[1] + ptr[2])/3.0);
         	   }
      else if (png_get_color_type(v->png_ptr, v->info_ptr) == PNG_COLOR_TYPE_RGB_ALPHA)
         for (y = 0; y < v->ny; ++y)
            for (x = 0; x < v->nx; ++x) {
         		ptr = &(v->row_pointers[y][x*4]);
         	   v->array[y][x] = (int) ((1.0-ptr[3]/255.0)*255 + (0.0+(ptr[3]/255.0))*((ptr[0] + ptr[1] + ptr[2])/3.0));
         	   }
      else if (png_get_color_type(v->png_ptr, v->info_ptr) == PNG_COLOR_TYPE_PALETTE) {
         for (y = 0; y < v->ny; ++y)
            for (x = 0; x < v->nx; ++x) {
         		value = v->row_pointers[y][x];
               png_get_PLTE(v->png_ptr, v->info_ptr, &palette, &num_palette);
               color = palette[value];
               intensity = (color.red + color.green + color.blue)/3.0;
         	   v->array[y][x] = intensity;
         	   }
         }
      else {
         printf("oops -- image type not supported\n");
         exit(-1);
         }
      }
   else if (png_get_bit_depth(v->png_ptr, v->info_ptr) == 16) {
      if (png_get_color_type(v->png_ptr, v->info_ptr) == PNG_COLOR_TYPE_GRAY)
         for (y = 0; y < v->ny; ++y)
            for (x = 0; x < v->nx; ++x) {
         		ptr = &(v->row_pointers[y][x*2]);
         	   v->array[y][x] = (ptr[0] << 8) + ptr[1];
         	   }
      else {
         printf("oops -- image type not supported\n");
         exit(-1);
         }
      }
   else {
      printf("oops -- image type not supported\n");
      exit(-1);
      }
   }

void read_png_K(struct vars *v, char *input_file_name) {
   //
   // read PNG into vars and convert to grayscale array
   //
	FILE *input_file;
   int y;
   png_uint_32 res_x,res_y;
   int unit_type;
   //
   // read PNG file
   //
   input_file = fopen(input_file_name, "rb");
   if (input_file == 0) {
      printf("oops -- can't open %s\n",input_file_name);
      exit(-1);
      }
   v->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                       NULL, NULL, NULL);
   v->info_ptr = png_create_info_struct(v->png_ptr);
   png_init_io(v->png_ptr, input_file);
   png_read_info(v->png_ptr, v->info_ptr);
   //
   // get size
   //
   v->nx = png_get_image_width(v->png_ptr, v->info_ptr);
   v->ny = png_get_image_height(v->png_ptr, v->info_ptr);
   v->bit_depth = png_get_bit_depth(v->png_ptr, v->info_ptr);
   //
   // set pixels
   //
	v->row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * v->ny);
	for (y = 0; y < v->ny; ++y)
		v->row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(v->png_ptr,
		                                                         v->info_ptr));
	png_read_image(v->png_ptr, v->row_pointers);
   //
   // allocate array
   //
   v->array = malloc(v->ny*sizeof(uint32_t *));
   for (y = 0; y < v->ny; ++y)
      v->array[y] = malloc(v->nx*sizeof(uint32_t));
   //
   // copy image to array
   //
   png_array_K(v);
   //
   // close and return
   //
   fclose(input_file);
   printf("read %s\n",input_file_name);
   printf("   bit depth: %d\n",v->bit_depth);
   printf("   x pixels: %d, y pixels: %d\n",v->nx,v->ny);
   }

void write_png_K(struct vars *v, char *output_file_name) {
   //
   // write grayscale PNG from vars
   //
	FILE *output_file;
   int x,y;
   int num_text=0;
   char xmins[100],ymins[100],zmins[100],zmaxs[100];
   png_text text_ptr[4];
   png_uint_32 res_x,res_y;
   png_byte color_type;
   png_byte bit_depth;
   png_byte *ptr;
   //
   // open PNG file
   //
	output_file = fopen(output_file_name, "wb");
	v->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
      NULL, NULL, NULL);
	v->info_ptr = png_create_info_struct(v->png_ptr);
	png_init_io(v->png_ptr, output_file);
   //
   // set vars
   //
   bit_depth = v->bit_depth;
   color_type = PNG_COLOR_TYPE_GRAY; 
	png_set_IHDR(v->png_ptr, v->info_ptr, v->nx, v->ny,
	   bit_depth, color_type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
   png_write_info(v->png_ptr, v->info_ptr);
   //
   // allocate pixels
   //
	v->row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * v->ny);
	for (y = 0; y < v->ny; ++y)
		v->row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(v->png_ptr,
		                                                         v->info_ptr));
   //
   // set pixels
   //
   if (bit_depth == 8) {
      for (y = 0; y < v->ny; ++y)
         for (x = 0; x < v->nx; ++x) {
      		ptr = &(v->row_pointers[y][x]);
      	   ptr[0] = v->array[y][x];
      	   }
         }
   else if (bit_depth == 16) {
      for (y = 0; y < v->ny; ++y)
         for (x = 0; x < v->nx; ++x) {
      		ptr = &(v->row_pointers[y][x*2]);
      	   ptr[0] = (v->array[y][x] >> 8) & 255;
      	   ptr[1] = v->array[y][x] & 255;
      	   }
         }
   else {
      printf("oops -- don't recognize bit depth %d\n",v->bit_depth);
      exit(-1);
      }
   //
   // write, close, and return
   //
	png_write_image(v->png_ptr, v->row_pointers);
	png_write_end(v->png_ptr, NULL);
   fclose(output_file);
   printf("write %s\n",output_file_name);
   printf("   bit depth: %d\n",v->bit_depth);
   printf("   x pixels: %d, y pixels: %d\n",v->nx,v->ny);
   }

int main(int argc, char **argv) {
   //
   // local vars
   //
   struct vars a,b;
   int x,y;
   //
   // command line args
   //
   if (argc != 4) {
      printf("command line: png_sub a.png b.png out.png\n");
      printf("   a.png = first input PNG file\n");
      printf("   b.png = second input PNG file\n");
      printf("   out.png = output PNG file\n");
      exit(-1);
      }
   //
   // read inputs
   //
   read_png_K(&a,argv[1]);
   read_png_K(&b,argv[2]);
   //
   // subtract images
   //
   for (y = 0; y < a.ny; ++y)
      for (x = 0; x < a.nx; ++x) {
         if ((a.array[y][x] > b.array[y][x]))
            a.array[y][x] = a.array[y][x] - b.array[y][x];
         else
            a.array[y][x] = 0;
         }
   //
   // write PNG
   //
   write_png_K(&a,argv[3]);
   exit(0);
   }
