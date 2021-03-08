#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>

// Library For nRF24: https://github.com/nRF24/RF24/

// Pin Names

#define index_flex_pin 4 // Replace for actual pin number
#define ring_flex_pin 5 // Replace for actual pin number

// Variables

const byte addresses [][6] = {"00000", "00001"};

boolean startup_glove = 1;
boolean test_receive = 0;

int sensor_data [2];
int IndexflexValue;
int Index_position;
int RingflexValue;
int Ring_position;

RF24 radio(9,10); // Connected to CE, CSN Pins from transreceiver

void setup() {
  Serial.begin(9600);
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

    IndexflexValue = analogRead(index_flex_pin);
    Index_position = map(IndexflexValue, 0, 1023, 0, 1);

    RingflexValue = analogRead(ring_flex_pin);
    Ring_position =  map(RingflexValue, 0 ,1023, 0 ,1);

    sensor_data[0] = Index_position;
    sensor_data[1] = Ring_position;

    send_SensorData();
  }
}

          // send_SensorData Function //

void send_SensorData () {

  bool transmitted;
  transmitted = radio.write(&sensor_data, sizeof(sensor_data));

  if (transmitted) {
    Serial.println("Transmission Successful");
    Serial.print("Data sent: " + sensor_data[0]);
    Serial.println(", " + sensor_data[1]);
  }
  else {
    Serial.println("Transmission Failed");
  }
  
}
