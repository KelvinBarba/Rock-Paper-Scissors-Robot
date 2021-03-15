// Glove (Master or Transmitter)

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

    // Pins //

#define CE_PIN   9
#define CSN_PIN 10
#define index_flex_pin A0 
#define ring_flex_pin A1 
#define start_button_pin A5 

    // Variables //

const byte slaveAddress[5] = {'R','x','A','A','A'};
int sensor_data[3] = {0,0,0};

//char dataToSend[10] = "Message 0";
//char txNum = '0';

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000; // send once per second

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

    // Setup //

void setup() 
{
    Serial.begin(9600);
	pinMode(index_flex_pin, INPUT);
	pinMode(ring_flex_pin, INPUT);
	pinMode(start_button_pin, INPUT);
    Serial.println("GloveTx Starting");

    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.setRetries(3,5); // delay, count
    radio.openWritingPipe(slaveAddress);
}

    // Loop //

void loop() {
    currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis) {
        send();
        prevMillis = millis();
    }
}

    // send //

void send() 
{
    bool rslt;
    int Pinky_flex_Value = 0;
	int Ring_flex_value = 0;;
	int button_data = 0;
	
    Pinky_flex_Value = analogRead(index_flex_pin);
    Ring_flex_value = analogRead(ring_flex_pin);
	
	button_data = analogRead(start_button_pin);
	
	sensor_data[0] = Pinky_flex_Value;
    sensor_data[1] = Ring_flex_value;
	sensor_data[2] = button_data;
    
    rslt = radio.write( &sensor_data, sizeof(sensor_data));

    Serial.print("Data Sent ");
    Serial.print(sensor_data[0]);
    if (rslt) {
        Serial.println("  Acknowledge received");
    }
    else {
        Serial.println("  Tx failed");
    }
}

//================

//void updateMessage() {
        // so you can see that new data is being sent
//    txNum += 1;
//    if (txNum > '9') {
//        txNum = '0';
//    }
//    dataToSend[8] = txNum;
//}
