//
// coord.c
//
// Neil Gershenfeld 1/16/14
// (c) Massachusetts Institute of Technology 2014
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge the fab modules project. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all 
// liability.
//
/*
gcc coord.c -o coord -I /usr/include/opencv -lm -lX11 -lopencv_core -lopencv_highgui -lopencv_imgproc
*/

//
// defines
//
/*
camera settings moved to command line
#define FOCUS_ABSOLUTE 0
#define EXPOSURE 100
//#define EXPOSURE_TYPE V4L2_EXPOSURE_MANUAL
#define EXPOSURE_TYPE V4L2_EXPOSURE_APERTURE_PRIORITY
#define BRIGHTNESS 50
#define CONTRAST 50
#define DISPLAY_BRIGHTNESS "xrandr --output HDMI1 --brightness .4"
#define ZOOM "v4l2-ctl -d /dev/video1 -c zoom_absolute=200"
*/
#define DELAY_TIME 0.1
#define DEPTH 8
#define IMAGE_WIDTH 960
#define IMAGE_HEIGHT 720
/*
640x480
800x600
960x720
1280x720
1920x1080
*/
//
// includes
//
#include <X11/Xlib.h>
#include <cv.h>
#include <highgui.h>
#include <linux/videodev2.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h> 
//
// X
//
#define MWM_HINTS_FUNCTIONS    (1L << 0)
#define MWM_HINTS_DECORATIONS   (1L << 1)
#define MWM_HINTS_INPUT_MODE    (1L << 2)
#define MWM_FUNC_MOVE           (1L << 2)
#define MWM_INPUT_MODELESS      0
//
// global variables
//
uint8_t *memory_buffer;
struct v4l2_buffer video_buffer;
struct v4l2_requestbuffers request_buffer;
IplImage* frame;
CvMat cvmat;
//
// init camera capture
//
int capture_init(int fd) {
   struct v4l2_capability cap = {0};
   struct v4l2_fmtdesc fmtd = {0};
   struct v4l2_format fmt = {0};
   struct v4l2_control ctrl = {0};
   //
   // query capabilities
   //
   if (-1 == ioctl(fd, VIDIOC_QUERYCAP, &cap)) {
      perror("querying capabilities");
      return 1;
      }
   printf("capabilities:\n"
          "  driver: \"%s\"\n"
          "  card: \"%s\"\n"
          "  bus: \"%s\"\n",
           cap.driver,
           cap.card,
           cap.bus_info);
   fmtd.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
   printf("formats:\n");
   while (0 == ioctl(fd, VIDIOC_ENUM_FMT, &fmtd)) {
      printf("  %s\n", fmtd.description);
      fmtd.index++;
      }
   //
   // set format
   //
   fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
   fmt.fmt.pix.width = IMAGE_WIDTH;
   fmt.fmt.pix.height = IMAGE_HEIGHT;
   fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
   fmt.fmt.pix.field = V4L2_FIELD_NONE;
   if (-1 == ioctl(fd, VIDIOC_S_FMT, &fmt)) {
      perror("setting pixel format");
      return 1;
      }
   char str[10];
   strncpy(str, (char *)&fmt.fmt.pix.pixelformat, 4);
   printf( "mode:\n"
           "  width: %d\n"
           "  height: %d\n"
           "  format: %s\n"
           "  field: %d\n",
           fmt.fmt.pix.width,
           fmt.fmt.pix.height,
           str,
           fmt.fmt.pix.field);
   //
   // set camera controls
   //
   /*
   moved to command line
   ctrl.id = V4L2_CID_FOCUS_AUTO;
   ctrl.value = 0;
   if (-1 == ioctl(fd,VIDIOC_S_CTRL,&ctrl)) {
      perror("setting focus auto");
      return 1;
      }
   ctrl.id = V4L2_CID_FOCUS_ABSOLUTE;
   ctrl.value = FOCUS_ABSOLUTE;
   if (-1 == ioctl(fd,VIDIOC_S_CTRL,&ctrl)) {
      perror("setting focus absolute");
      return 1;
      }
   ctrl.id = V4L2_CID_EXPOSURE_AUTO;
   ctrl.value = EXPOSURE_TYPE;
   if (-1 == ioctl(fd,VIDIOC_S_CTRL,&ctrl)) {
      perror("setting exposure auto");
      return 1;
      }
   if (EXPOSURE_TYPE == V4L2_EXPOSURE_MANUAL) {
      ctrl.id = V4L2_CID_EXPOSURE_ABSOLUTE;
      ctrl.value = EXPOSURE;
      if (-1 == ioctl(fd,VIDIOC_S_CTRL,&ctrl)) {
         perror("setting exposure absolute");
         return 1;
         }
      }
   ctrl.id = V4L2_CID_BRIGHTNESS;
   ctrl.value = BRIGHTNESS;
   if (-1 == ioctl(fd,VIDIOC_S_CTRL,&ctrl)) {
      perror("setting brightness");
      return 1;
      }
   ctrl.id = V4L2_CID_CONTRAST;
   ctrl.value = CONTRAST;
   if (-1 == ioctl(fd,VIDIOC_S_CTRL,&ctrl)) {
      perror("setting contrast");
      return 1;
      }
   */
   //
   // set zoom
   //
   /*
   moved to command line
   system(ZOOM);
   */
   //
   // set up the memory map
   //
   request_buffer.count = 1;
   request_buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
   request_buffer.memory = V4L2_MEMORY_MMAP;
   if (-1 == ioctl(fd, VIDIOC_REQBUFS, &request_buffer)) {
      perror("requesting buffer");
      return 1;
      }
   video_buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
   video_buffer.memory = V4L2_MEMORY_MMAP;
   video_buffer.index = 0;
   if(-1 == ioctl(fd, VIDIOC_QUERYBUF, &video_buffer)) {
      perror("querying buffer");
      return 1;
      }
   memory_buffer = mmap(NULL,video_buffer.length,PROT_READ|PROT_WRITE,MAP_SHARED,fd,video_buffer.m.offset);
   printf("buffer length: %d\n", video_buffer.length);
   //
   // start the stream
   //
   video_buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
   video_buffer.memory = V4L2_MEMORY_MMAP;
   video_buffer.index = 0;
   if (-1 == ioctl(fd, VIDIOC_STREAMON, &video_buffer.type)) {
      perror("start capture");
      return 1;
      }
   return 0;
   }
//
// capture an image
//
int capture_image(int fd) {
   //
   // capture an image 
   //
   fd_set fds;
   struct timeval tv = {0};
   //
   // enqueue the buffer
   //
   if (-1 == ioctl(fd, VIDIOC_QBUF, &video_buffer)) {
      perror("enqueue buffer");
      return 1;
      }
   //
   // wait for select
   //
   FD_ZERO(&fds);
   FD_SET(fd, &fds);
   tv.tv_sec = 1;
   int r = select(fd+1, &fds, NULL, NULL, &tv);
   if (-1 == r) {
      perror("select frame");
      return 1;
      }
   //
   // dequeue the buffer
   //
   if (-1 == ioctl(fd, VIDIOC_DQBUF, &video_buffer)) {
      perror("dequeue buffer");
      return 1;
      }
   return 0;
   }
//
// end capture
//
int capture_end(int fd) {
   //
   // end the capture stream
   //
   if (-1 == ioctl(fd, VIDIOC_STREAMOFF, &video_buffer.type)) {
      perror("stop capture");
      return 1;
      }
   return 0;
   }
//
// delay between frames
//
void delay(float time) {
   usleep(time*1e6);
   }
//
// main
//
int main(int argc, char **argv) {
   //
   // local variables
   //
   Display *D;
   int S;
   Window W;
   GC Gc,GcRev;
   XEvent Event;
   XWindowAttributes A;
   struct {
      long flags;
      long functions;
      long decorations;
      long inputmode;
      } prop;
   unsigned int i,j,r,row,col,min,max,p0,p1;
   int ival;
   float y,norm,fval;
   IplImage *img,*img0,*img1,*diff,*rows,*cols,*weight;
   CvCapture* cap;
   char name[100];
   int x,width,height;
   int fd;
   char c;
   float threshold;
   char video[100];
   //
   // command line args
   //
   if (argc < 2) {
      printf("command line: coord output_file background_threshold video\n");
      printf("   output_file = output coordinate PNG file\n");
      printf("   background_threshold = background subtraction threshold (default 0)\n");
      printf("   video = video device (default /dev/video1)\n");
      exit(-1);
      }
   threshold = 0;
   if (argc >=3)
      sscanf(argv[2],"%f",&threshold);
   sprintf(video,"/dev/video1");
   if (argc >= 4)
      sscanf(argv[3],"%s",video);
   //
   // set up display
   //
   /*
   moved to command line
   system(DISPLAY_BRIGHTNESS);
   */
   //
   // set up X window
   //
   D = XOpenDisplay("");
   S = DefaultScreen(D);
   W = XCreateSimpleWindow(D, DefaultRootWindow(D),
       0, 0, 100, 100, 1, BlackPixel(D,S), BlackPixel(D,S));
   XStoreName(D, W, "xcirc output");
   XMapRaised(D, W);
   //
   // NETWM fullscreen window
   //
   /*
   Atom wm_state = XInternAtom(D, "_NET_WM_STATE", False);
   Atom fullscreen = XInternAtom(D, "_NET_WM_STATE_FULLSCREEN", False);
   Event.type = ClientMessage;
   Event.xclient.window = W;
   Event.xclient.message_type = wm_state;
   Event.xclient.format = 32;
   Event.xclient.data.l[0] = 1;
   Event.xclient.data.l[1] = fullscreen;
   Event.xclient.data.l[2] = 0;
   XSendEvent(D, DefaultRootWindow(D), False,
   SubstructureNotifyMask, &Event);
   */
   //
   // non-NETWM maximize window
   //
   XGetWindowAttributes(D, DefaultRootWindow(D), &A);
   XMoveResizeWindow(D, W, 0, 0, A.width, A.height);
   printf("window %d %d\n",A.width,A.height);
   //
   // non-NETWM remove window trim
   //
   prop.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS|MWM_HINTS_INPUT_MODE;
   prop.decorations = 0;
   prop.functions = MWM_FUNC_MOVE;
   prop.inputmode = MWM_INPUT_MODELESS;
   Atom a = XInternAtom(D,"_MOTIF_WM_HINTS",0);
   XChangeProperty(D, W, a, a, 32, PropModeReplace, (unsigned char*)&prop, 4);
   //
   // set up X graphics contexts
   //
   Gc = XCreateGC (D, W, 0L, (XGCValues *) 0);
   XSetForeground(D, Gc, WhitePixel(D, S));
   GcRev = XCreateGC (D, W, 0L, (XGCValues *) 0);
   XSetForeground(D, GcRev, BlackPixel(D, S));
   XSelectInput (D, W, KeyPressMask);
   //
   // open and init video
   //
   fd = open(video,O_RDWR);
   if (fd == -1) {
      perror("opening video device");
      return -1;
      }
   capture_init(fd);
   //
   // create images
   //
   frame = cvCreateImage(cvSize(IMAGE_WIDTH,IMAGE_HEIGHT),IPL_DEPTH_8U,1);
   img = cvCreateImage(cvSize(IMAGE_WIDTH,IMAGE_HEIGHT),IPL_DEPTH_8U,1);
   img0 = cvCreateImage(cvSize(IMAGE_WIDTH,IMAGE_HEIGHT),IPL_DEPTH_8U,1);
   img1 = cvCreateImage(cvSize(IMAGE_WIDTH,IMAGE_HEIGHT),IPL_DEPTH_8U,1);
   diff = cvCreateImage(cvSize(IMAGE_WIDTH,IMAGE_HEIGHT),IPL_DEPTH_8U,1);
   rows = cvCreateImage(cvSize(IMAGE_WIDTH,IMAGE_HEIGHT),IPL_DEPTH_8U,1);
   cols = cvCreateImage(cvSize(IMAGE_WIDTH,IMAGE_HEIGHT),IPL_DEPTH_8U,1);
   weight = cvCreateImage(cvSize(IMAGE_WIDTH,IMAGE_HEIGHT),IPL_DEPTH_8U,1);
   //
   // flush X
   //
   XFlush(D);
   delay(DELAY_TIME);
   //
   // all on
   //
   XClearWindow(D, W);
   XFillRectangle(D, W, Gc, 0, 0, A.width, A.height);
   XFlush(D);
   delay(DELAY_TIME);
   capture_image(fd);
   cvmat = cvMat(IMAGE_HEIGHT,IMAGE_WIDTH,CV_8UC3,(void *) memory_buffer);
   frame = cvDecodeImage(&cvmat,1);
   sprintf(name,"all_color.png");
   cvSaveImage(name,frame,0);
   cvCvtColor(frame,img0,CV_RGB2GRAY);
   sprintf(name,"all_on.png");
   cvSaveImage(name,img0,0);
   //
   // all off
   //
   XClearWindow(D, W);
   XFillRectangle(D, W, GcRev, 0, 0, A.width, A.height);
   XFlush(D);
   delay(DELAY_TIME);
   capture_image(fd);
   cvmat = cvMat(IMAGE_HEIGHT,IMAGE_WIDTH,CV_8UC3,(void *) memory_buffer);
   frame = cvDecodeImage(&cvmat,1);
   cvCvtColor(frame,img1,CV_RGB2GRAY);
   sprintf(name,"all_off.png");
   cvSaveImage(name,img1,0);
   //
   // difference weights
   //
   for (row = 0; row < img0->height; ++row)
      for (col = 0; col < img0->width; ++col) {
         p0 = (unsigned char) ((img0->imageData) + (img0->widthStep)*row)[col];
         p1 = (unsigned char) ((img1->imageData) + (img1->widthStep)*row)[col];
         if (p0 > p1)
            ((weight->imageData) + (weight->widthStep)*row)[col] = p0 - p1;
         else
            ((weight->imageData) + (weight->widthStep)*row)[col] = 0;
         }
   sprintf(name,"all_diff.png");
   cvSaveImage(name,weight,0);
   //
   // loop over depth
   //
   for (i = 0; i < DEPTH; ++i) {
      //
      // check for break
      //
      if (XCheckWindowEvent(D, W, KeyPressMask, &Event)) {
         break;
         }
      //
      // odd Gray code rows
      //
      XClearWindow(D, W);
      for (j = 0; j < pow(2,i+1); ++j) {
         y = A.height*(j)/pow(2,i+1);
         height = A.height*1.0/pow(2,i+1);
         if (((((j+1)/2))%2) == 1)
            XFillRectangle(D,W,Gc,0,y,A.width,height);
         }
      XFlush(D);
      delay(DELAY_TIME);
      capture_image(fd);
      cvmat = cvMat(IMAGE_HEIGHT,IMAGE_WIDTH,CV_8UC3,(void *) memory_buffer);
      frame = cvDecodeImage(&cvmat,1);
      cvCvtColor(frame,img0,CV_RGB2GRAY);
      sprintf(name,"img_%03d_0.png",i);
      cvSaveImage(name,img0,0);
      //
      // even Gray code rows
      //
      XClearWindow(D, W);
      for (j = 0; j < pow(2,i+1); ++j) {
         y = A.height*(j)/pow(2,i+1);
         height = A.height*1.0/pow(2,i+1);
         if (((((j+1)/2))%2) == 0)
            XFillRectangle(D,W,Gc,0,y,A.width,height);
         }
      XFlush(D);
      delay(DELAY_TIME);
      capture_image(fd);
      cvmat = cvMat(IMAGE_HEIGHT,IMAGE_WIDTH,CV_8UC3,(void *) memory_buffer);
      frame = cvDecodeImage(&cvmat,1);
      cvCvtColor(frame,img1,CV_RGB2GRAY);
      sprintf(name,"img_%03d_1.png",i);
      cvSaveImage(name,img1,0);
      //
      // difference and accumulate
      //
      for (row = 0; row < img0->height; ++row)
         for (col = 0; col < img0->width; ++col) {
            p0 = (unsigned char) ((img0->imageData) + (img0->widthStep)*row)[col];
            p1 = (unsigned char) ((img1->imageData) + (img1->widthStep)*row)[col];
            if (p0 > p1) {
               if (i == 0)
                  ((rows->imageData) + (rows->widthStep)*row)[col] = (1 << 7);
               else
                  if (((((rows->imageData) + (rows->widthStep)*row)[col]) & (1 << (8-i))) == 0)
                     ((rows->imageData) + (rows->widthStep)*row)[col] += (1 << (7-i));
               ((diff->imageData) + (diff->widthStep)*row)[col] = (p0-p1);
               //((diff->imageData) + (diff->widthStep)*row)[col] = 255;
               }
            else {
               if (i != 0)
                  if (((((rows->imageData) + (rows->widthStep)*row)[col]) & (1 << (8-i))) != 0)
                     ((rows->imageData) + (rows->widthStep)*row)[col] += (1 << (7-i));
               ((diff->imageData) + (diff->widthStep)*row)[col] = 0;
               }
            }
      sprintf(name,"diff_%03d.png",i);
      cvSaveImage(name,diff,0);
      }
   //
   // filter and save
   //
   for (row = 0; row < weight->height; ++row)
      for (col = 0; col < weight->width; ++col) {
         p0 = (unsigned char) ((weight->imageData) + (weight->widthStep)*row)[col];
         if (p0 < threshold) {
            ((rows->imageData) + (rows->widthStep)*row)[col] = 0;
            }
         else
            ((rows->imageData) + (rows->widthStep)*row)[col] =
            255 - ((rows->imageData) + (rows->widthStep)*row)[col];
         }
   sprintf(name,argv[1],i);
   cvSaveImage(name,rows,0);
   //
   // close video
   //
   capture_end(fd);
   close(fd);
   //
   // exit
   //
   exit(0);
   }
