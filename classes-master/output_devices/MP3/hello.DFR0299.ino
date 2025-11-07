//
// hello.DFR0299.ino
//    DFR0299 MP3 player hello-world
//
// Neil Gershenfeld 10/28/25
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

DFRobotDFPlayerMini player;

void setup() {
  Serial1.begin(9600);
  Serial.begin(115200);  
  if (!player.begin(Serial1)) {
    Serial.println(("unable to begin:"));
    while(true);
    }
  Serial.println(("player ready"));
  player.volume(20);
  player.EQ(DFPLAYER_EQ_NORMAL);
  player.outputDevice(DFPLAYER_DEVICE_SD);
  Serial.println("track to play?");
  }

void loop() {
  int track;
  while (Serial.available() == 0);
  track = Serial.read()-'0';
  if (track > 0) {
    Serial.print("starting track ");
    Serial.println(track);
    player.play(track);
    Serial.println("next track to play?");
    }
  }
