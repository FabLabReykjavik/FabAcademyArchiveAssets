//
// hello.ESP32-Sense.ino
//
// ESP32-Sense image processing hello-world
//
// Neil Gershenfeld 10/28/23
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <WiFi.h>
#include <math.h>
#include "esp_camera.h"
#include "esp_http_server.h"

#define INTENSITY_THRESHOLD 64
#define MOTION_THRESHOLD 100
#define BOUNDARY "1234567890abcdefghij"
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     10
#define SIOD_GPIO_NUM     40
#define SIOC_GPIO_NUM     39
#define Y9_GPIO_NUM       48
#define Y8_GPIO_NUM       11
#define Y7_GPIO_NUM       12
#define Y6_GPIO_NUM       14
#define Y5_GPIO_NUM       16
#define Y4_GPIO_NUM       18
#define Y3_GPIO_NUM       17
#define Y2_GPIO_NUM       15
#define VSYNC_GPIO_NUM    38
#define HREF_GPIO_NUM     47
#define PCLK_GPIO_NUM     13

const char* ssid = "your WiFi SSID";
const char* password = "your WiFi password";
static const char* STREAM_TYPE = "multipart/x-mixed-replace;boundary=" BOUNDARY;
static const char* STREAM_BOUNDARY = "\r\n--" BOUNDARY "\r\n";
static const char* STREAM_IMAGE = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";
httpd_handle_t stream_httpd = NULL;

static esp_err_t stream_handler(httpd_req_t *req) {
   camera_fb_t *framebuffer = NULL;
   esp_err_t result = ESP_OK;
   size_t header_length,jpg_length;
   uint8_t *jpg_buffer;
   uint8_t *rgb_buffer = NULL;
   uint8_t *motion_buffer = NULL;
   char part_buffer[64];
   uint32_t width,height,len,row,col,intensity,motion;
   httpd_resp_set_type(req,STREAM_TYPE);  
   while (true) {
      //
      // get frame
      //
      framebuffer = esp_camera_fb_get();
      width = framebuffer->width;
      height = framebuffer->height;
      len = width*height*3;
      //
      // convert to RGB
      //
      if (!(rgb_buffer))
         rgb_buffer = (uint8_t *)ps_malloc(len);
      fmt2rgb888(framebuffer->buf,framebuffer->len,PIXFORMAT_JPEG,rgb_buffer);
      esp_camera_fb_return(framebuffer);
      //
      // invert
      //
      for (col = width/2; col < width; ++col) {
         for (row = height/2; row < height; ++row) {
            rgb_buffer[3*width*row+3*col+0] = 255-rgb_buffer[3*width*row+3*col+0];
            rgb_buffer[3*width*row+3*col+1] = 255-rgb_buffer[3*width*row+3*col+1];
            rgb_buffer[3*width*row+3*col+2] = 255-rgb_buffer[3*width*row+3*col+2];
            }
         }
      //
      // threshold
      //
      for (col = width/2; col < width; ++col) {
         for (row = 0; row < height/2; ++row) {
            intensity = rgb_buffer[3*width*row+3*col+0]
               + rgb_buffer[3*width*row+3*col+1]
               + rgb_buffer[3*width*row+3*col+2];
            if (intensity > 3*INTENSITY_THRESHOLD) {
               rgb_buffer[3*width*row+3*col+0] = 255;
               rgb_buffer[3*width*row+3*col+1] = 255;
               rgb_buffer[3*width*row+3*col+2] = 255;
               }
            else {
               rgb_buffer[3*width*row+3*col+0] = 0;
               rgb_buffer[3*width*row+3*col+1] = 0;
               rgb_buffer[3*width*row+3*col+2] = 0;
               }
            }
         }
      //
      // motion detect
      //      
      if (!(motion_buffer))
         motion_buffer = (uint8_t *)ps_malloc((width/2)*(height/2)*3);
      for (col = 0; col < width/2; ++col) {
         for (row = 0; row < height/2; ++row) {
            motion = abs(rgb_buffer[3*width*row+3*col+0]
               - motion_buffer[3*(width/2)*row+3*col+0])
               + abs(rgb_buffer[3*width*row+3*col+1]
               - motion_buffer[3*(width/2)*row+3*col+1])
               + abs(rgb_buffer[3*width*row+3*col+2]
               - motion_buffer[3*(width/2)*row+3*col+2]);
            motion_buffer[3*(width/2)*row+3*col+0]
               = rgb_buffer[3*width*row+3*col+0];
            motion_buffer[3*(width/2)*row+3*col+1]
               = rgb_buffer[3*width*row+3*col+1];
            motion_buffer[3*(width/2)*row+3*col+2]
               = rgb_buffer[3*width*row+3*col+2];
            if (motion < MOTION_THRESHOLD) {
               rgb_buffer[3*width*row+3*col+0] = 0;
               rgb_buffer[3*width*row+3*col+1] = 0;
               rgb_buffer[3*width*row+3*col+2] = 0;
               }
            }
         }
      //
      // convert to JPG
      //
      fmt2jpg(rgb_buffer,len,width,height,PIXFORMAT_RGB888,80,&jpg_buffer,&jpg_length);
      //
      // send to stream
      //
      header_length = snprintf((char *)part_buffer, 64, STREAM_IMAGE,jpg_length);
      httpd_resp_send_chunk(req, (const char *)part_buffer,header_length);
      httpd_resp_send_chunk(req,(const char *)jpg_buffer,jpg_length);
      httpd_resp_send_chunk(req,STREAM_BOUNDARY,strlen(STREAM_BOUNDARY));
      free(jpg_buffer);
      }
   return result;
   }

void start_server() {
   httpd_config_t config = HTTPD_DEFAULT_CONFIG();
   config.server_port = 80;
   httpd_uri_t uri_t = {
      .uri       = "/",
      .method    = HTTP_GET,
      .handler   = stream_handler,
      .user_ctx  = NULL
      };
   httpd_start(&stream_httpd,&config);
   httpd_register_uri_handler(stream_httpd, &uri_t);
   }

void setup() {
   Serial.begin(115200);
   camera_config_t config;
   config.ledc_channel = LEDC_CHANNEL_0;
   config.ledc_timer = LEDC_TIMER_0;
   config.pin_d0 = Y2_GPIO_NUM;
   config.pin_d1 = Y3_GPIO_NUM;
   config.pin_d2 = Y4_GPIO_NUM;
   config.pin_d3 = Y5_GPIO_NUM;
   config.pin_d4 = Y6_GPIO_NUM;
   config.pin_d5 = Y7_GPIO_NUM;
   config.pin_d6 = Y8_GPIO_NUM;
   config.pin_d7 = Y9_GPIO_NUM;
   config.pin_xclk = XCLK_GPIO_NUM;
   config.pin_pclk = PCLK_GPIO_NUM;
   config.pin_vsync = VSYNC_GPIO_NUM;
   config.pin_href = HREF_GPIO_NUM;
   config.pin_sscb_sda = SIOD_GPIO_NUM;
   config.pin_sscb_scl = SIOC_GPIO_NUM;
   config.pin_pwdn = PWDN_GPIO_NUM;
   config.pin_reset = RESET_GPIO_NUM;
   config.xclk_freq_hz = 20000000;
   config.fb_location = CAMERA_FB_IN_PSRAM;
   config.pixel_format = PIXFORMAT_JPEG;
   config.frame_size = FRAMESIZE_QVGA; // 320x240
   //config.frame_size = FRAMESIZE_CIF; // 400x296
   //config.frame_size = FRAMESIZE_VGA; // 640x480
   //config.frame_size = FRAMESIZE_SVGA; // 800x600
   config.jpeg_quality = 10;
   config.fb_count = 2;
   esp_camera_init(&config);   
   WiFi.begin(ssid,password);
   Serial.print("Connecting WiFi ");
   while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      }
   Serial.println("");
   Serial.print("Stream is at http://");
   Serial.println(WiFi.localIP());
   start_server();
   }

void loop() {
   ;
   }
