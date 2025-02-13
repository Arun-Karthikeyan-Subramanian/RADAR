#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeOut = 0xE9E8F0F0E1LL; 


RF24 radio(7, 8); 


struct Signal {
  byte rover_front;
  byte rover_back;
  byte rover_right;
  byte rover_left;
  byte arm_up;
  byte arm_down;
  byte grip;
  byte ungrip;
};

Signal data;

void ResetData(){
  data.rover_front = 127; 
  data.rover_back = 127;  
  data.rover_right = 127;    
  data.rover_left = 127;     
  data.arm_up = 127;
  data.arm_down = 127;  
  data.grip = 127;
  data.ungrip = 127;
}

void setup(){
  radio.begin();
  radio.openWritingPipe(pipeOut);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
  ResetData();
 
}

int mapJoystickValues(int val, int lower, int middle, int upper, bool reverse){
  val = constrain(val, lower, upper);
  if ( val < middle )
  val = map(val, lower, middle, 0, 128);
  else
  val = map(val, middle, upper, 128, 255);
  return ( reverse ? 255 - val : val );
}
  void loop()
{
  Serial.println(sizeof(Signal));
  data.rover_front = mapJoystickValues( analogRead(A0), 12, 524, 1020, true );
  data.rover_back = mapJoystickValues( analogRead(A1), 12, 524, 1020, true );
  data.rover_right = mapJoystickValues( analogRead(A2), 12, 524, 1020, false );
  data.rover_left = mapJoystickValues( analogRead(A3), 12, 524, 1020, false );
  radio.write(&data, sizeof(Signal));
}
