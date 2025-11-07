float v1,v2,v3,v4;
float v1min,v2min,v3min,v4min;


void setup() {
  Serial.begin(115200);
  touchSetCycles(0x5000,0x5000);
  v1min = v3min = v4min = 1e6;
  }

#define max_buffer 25

void loop() {
  v1 = touchRead(1);
  if (v1 < v1min) v1min = v1;
  v3 = touchRead(3);
  if (v3 < v3min) v3min = v3;
  v4 = touchRead(4);
  if (v4 < v4min) v4min = v4;
  Serial.print(v1-v1min);
  Serial.print(',');
  Serial.print(v3-v3min);
  Serial.print(',');
  Serial.println(v4-v4min);
  delay(10);
  }
