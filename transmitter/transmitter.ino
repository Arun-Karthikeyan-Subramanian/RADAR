#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(4, 5);  // CE, CSN


const byte address[6] = "00001";

void setup()
{
  radio.begin();
  

  radio.openWritingPipe(address);
  

  radio.stopListening();
}
void loop()
{
  
  const char text[] = "HOW YOU DOIN"; //message
  radio.write(&text, sizeof(text));
  
  delay(1000);
}
