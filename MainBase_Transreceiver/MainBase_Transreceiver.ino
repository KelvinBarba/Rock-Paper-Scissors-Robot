#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>
#include <LiquidCrystal.h> // includes the LiquidCrystal Library 

// Library For nRF24: https://github.com/nRF24/RF24/

// Pin Names

// Variables

const byte addresses [] [6] = {"00000", "00001"};

boolean startup_base = 1;
boolean test_send = 1;
boolean test_receive = 0;

int sensor_data = {0,0};
int Index_position = 3; // Initial Value
int Ring_position = 3; // Initial Value

RF24 radio(7,8); // CNS, CE Pins from transreceiver

LiquidCrystal lcd(1, 2, 4, 5, 6, 7); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 

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
    get_SensorData():
  }

}

          // get_SensorData Function //

void get_SensorData() {

  if (radio.available()) {
    radio.read(&sensor_data, sizeof(sensor_data));

    Index_position = sensor_data[0];
    Ring_position = sensor_data[1];
    
    Serial.print("Data Received: " + sensor_data[0]);
    Serial.println(", " + sensor_data[1]);
  }
}
