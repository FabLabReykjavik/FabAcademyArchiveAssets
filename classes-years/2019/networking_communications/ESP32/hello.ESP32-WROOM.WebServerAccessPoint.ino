//
// hello.ESP32-WROOM.WebServerAccessPoint.ino
//
// ESP32 Web server + access point hello-world
//
// Neil Gershenfeld 11/16/19
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <WiFi.h>

const char* ssid = "ESP32-WROOM hello world";
const char* password = "12345678";
IPAddress local(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid,password);
  WiFi.softAPConfig(local,gateway,subnet);
  printf("Access point started at %s\n",local.toString().c_str());
  server.begin();
  }

void loop() {
  char cold,cnew;
  WiFiClient client = server.available();
  if (client) {
    printf("\nReceived connection from %s\n\n",client.remoteIP().toString().c_str());
    while (client.connected()) {
      if (client.available()) {
        cnew = client.read();
        printf("%c",cnew);
        if ((cold == '\n') && (cnew == '\r')) { // check for blank line at end of request
          client.printf("HTTP/1.1 200 OK\n");
          client.printf("Content-type:text/html\n");
          client.printf("\n");
          client.printf("Hello %s from ESP32-WROOM!<br>\n",client.remoteIP().toString().c_str());
          client.stop();
          break;
          }
        cold = cnew;
        }
      }
    }
  }
