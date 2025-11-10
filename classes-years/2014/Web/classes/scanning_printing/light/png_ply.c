//
// png_ply.c
//
// Neil Gershenfeld 1/18/14
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
gcc png_ply.c -o png_ply -lm -lpng
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
   struct mesh_type *mesh;
   png_bytep *row_pointers;
   png_structp png_ptr;
   png_infop info_ptr;
   };

struct mesh_type {
   struct mesh_triangle_type *first,*triangle,*last;
   float min[3];
   float max[3];
   int number;
   };

struct mesh_triangle_type {
   struct mesh_triangle_type *previous,*next;
   float normal[3],v0[3],v1[3],v2[3];
   int c0[3],c1[3],c2[3];
   int attribute;
   };

void png_array_RGB(struct vars *v) {
   //
   // copy PNG image to RGB array
   //
   int intensity,value,x,y;
   int bit, byte;
   png_byte *ptr;
   png_colorp palette;
   png_color color;
   //png_uint_32 num_palette;
   int num_palette;
   //
   // allocate array
   //
   v->array = malloc(v->ny*sizeof(uint32_t *));
   for (y = 0; y < v->ny; ++y)
      v->array[y] = malloc(3*v->nx*sizeof(uint32_t));
   //
   // handle image type
   //
   if (png_get_bit_depth(v->png_ptr, v->info_ptr) == 8) {
      if (png_get_color_type(v->png_ptr, v->info_ptr) == PNG_COLOR_TYPE_GRAY)
         for (y = 0; y < v->ny; ++y)
            for (x = 0; x < v->nx; ++x) {
         		ptr = &(v->row_pointers[y][x*1]);
         	   v->array[y][3*x] = ptr[0];
         	   v->array[y][3*x+1] = ptr[0];
         	   v->array[y][3*x+2] = ptr[0];
         	   }
      else if (png_get_color_type(v->png_ptr, v->info_ptr) == PNG_COLOR_TYPE_RGB)
         for (y = 0; y < v->ny; ++y)
            for (x = 0; x < v->nx; ++x) {
         		ptr = &(v->row_pointers[y][x*3]);
         	   v->array[y][3*x] = ptr[0];
         	   v->array[y][3*x+1] = ptr[1];
         	   v->array[y][3*x+2] = ptr[2];
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
   //
   // allocate array
   //
   v->array = malloc(v->ny*sizeof(uint32_t *));
   for (y = 0; y < v->ny; ++y)
      v->array[y] = malloc(v->nx*sizeof(uint32_t));
   //
   // handle image type
   //
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

void read_png(struct vars *v, char *input_file_name) {
   //
   // read PNG into vars
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
   // close and return
   //
   fclose(input_file);
   printf("read %s\n",input_file_name);
   printf("   bit depth: %d\n",v->bit_depth);
   printf("   x pixels: %d, y pixels: %d\n",v->nx,v->ny);
   }

void triangle(struct vars *v,float v0[3],int c0[3],float v1[3],int c1[3],float v2[3],int c2[3]) {
   //
   // add a triangle to mesh
   //
   v->mesh->last = malloc(sizeof(struct mesh_triangle_type));
   v->mesh->last->previous = v->mesh->triangle;
   v->mesh->triangle->next = v->mesh->last;
   v->mesh->triangle = v->mesh->last;
   v->mesh->triangle->next = 0;
   v->mesh->triangle->attribute = 0;
   v->mesh->triangle->normal[0] = v->mesh->triangle->normal[1] = v->mesh->triangle->normal[2] = 0;
   v->mesh->triangle->v0[0] = v0[0];
   v->mesh->triangle->v0[1] = v0[1];
   v->mesh->triangle->v0[2] = v0[2];
   v->mesh->triangle->v1[0] = v1[0];
   v->mesh->triangle->v1[1] = v1[1];
   v->mesh->triangle->v1[2] = v1[2];
   v->mesh->triangle->v2[0] = v2[0];
   v->mesh->triangle->v2[1] = v2[1];
   v->mesh->triangle->v2[2] = v2[2];
   v->mesh->triangle->c0[0] = c0[0];
   v->mesh->triangle->c0[1] = c0[1];
   v->mesh->triangle->c0[2] = c0[2];
   v->mesh->triangle->c1[0] = c1[0];
   v->mesh->triangle->c1[1] = c1[1];
   v->mesh->triangle->c1[2] = c1[2];
   v->mesh->triangle->c2[0] = c2[0];
   v->mesh->triangle->c2[1] = c2[1];
   v->mesh->triangle->c2[2] = c2[2];
   }

void write_ply(struct vars *v, char *output_file_name) {
   //
   // write mesh as PLY
   //
   FILE *output_file;
   char buf[80];
   uint32_t count;
   //
   // open output file
   //
   output_file = fopen(output_file_name, "wb");
   if (output_file == 0) {
      printf("oops -- can't open %s\n",output_file_name);
      exit(-1);
      }
   //
   // count triangles
   //
   count = 0;
   v->mesh->triangle = v->mesh->first;
   while (v->mesh->triangle->next != 0) {
      v->mesh->triangle = v->mesh->triangle->next;
      count += 1;
      }
   //
   // write header
   //
   fprintf(output_file,"ply\n");
   fprintf(output_file,"format ascii 1.0\n");
   fprintf(output_file,"element vertex %d\n",3*count);
   fprintf(output_file,"property float x\n");
   fprintf(output_file,"property float y\n");
   fprintf(output_file,"property float z\n");
   fprintf(output_file,"property uchar red\n");
   fprintf(output_file,"property uchar green\n");
   fprintf(output_file,"property uchar blue\n");
   fprintf(output_file,"element face %d\n",count);
   fprintf(output_file,"property list uchar int vertex_indices\n");
   fprintf(output_file,"end_header\n");
   //
   // write vertices 
   //
   v->mesh->triangle = v->mesh->first;
   while (v->mesh->triangle->next != 0) {
      v->mesh->triangle = v->mesh->triangle->next;
      fprintf(output_file,"%f %f %f %d %d %d\n",
         v->mesh->triangle->v0[0],
         v->mesh->triangle->v0[1],
         v->mesh->triangle->v0[2],
         v->mesh->triangle->c0[0],
         v->mesh->triangle->c0[1],
         v->mesh->triangle->c0[2]);
      fprintf(output_file,"%f %f %f %d %d %d\n",
         v->mesh->triangle->v1[0],
         v->mesh->triangle->v1[1],
         v->mesh->triangle->v1[2],
         v->mesh->triangle->c1[0],
         v->mesh->triangle->c1[1],
         v->mesh->triangle->c1[2]);
      fprintf(output_file,"%f %f %f %d %d %d\n",
         v->mesh->triangle->v2[0],
         v->mesh->triangle->v2[1],
         v->mesh->triangle->v2[2],
         v->mesh->triangle->c2[0],
         v->mesh->triangle->c2[1],
         v->mesh->triangle->c2[2]);
      }
   //
   // write faces
   //
   v->mesh->triangle = v->mesh->first;
   count = 0;
   while (v->mesh->triangle->next != 0) {
      v->mesh->triangle = v->mesh->triangle->next;
      fprintf(output_file,"3 %d %d %d\n",count,count+1,count+2,count+3);
      count += 3;
      }
   //
   // return
   //
   printf("wrote %d triangles to %s\n",count,output_file_name);
   fclose(output_file);
   }

int main(int argc, char **argv) {
   //
   // local vars
   //
   struct vars v,map;
   int x,y;
   float v00[3],v01[3],v10[3],v11[3];
   int c00[3],c01[3],c10[3],c11[3];
   float scale;
   //
   // command line args
   //
   if (argc != 5) {
      printf("command line: png_ply height.png texture.png out.ply scale\n");
      printf("   height.png = input height map\n");
      printf("   texture.png = input texture map\n");
      printf("   out.ply = output PLY file\n");
      printf("   scale = z scale\n");
      exit(-1);
      }
   sscanf(argv[4],"%f",&scale);
   //
   // read input
   //
   read_png(&v,argv[1]);
   png_array_K(&v);
   read_png(&map,argv[2]);
   png_array_RGB(&map);
   //
   // generate mesh
   //
   v.mesh = malloc(sizeof(struct mesh_type));
   v.mesh->triangle = malloc(sizeof(struct mesh_triangle_type));
   v.mesh->first = v.mesh->triangle;
   v.mesh->last = v.mesh->triangle;
   v.mesh->triangle->previous = v.mesh->triangle->next = 0;
   for (y = 0; y < (v.ny-1); ++y) {
      for (x = 0; x < (v.nx-1); ++x) {
         v00[0] = x+1;
         v00[1] = v.ny-y;
         v00[2] = scale * (int) v.array[y][x];
         c00[0] = map.array[y][3*x];
         c00[1] = map.array[y][3*x+1];
         c00[2] = map.array[y][3*x+2];
         v01[0] = x;
         v01[1] = v.ny-y;
         v01[2] = scale * (int) v.array[y][x+1];
         c01[0] = map.array[y][3*(x+1)];
         c01[1] = map.array[y][3*(x+1)+1];
         c01[2] = map.array[y][3*(x+1)+2];
         v10[0] = x+1;
         v10[1] = v.ny-y-1;
         v10[2] = scale * (int) v.array[y+1][x];
         c10[0] = map.array[y+1][3*x];
         c10[1] = map.array[y+1][3*x+1];
         c10[2] = map.array[y+1][3*x+2];
         v11[0] = x;
         v11[1] = v.ny-y-1;
         v11[2] = scale * (int) v.array[y+1][x+1];
         c11[0] = map.array[y+1][3*(x+1)];
         c11[1] = map.array[y+1][3*(x+1)+1];
         c11[2] = map.array[y+1][3*(x+1)+2];
         triangle(&v,v00,c00,v01,c01,v10,c10);
         triangle(&v,v01,c01,v11,c01,v10,c10);
         }
      }
   //
   // write PLY
   //
   write_ply(&v,argv[3]);
   exit(0);
   }
