#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


// Initialize the radio object with CE and CSN pins
RF24 radio(4, 5); // CE = 4, CSN = 5 (adjust pins as needed for your ESP32)

long weight; // Variable to store the weight value

void setup() {
    // Start serial communication for debugging
    Serial.begin(115200);
    
    // Start the radio communication
    if (!radio.begin()) {
        Serial.println("Radio module initialization failed. Check wiring.");
        while (1); // Halt the program if initialization fails
    } else {
        Serial.println("Radio module initialized successfully.");
    }
    
    // Set the communication channel (0 to 127)
    radio.setChannel(5);
    
    // Set the data rate (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS)
    radio.setDataRate(RF24_1MBPS);
    
    // Set the power amplifier level (RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX)
    radio.setPALevel(RF24_PA_HIGH);
    
    // Set the address for the pipe to write to
    radio.openWritingPipe(0x1234567890LL); // 5-byte address
    
    Serial.println("Transmitter ready. Sending weight data...");
}

void loop() {
    // Set the weight value
    weight = 13381;
    radio.write(&weight, sizeof(weight));
    // Send the weight value over the radio
    if (radio.write(&weight, sizeof(weight))) {
        Serial.println("Weight sent successfully.");
    } else {
        Serial.println("Failed to send weight.");
    }
    
    // Wait for 1 second before sending the next value
    delay(1000);
}