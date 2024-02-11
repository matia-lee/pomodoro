#include "LiquidCrystal.h"
#include "Encoder.h"

int seconds = 0;
int minutes = 0;
int count = 0;

int study_minutes = 0;
int short_break_minutes = 0;
int long_break_minutes = 0;
int repeats = 0;



int buttonState = 0; 
int lastButtonState = LOW; 
int messageNumber = 0;
long lastDebounceTime = 0;  
long debounceDelay = 50;  



int break_duration;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int ledGreen = 6;
const int ledRed = 7;
const int startButton = 8;
const int nextButton = A0;
const int pinCLK = 13;
const int pinDT = 10;
const int pinSW = 9;

Encoder myEnc(pinCLK, pinDT);

long oldPosition = 0;

void setup() 
{
  lcd.begin(16, 2);
  pinMode(ledGreen,OUTPUT);
  pinMode(ledRed,OUTPUT);
  pinMode(startButton,INPUT);
  pinMode(pinSW, INPUT_PULLUP);

  lcd.print("Welcome! ");
  lcd.setCursor(0, 1);
  lcd.print("Click to start->");
}

void loop() {

  int reading = digitalRead(nextButton);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        messageNumber++;
        if (messageNumber > 4) {
          lcd.clear();
          lcd.print("<- Click left");
          lcd.setCursor(0, 1);
          lcd.print("to begin!");
        } else {
          lcd.clear();
          switch (messageNumber) {
            case 1:
              lcd.print("Study mins: ");
              lcd.setCursor(0, 1);
              lcd.print(study_minutes);
              break;
            case 2:
              lcd.print("Short break mins: ");
              lcd.setCursor(0, 1);
              lcd.print(short_break_minutes);
              break;
            case 3:
              lcd.print("Long break mins: ");
              lcd.setCursor(0, 1);
              lcd.print(long_break_minutes);
              break;
            case 4:
              lcd.print("# of repeats: ");
              lcd.setCursor(0, 1);
              lcd.print(repeats);
              break;
          }
        }
      }
    }
  }
  lastButtonState = reading;


  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    displayMessage();
    oldPosition = newPosition;
  }

  if (digitalRead(startButton) == HIGH) {
    startTimer();
  }
}

void displayMessage() {
  long newPosition = myEnc.read();
  int delta = newPosition - oldPosition;

  switch (messageNumber) {
    case 1:
      study_minutes = study_minutes + delta;
      if (study_minutes < 0) study_minutes = 0;
      Serial.print("Study time initialized to: ");
      Serial.print(study_minutes);
      Serial.println(" minutes");
      break;
    case 2:
      short_break_minutes = short_break_minutes + delta;
      if (short_break_minutes < 0) short_break_minutes = 0;
      break;
    case 3:
      long_break_minutes = long_break_minutes + delta;
      if (long_break_minutes < 0) long_break_minutes = 0;
      break;
    case 4:
      repeats = repeats + delta;
      if (repeats < 0) repeats = 0;
      break;
  }

  lcd.clear();
  switch (messageNumber) {
    case 1:
      lcd.print("Study mins: ");
      lcd.setCursor(0, 1);
      lcd.print(study_minutes);
      break;
    case 2:
      lcd.print("Short break mins: ");
      lcd.setCursor(0, 1);
      lcd.print(short_break_minutes);
      break;
    case 3:
      lcd.print("Long break mins: ");
      lcd.setCursor(0, 1);
      lcd.print(long_break_minutes);
      break;
    case 4:
      lcd.print("# of repeats: ");
      lcd.setCursor(0, 1);
      lcd.print(repeats);
      break;
  }

  oldPosition = newPosition;
}

void startTimer () {
  count = 0; 

  while(count<repeats){ 
    lcd.clear();
    lcd.print("Study time!");
    digitalWrite(ledGreen,LOW);
    digitalWrite(ledRed,HIGH);
    lcd.setCursor(0, 1);

    seconds = 60;
    minutes = study_minutes;
    minutes--;
    while(minutes >= 0){
      delay(1000);
      seconds = 59;
      while(seconds > 0){
        lcd.setCursor(0, 1);
        if(minutes < 10){
          lcd.print("0");
        }
        lcd.print(minutes);
        lcd.print(":");
        if(seconds < 10){  
          lcd.print("0");
        }
        lcd.print(seconds);
        delay(1000);
        seconds--;
      }
      minutes--;
    }
    
    lcd.clear();
    lcd.setCursor(0, 0);
    digitalWrite(ledGreen,HIGH);
    digitalWrite(ledRed,LOW);
    
    if(count==(repeats-1)){
      break_duration = long_break_minutes;
      lcd.print("Long break!");
    }
    else{ 
      break_duration = short_break_minutes;
      lcd.print("Short break!");
    }

    lcd.setCursor(0, 1);
    seconds = 60;
    minutes = break_duration;
    minutes--;

    while(break_duration >= 0) {
      delay(1000);
      seconds = 59;
      while(seconds > 0){
        lcd.setCursor(0, 1);
        if(minutes < 10){
          lcd.print("0");
        }
        lcd.print(minutes);
        lcd.print(":");
        if(seconds < 10){
          lcd.print("0");
        }
        lcd.print(seconds);
        delay(1000);
        seconds--;
      }
      minutes--;
    }
    count++;  
  }
}