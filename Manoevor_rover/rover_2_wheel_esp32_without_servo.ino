d#include <WiFi.h>
#include <esp_now.h>

// Define the motor control pins
const int int1 = 12;
const int int2 = 14;
const int int3 = 26;
const int int4 = 27;
const int ena = 25;  // Added ENA pin
const int enb = 33;  // Added ENB pin

// Array of motor control pins
int pin[] = {int1, int2, int3, int4, ena, enb};

// Structure to hold received data
struct Signal {
  int rover_x;
  int rover_y;
  int arm;     // Not used in this version
  int gripper; // Not used in this version
};

Signal receivedData;
unsigned long lastRecvTime = 0;  // Track last received time

// Function to reset data to default values
void ResetData() {
  receivedData.rover_x = 127;
  receivedData.rover_y = 127;
  receivedData.arm = 127;
  receivedData.gripper = 127;
}

// Callback function when data is received
void onReceive(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  lastRecvTime = millis();  // Update last received time
}

// Function to control the rover movement
void movement(const Signal &s1) {
  if (abs(s1.rover_x) <= abs(s1.rover_y)) {
    // Move forward or backward
    bool int1_st = s1.rover_y >= 0 ? HIGH : LOW;
    bool int2_st = s1.rover_y >= 0 ? LOW : HIGH;
    digitalWrite(int1, int1_st);
    digitalWrite(int2, int2_st);
    digitalWrite(int3, int1_st);
    digitalWrite(int4, int2_st);
    analogWrite(ena, abs(s1.rover_y));
    analogWrite(enb, abs(s1.rover_y));
  } else {
    // Turn left or right
    bool int1_st = s1.rover_x >= 0 ? LOW : HIGH;
    bool int2_st = s1.rover_x >= 0 ? HIGH : LOW;
    bool int3_st = s1.rover_x <= 0 ? LOW : HIGH;
    bool int4_st = s1.rover_x <= 0 ? HIGH : LOW;
    digitalWrite(int1, int1_st);
    digitalWrite(int2, int2_st);
    digitalWrite(int3, int3_st);
    digitalWrite(int4, int4_st);
    analogWrite(ena, abs(s1.rover_x));
    analogWrite(enb, abs(s1.rover_x));
  }
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Set motor control pins as outputs
  for (int i = 0; i < (sizeof(pin) / sizeof(pin[0])); i++) {
    pinMode(pin[i], OUTPUT);
  }

  // Initialize WiFi in STA mode
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed!");
    return;
  }

  // Register callback function for receiving data
  esp_now_register_recv_cb(onReceive);

  // Reset data to default values
  ResetData();
}

void loop() {
  unsigned long now = millis();

  // If no data received for 1 second, reset values
  if (now - lastRecvTime > 1000) {
    ResetData();
  }

  // Control the rover movement based on received data
  movement(receivedData);

  // Print received values for debugging
  Serial.print("Rover X: "); Serial.print(receivedData.rover_x);
  Serial.print(" | Rover Y: "); Serial.print(receivedData.rover_y);
  Serial.print(" | Arm: "); Serial.print(receivedData.arm);
  Serial.print(" | Gripper: "); Serial.println(receivedData.gripper);

  delay(10);  // Small delay to avoid spamming serial output
}