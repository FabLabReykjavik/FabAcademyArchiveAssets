//
// hello.ESP32-WROOM.UDP.ino
//
// ESP32 UDP hello-world
//
// Neil Gershenfeld 11/16/19
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include "WiFi.h"
#include "AsyncUDP.h"

const char* ssid = "???"; // replace with your WiFi SSID
const char* password = "???"; // replace with your WiFi password
const uint8_t address[] = {???,???,???,???}; // replace with UDP IP address
const uint16_t port = ???; // replace with UDP port number

AsyncUDP udp;

void setup() {
  //
  // start serial
  //
  Serial.begin(115200);
  //
  // connect to WiFi
  //
  Serial.print("\nConnecting ");
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
    }
  Serial.print("\nConnected with local address ");
  Serial.print(WiFi.localIP());
  Serial.print("\n");
  //
  // start UDP
  //
  udp.listen(1234);
  //
  // UDP packet handler
  //
  udp.onPacket([](AsyncUDPPacket packet) {
    //
    // write to serial
    //
    Serial.print("Received from ");
    Serial.print(packet.remoteIP());
    Serial.print(": ");
    Serial.write(packet.data(),packet.length());
    Serial.print("\n");
    });
  }

void loop() {
  //
  // check for serial
  //
  if (Serial.available()) {
    //
    // read from serial
    //
    String s = Serial.readStringUntil('\n');
    Serial.print("Sending to ");
    Serial.print(IPAddress(address));
    Serial.print(": ");
    Serial.print(s);
    Serial.print("\n");
    //
    // send to UDP
    //
    udp.connect(IPAddress(address),port);
    udp.print(s);
    udp.close();
    }
  }
