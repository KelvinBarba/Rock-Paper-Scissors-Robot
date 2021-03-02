#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>

// Library For nRF24: https://github.com/nRF24/RF24/

// Pin Names

// Variables

boolean startup_base = 1;
boolean test_send = 1;
boolean test_receive = 0;
RF24 radio(7,8); // CNS, CE Pins
const byte addresses [] [6] = {"00000", "00001"};

void setup() {
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00000
  radio.openReadingPipe(1, addresses[0]); //00001
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  delay(5);

// Confirm Connection between Base and Glove

  if (startup_base) {
    radio.stopListening();
    radio.write(&test_send, sizeof(test_send));

    delay(5);

    radio.startListening();
    
    while(radio.available()) {
      radio.read(&test_receive, sizeof(test_receive));

      if (test_receive) {
        Serial.print("Connection Established to Glove");
        startup_base = 0;
      }
      else {
        Serial.print("Could not establish a connection");
      }
    }
    
  }

// Base will receive input from glove

  if (startup_base != 1) {
    radio.startListening();
    
  }

}
