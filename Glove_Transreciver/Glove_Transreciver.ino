#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>

// Library For nRF24: https://github.com/nRF24/RF24/

// Pin Names

#define flex_pin 4 // Replace for actual pin number

// Variables

const byte address [][6] = {"00000". "00001"};
boolean startup_glove = 1;
boolean test_receive = 0;

RF24 radio(7,8); // CE, CSN Pins from transreceiver

void setup() {
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00000
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  delay(5);
  
// Confirm Connection between Base and Glove

  if (startup_glove) {
    radio.startListening();

    if (radio.available()) {
      while(radio.available()) {
        radio.read(&test_receive, sizeof(test_receive));
      }
      delay(5);
      if (test_receive){
        radio.stopListening();
        radio.write(&test_receive, sizeof(test_receive));
        startup_glove = 0;
      }
      else {
        radio.write(&test_receive, sizeof(test_receive));
      }
    }
  }

// Glove can start sending data

  if (startup_glove != 1) {
    radio.stopListening();

    int flexValue = analogRead(flex_pin);
  }


}
