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

int meanX = 0, meanY = 0, meanArm = 0, meanGripper = 0;
bool meanCalculated = false;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipeOut);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();

  calculateMean();  // Start calibration
}

void calculateMean() {
  Serial.println("Calculating joystick center positions...");
  
  long sumX = 0, sumY = 0, sumArm = 0, sumGripper = 0;
  int count = 0;
  unsigned long startTime = millis();

  while (millis() - startTime < 5000) {  // Run for exactly 5 seconds
    sumX += analogRead(A0);
    sumY += analogRead(A1);
    sumArm += analogRead(A2);
    sumGripper += analogRead(A3);
    count++;
    delay(10);  // Small delay to get reasonable samples
  }

  // Calculate the mean values separately
  meanX = sumX / count;
  meanY = sumY / count;
  meanArm = sumArm / count;
  meanGripper = sumGripper / count;
  meanCalculated = true;

  Serial.println("Mean values calculated! You can move the joystick now.");
  Serial.print("Mean X: "); Serial.print(meanX);
  Serial.print(" | Mean Y: "); Serial.print(meanY);
  Serial.print(" | Mean Arm: "); Serial.print(meanArm);
  Serial.print(" | Mean Gripper: "); Serial.println(meanGripper);
}

// Function to map joystick values separately
int mapJoystickValues(int val, int mean) {
  if (val > mean) {
    return map(val, mean, 1023, 0, 255);  // Map positive direction
  } else {
    return map(val, 0, mean, -255, 0);  // Map negative direction
  }
}

void loop() {
  if (!meanCalculated) return;  // Wait until mean is calculated

  data.rover_x = mapJoystickValues(analogRead(A0), meanX);
  data.rover_y = mapJoystickValues(analogRead(A1), meanY);
  data.arm = mapJoystickValues(analogRead(A2), meanArm);
  data.gripper = mapJoystickValues(analogRead(A3), meanGripper);

  // Print values
  Serial.print("Rover X: "); Serial.print(data.rover_x);
  Serial.print(" | Rover Y: "); Serial.print(data.rover_y);
  Serial.print(" | Arm: "); Serial.print(data.arm);
  Serial.print(" | Gripper: "); Serial.println(data.gripper);

  // Send data over RF
  radio.write(&data, sizeof(Signal));

  delay(100);  // Small delay to prevent flooding the receiver
}
