//
// hello.ESP32-WROOM.WebClient.ino
//
// ESP32 Web client hello-world
//
// Neil Gershenfeld 11/13/19
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <WiFi.h>

const char* ssid = "your SSID goes here";
const char* password = "your password goes here";
const char* host = "academy.cba.mit.edu";
const char* URL = "/classes/networking_communications/hello.html";
const int port = 80;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  printf("\nConnecting to WiFi ");
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    printf(".");
    }
  printf("\nWiFi connected with address %s\n",WiFi.localIP().toString().c_str());
  printf("Connecting to host %s\n",host);
  if (!client.connect(host,port)) {
    printf("Connection failed\n");
    return;
    }
  printf("Requesting URL %s\n\n",URL);
  client.print(String("GET ")
     + URL
     + " HTTP/1.1\r\n"
     + "Host: "+host+"\r\n"
     + "Connection: close\r\n\r\n");
  while (client.available() == 0)
    ;
  printf("Received response:\n");
  while(client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
    }
  }

void loop()
{
  ;
  }
