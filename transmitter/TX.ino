#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeOut = 0xE9E8F0F0E1LL; 


RF24 radio(7, 8); 


struct Signal {
  int rover_x;
  int rover_y;
  int arm;
  int gripper;
};

Signal data;

void ResetData(){
  data.rover_x = 512; 
  data.rover_y= 512;  
  data.arm = 512;
  data.gripper = 512;
}

void setup(){
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipeOut);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
  ResetData();
}

void loop()
{
  Serial.println(sizeof(Signal));
  data.rover_x = constrain( analogRead(A0), 0, 1023);
  data.rover_y =  constrain( analogRead(A1), 0, 1023);
  data.arm =  constrain( analogRead(A2), 0, 1023 );
  data.gripper = constrain( analogRead(A3), 0, 1023 );

  Serial.print("Rover X: "); Serial.print(data.rover_x);
  Serial.print(" | Rover Y: "); Serial.print(data.rover_y);
  Serial.print(" | Arm: "); Serial.print(data.arm);
  Serial.print(" | Gripper: "); Serial.println(data.gripper);

  radio.write(&data, sizeof(Signal));

  delay(1000);
}
