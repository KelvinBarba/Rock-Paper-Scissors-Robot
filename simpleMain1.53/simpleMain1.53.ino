// Base (Slave or Reciever)

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal.h>
#include "pitches.h"

    // Pins //

#define CE_PIN   9
#define CSN_PIN 10

    // Variables //

const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

int sensor_data[3];
int pointer_position;
int pinky_position;
int button_pos;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 

bool newData = false;
bool GameOn = false;

RF24 radio(CE_PIN, CSN_PIN);

    // Setup //

void setup() 
{
    Serial.begin(9600);
	lcd.begin(16,2);
	pinMode(A1, OUTPUT);
    Serial.println("SimpleRx Starting");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.startListening();
}

    // Loop //

void loop() 
{
    getData();

    if (button_pos < 5 && GameOn == false) 
	{
		GameOn = true;
		start_button();
    }
}

    // getData //

void getData() {
    if ( radio.available() ) 
	{
        radio.read(&sensor_data, sizeof(sensor_data));
        
        pointer_position = sensor_data[0]; // Need to figure out if 0 or 1 is closed
        pinky_position = sensor_data[1];
		button_pos = sensor_data[2];
        
        newData = true;
    }
}

    // find_player_response //

int find_player_response(int pointer_position, int pinky_position) 
{ 
  if (pointer_position <= 675 && pinky_position <= 675) // Paper
    return 1;
  else if (pointer_position > 675 && pinky_position <= 675 || pointer_position <= 675 && pinky_position > 675  ) // Scissors
    return 2;
  else if (pointer_position > 675 && pinky_position > 675) // Rock
    return 0;
  else
    return 3;
}

// Start Game button function

void start_button () 
{
  int player_signal = 3;
  lcd.clear();
  lcd.println("Starting in: ");
  for (int i = 3; i > 0; i--) {
    delay(1000);
	lcd.clear();
    lcd.println(i);
  }
  getData();
  lcd.clear();
  lcd.println("Go");
  delay(1000);
  player_signal = find_player_response(pointer_position, pinky_position);
  RPS_func(player_signal);
}

// RPS_function 

void RPS_func(int player_outcome) 
{
	int comp_outcome = random(3);
	lcd.clear();
	switch (player_outcome) 
	{
	case 0: // Player chose Rock
      if (comp_outcome == 0)
	  {
        lcd.println("P:Rock, C:Rock");
		ResultBuzzer(2);
	  }
      else if (comp_outcome == 1)
	  {
        lcd.println("P:Rock, C:Paper");
		ResultBuzzer(1);
	  }
      else if (comp_outcome == 2)
	  {
        lcd.println("P:Rock, C:Scsor");
		ResultBuzzer(0);
	  }
      else
        lcd.println("C Outcome invalid");
      break;
    case 1: // Player chose Paper
      if (comp_outcome == 0)
	  {
        lcd.println("P:Paper, C:Rock");
		ResultBuzzer(0);
	  }
      else if (comp_outcome == 1)
	  {
        lcd.println("P:Paper, C:Paper");
		ResultBuzzer(2);
	  }
      else if (comp_outcome == 2)
	  {
        lcd.println("P:Paper, C:Scsor");
		ResultBuzzer(1);
	  }
      else
        lcd.println("C Outcome invalid");
      break;
    case 2: // Player chose Scissors
      if (comp_outcome == 0)
	  {
        lcd.println("P:Scsor, C:Rock");
		ResultBuzzer(1);
	  }
      else if (comp_outcome == 1)
	  {
        lcd.println("P:Scsor, C:Paper");
		ResultBuzzer(0);
	  }
      else if (comp_outcome == 2)
	  {
        lcd.println("P:Scsor, C:Scsor");
		ResultBuzzer(2);
	  }
      else
        lcd.println("C Outcome invalid");
      break;
    case 3:
      lcd.println("Invalid P Outcome");
      break;
  }
  GameOn = false;
  comp_outcome = 3;
}
void ResultBuzzer(int game_result)
{
	if (game_result == 0) //Player Wins
	{
		lcd.setCursor(0,1);
		lcd.println("Player Wins!");   
		WinSound();
	}
	else if (game_result == 1)//Computer Wins
	{
		lcd.setCursor(0,1);
		lcd.println("Computer Wins!");
		LoseSound();
	}
	else if (game_result == 2)//Game is tied
	{
		lcd.setCursor(0,1);
		lcd.println("Game is a Tie!");
		TieSound();
	}
}
void WinSound() 
{
	tone(A1,NOTE_E6,125);
	delay(130);
	tone(A1,NOTE_G6,125);
	delay(130);
	tone(A1,NOTE_E7,125);
	delay(130);
	tone(A1,NOTE_C7,125);
	delay(130);
	tone(A1,NOTE_D7,125);
	delay(130);
	tone(A1,NOTE_G7,125);
	delay(125);
	noTone(A1);
}
void LoseSound() 
{
    tone(A1, 391, 47);
    delay(47);
    delay(47);
    delay(23);
    tone(A1, 415, 47);
    delay(47);
    tone(A1, 880, 47);
    delay(47);
    delay(23);
    delay(23);
    tone(A1, 1046, 23);
    delay(23);
    tone(A1, 554, 23);
    delay(23);
    delay(23);
    tone(A1, 587, 23);
    delay(23);
    tone(A1, 622, 23);
    delay(23);
    tone(A1, 659, 47);
    delay(47);
	noTone(A1); 
}

void TieSound() 
{
	tone(A1,NOTE_G4,35);
	delay(35);
	tone(A1,NOTE_G5,35);
	delay(35);
	tone(A1,NOTE_G6,35);
	delay(35);
	
	tone(A1,NOTE_G4,35);
	delay(35);
	tone(A1,NOTE_G5,35);
	delay(35);
	tone(A1,NOTE_G6,35);
	delay(35);
	noTone(A1);
}
    // showData//

// void showData() {
    // if (newData == true) {
        // Serial.print("Data received ");
        // Serial.print("Pointer: ");
        // Serial.print(pointer_position);
        // Serial.print(" Pinky: ");
        // Serial.println(pinky_position);
		// Serial.print(" Button: ");
        // Serial.println(button_pos);
        // newData = false;
    // }
// }

