#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int xPin = A0;  // Joystick X-axis
const int yPin = A1;  // Joystick Y-axis
const int swPin = 2;  // Joystick button (switch)

struct JoyValues {
  int xValue;
  int yValue;
  bool swValue;
} joy1;

RF24 radio(9, 10);  // CE = 9, CSN = 10

void setup() {
  Serial.begin(9600);
  pinMode(swPin, INPUT_PULLUP);  // Set switch pin as input with pull-up resistor

  if (!radio.begin()) {
    Serial.println("Radio module initialization failed. Check wiring.");
    while (1);
  } else {
    Serial.println("Radio module initialized successfully.");
  }

  radio.setChannel(5);  // Set the same channel as the receiver
  radio.setDataRate(RF24_1MBPS);  // Set data rate to 1 Mbps
  radio.setPALevel(RF24_PA_HIGH);  // Set power amplifier level to high
  radio.openWritingPipe(0x1234567890LL);  // Set the same address as the receiver
  radio.stopListening();  // Put the radio in transmit mode
  Serial.println("Transmitter ready. Sending joystick data...");
}

void loop() {
  // Read joystick values
  joy1.xValue = getValueAnalog(xPin);
  joy1.yValue = getValueAnalog(yPin);
  joy1.swValue = getValueDigital(swPin);

  // Print values to Serial Monitor
  Serial.print("XValue | ");
  Serial.print(joy1.xValue);
  Serial.print(" YValue | ");
  Serial.print(joy1.yValue);
  Serial.print(" Switch Value | ");
  Serial.println(joy1.swValue);

  // Send data to the receiver
  if (radio.write(&joy1, sizeof(joy1))) {
    Serial.println("Data sent successfully.");
  } else {
    Serial.println("Failed to send data.");
  }

  delay(1000);  // Delay for stability
}

int getValueAnalog(int pin) {
  return analogRead(pin);  // Read analog value
}

int getValueDigital(int pin) {
  return digitalRead(pin);  // Read digital value
}